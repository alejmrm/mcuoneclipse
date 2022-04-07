/*
 * Copyright (c) 2022, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "McuLittleFS.h"
#include "McuLittleFSMemory.h"
#include "McuShell.h"
#include "McuTimeDate.h"
#include "littleFS/lfs.h"

#define McuLFS_FILE_NAME_SIZE  60 /* Length of file name, used in buffers */
#define McuLFS_ACCESS_MUTEX_WAIT_TIME_MS 5000

/* variables used by the file system */
static bool McuLFS_isMounted = FALSE;
static lfs_t McuLFS_lfs;
static SemaphoreHandle_t fileSystemAccessMutex;

#define FILESYSTEM_READ_BUFFER_SIZE 256//256
#define FILESYSTEM_PROG_BUFFER_SIZE 256//256
#define FILESYSTEM_LOOKAHEAD_SIZE 256 //128
#define FILESYSTEM_CACHE_SIZE 256

bool McuLFS_IsMounted(void) {
  return McuLFS_isMounted;
}

static int block_device_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size) {
  uint8_t res;
  res = McuLFSMem_Read(block * c->block_size + off, buffer, size);
  if (res != ERR_OK) {
    return LFS_ERR_IO;
  }
  return LFS_ERR_OK;
}

int block_device_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) {
  uint8_t res;

  res = McuLFSMem_Program(block * c->block_size + off, buffer, size);
  if (res != ERR_OK) {
    return LFS_ERR_IO;
  }
  return LFS_ERR_OK;
}

int block_device_erase(const struct lfs_config *c, lfs_block_t block) {
  uint8_t res;

  res = McuLFSMem_EraseBlock(block * c->block_size);
  if (res != ERR_OK) {
    return LFS_ERR_IO;
  }
  return LFS_ERR_OK;
}

int block_device_sync(const struct lfs_config *c) {
  return LFS_ERR_OK;
}

// configuration of the file system is provided by this struct
static const struct lfs_config McuLFS_cfg = {
    // block device operations
    .read = block_device_read,
    .prog = block_device_prog,
    .erase =block_device_erase,
    .sync = block_device_sync,
    // block device configuration
    .read_size = FILESYSTEM_READ_BUFFER_SIZE,
    .prog_size = FILESYSTEM_PROG_BUFFER_SIZE,
    .block_size = 4096,
    .block_count = 16384, /* 16384 * 4K = 64 MByte */
    .cache_size = FILESYSTEM_CACHE_SIZE,
    .lookahead_size = FILESYSTEM_LOOKAHEAD_SIZE,
    .block_cycles = 500
};

/*-----------------------------------------------------------------------
 * Get a string from the file
 * (ported from FatFS function: f_gets())
 *-----------------------------------------------------------------------*/

char* McuLFS_gets (
  char* buff,  /* Pointer to the string buffer to read */
  int len,      /* Size of string buffer (characters) */
  lfs_file_t* fp       /* Pointer to the file object */
)
{
  int n = 0;
  char c, *p = buff;
  unsigned char s[2];
  uint32_t rc;

  if(xSemaphoreTakeRecursive(fileSystemAccessMutex,pdMS_TO_TICKS(McuLFS_ACCESS_MUTEX_WAIT_TIME_MS))) {
    while (n < len - 1) { /* Read characters until buffer gets filled */
      rc = lfs_file_read(&McuLFS_lfs,fp,s,1);
      if (rc != 1) break;
      c = s[0];

      if (c == '\r') continue; /* Strip '\r' */
      *p++ = c;
      n++;
      if (c == '\n') break;   /* Break on EOL */
    }
    *p = 0;
    xSemaphoreGiveRecursive(fileSystemAccessMutex);
    return n ? buff : 0;      /* When no data read (eof or error), return with error. */
  } else {
    return 0;
  }
}

/*-----------------------------------------------------------------------*
 * Put a character to the file
 * (ported from FatFS)
 *-----------------------------------------------------------------------*/

typedef struct {
  lfs_file_t* fp ;
  int idx, nchr;
  unsigned char buf[64];
} putbuff;


static void putc_bfd (
  putbuff* pb,
  char c
)
{
  uint32_t bw;
  int32_t i;

  if (c == '\n') {  /* LF -> CRLF conversion */
    putc_bfd(pb, '\r');
  }

  i = pb->idx;  /* Buffer write index (-1:error) */
  if (i < 0) return;

  pb->buf[i++] = (unsigned char)c;

  if (i >= (int)(sizeof pb->buf) - 3) { /* Write buffered characters to the file */
  bw = lfs_file_write(&McuLFS_lfs,pb->fp, pb->buf,(uint32_t)i);
    i = (bw == (uint32_t)i) ? 0 : -1;
  }
  pb->idx = i;
  pb->nchr++;
}

/*-----------------------------------------------------------------------*/
/* Put a string to the file
 * (ported from FatFS function: f_puts())                                            */
/*-----------------------------------------------------------------------*/

int McuLFS_puts (
  const char* str, /* Pointer to the string to be output */
  lfs_file_t* fp       /* Pointer to the file object */
)
{
  putbuff pb;
  uint32_t nw;

  if(xSemaphoreTakeRecursive(fileSystemAccessMutex,pdMS_TO_TICKS(McuLFS_ACCESS_MUTEX_WAIT_TIME_MS)))
  {
      pb.fp = fp;       /* Initialize output buffer */
      pb.nchr = pb.idx = 0;

      while (*str)      /* Put the string */
        putc_bfd(&pb, *str++);

      nw = lfs_file_write(&McuLFS_lfs,pb.fp, pb.buf, (uint32_t)pb.idx);

      if (   pb.idx >= 0    /* Flush buffered characters to the file */
        && nw>=0
        && (uint32_t)pb.idx == nw)
        {
            xSemaphoreGiveRecursive(fileSystemAccessMutex);
            return pb.nchr;
        }

      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return -1;
  }
  else
  {
    return 0;
  }
}

uint8_t McuLFS_Format(McuShell_ConstStdIOType *io)
{
  int res;

  if(xSemaphoreTakeRecursive(fileSystemAccessMutex,pdMS_TO_TICKS(McuLFS_ACCESS_MUTEX_WAIT_TIME_MS)))
  {
    if (McuLFS_isMounted)
    {
      if (io != NULL)
      {
        McuShell_SendStr((const uint8_t *)"File system is mounted, unmount it first.\r\n",io->stdErr);
      }
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
    if (io != NULL)
    {
      McuShell_SendStr((const uint8_t *)"Formatting ...", io->stdOut);
    }
    res = lfs_format(&McuLFS_lfs, &McuLFS_cfg);
    if (res == LFS_ERR_OK)
    {
      if (io != NULL)
      {
        McuShell_SendStr((const uint8_t *)" done.\r\n", io->stdOut);
      }
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_OK;
    }
    else
    {
      if (io != NULL)
      {
        McuShell_SendStr((const uint8_t *)" FAILED!\r\n", io->stdErr);
      }
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
  }
  else
  {
    return ERR_BUSY;
  }
}

uint8_t McuLFS_Mount(McuShell_ConstStdIOType *io)
{
  int res;

  if(xSemaphoreTakeRecursive(fileSystemAccessMutex,pdMS_TO_TICKS(McuLFS_ACCESS_MUTEX_WAIT_TIME_MS)))
  {
    if (McuLFS_isMounted)
    {
      if (io != NULL)
      {
        McuShell_SendStr((const uint8_t *)"File system is already mounted.\r\n", io->stdErr);
      }
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
    if (io != NULL)
    {
      McuShell_SendStr((const uint8_t *)"Mounting ...", io->stdOut);
    }
    res = lfs_mount(&McuLFS_lfs, &McuLFS_cfg);
    if (res == LFS_ERR_OK)
    {
      if (io != NULL)
      {
        McuShell_SendStr((const uint8_t *)" done.\r\n", io->stdOut);
      }
      McuLFS_isMounted = TRUE;
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_OK;
    }
    else
    {
      if (io != NULL) {
        McuShell_SendStr((const uint8_t *)" FAILED!\r\n", io->stdErr);
      }
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
  }
  else
  {
    return ERR_BUSY;
  }
}

uint8_t McuLFS_Unmount(McuShell_ConstStdIOType *io)
{
  int res;

  if(xSemaphoreTakeRecursive(fileSystemAccessMutex,pdMS_TO_TICKS(McuLFS_ACCESS_MUTEX_WAIT_TIME_MS)))
  {
    if (!McuLFS_isMounted)
    {
      if (io != NULL)
      {
        McuShell_SendStr((const uint8_t *)"File system is already unmounted.\r\n", io->stdErr);
      }
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
    if (io != NULL)
    {
      McuShell_SendStr((const uint8_t *)"Unmounting ...", io->stdOut);
    }
    res = lfs_unmount(&McuLFS_lfs);
    if (res == LFS_ERR_OK)
    {
      if (io != NULL)
      {
        McuShell_SendStr((const uint8_t *)" done.\r\n", io->stdOut);
      }
      McuLFS_isMounted = FALSE;
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_OK;
    }
    else
    {
      if (io != NULL)
      {
        McuShell_SendStr((const uint8_t *)" FAILED!\r\n", io->stdErr);
      }
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
  }
  else
  {
    return ERR_BUSY;
  }
}

uint8_t McuLFS_Dir(const char *path, McuShell_ConstStdIOType *io) {
  int res;
  lfs_dir_t dir;
  struct lfs_info info;

  if (io == NULL)
  {
    return ERR_FAILED; /* listing a directory without an I/O channel does not make any sense */
  }

  if(xSemaphoreTakeRecursive(fileSystemAccessMutex,pdMS_TO_TICKS(McuLFS_ACCESS_MUTEX_WAIT_TIME_MS)))
  {
    if (!McuLFS_isMounted)
    {
      McuShell_SendStr((const uint8_t *)"File system is not mounted, mount it first.\r\n", io->stdErr);
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
    if (path == NULL)
    {
      path = "/"; /* default path */
    }
    res = lfs_dir_open(&McuLFS_lfs, &dir, path);
    if (res != LFS_ERR_OK)
    {
      McuShell_SendStr((const uint8_t *)"FAILED lfs_dir_open()!\r\n", io->stdErr);
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
    for (;;)
    {
      res = lfs_dir_read(&McuLFS_lfs, &dir, &info);
      if (res < 0)
      {
        McuShell_SendStr((const uint8_t *)"FAILED lfs_dir_read()!\r\n", io->stdErr);
        xSemaphoreGiveRecursive(fileSystemAccessMutex);
        return ERR_FAILED;
      }
      if (res == 0)
      { /* no more files */
        break;
      }
      if(!(McuUtility_strcmp(info.name,".") == 0 || McuUtility_strcmp(info.name,"..") == 0 ))
      {
        switch (info.type)
        {
        case LFS_TYPE_REG:
          McuShell_SendStr((const uint8_t *)"reg ", io->stdOut);
          break;
        case LFS_TYPE_DIR:
          McuShell_SendStr((const uint8_t *)"dir ", io->stdOut);
          break;
        default:
          McuShell_SendStr((const uint8_t *)"?   ", io->stdOut);
          break;
        }
        static const char *prefixes[] = { "", "K", "M", "G" }; /* prefixes for kilo, mega and giga */
        for (int i = sizeof(prefixes) / sizeof(prefixes[0]) - 1; i >= 0; i--)
        {
          if (info.size >= (1 << 10 * i) - 1)
          {
            McuShell_printf("%*u%sB ", 4 - (i != 0), info.size >> 10 * i,
                prefixes[i]); /* \todo: remove printf */
            break;
          }
        } /* for */
        McuShell_SendStr((const uint8_t *)info.name, io->stdOut);
        McuShell_SendStr((const uint8_t *)"\r\n", io->stdOut);
      }
    } /* for */
    res = lfs_dir_close(&McuLFS_lfs, &dir);
    if (res != LFS_ERR_OK) {
      McuShell_SendStr((const uint8_t *)"FAILED lfs_dir_close()!\r\n", io->stdErr);
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
    xSemaphoreGiveRecursive(fileSystemAccessMutex);
    return ERR_OK;
  }
  else
  {
    return ERR_BUSY;
  }
}

/*
 * Prints a list of Files and Directories of a given path
 * If path == NULL, the Files and Direcotries of the root-directory are printed
 * The First two characters of every line determin if its a File (F:) or a Directory (D:)
 */
uint8_t McuLFS_FileList(const char *path, McuShell_ConstStdIOType *io)
{
  int res;
  lfs_dir_t dir;
  struct lfs_info info;

  if (io == NULL)
  {
    return ERR_FAILED; /* listing a directory without an I/O channel does not make any sense */
  }
  if(xSemaphoreTakeRecursive(fileSystemAccessMutex,pdMS_TO_TICKS(McuLFS_ACCESS_MUTEX_WAIT_TIME_MS)))
  {
    if (!McuLFS_isMounted)
    {
      McuShell_SendStr((const uint8_t *)"File system is not mounted, mount it first.\r\n", io->stdErr);
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
    if (path == NULL)
    {
      path = "/"; /* default path */
    }
    res = lfs_dir_open(&McuLFS_lfs, &dir, path);
    if (res != LFS_ERR_OK)
    {
      McuShell_SendStr((const uint8_t *)"FAILED lfs_dir_open()!\r\n", io->stdErr);
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
    for (;;)
    {
      res = lfs_dir_read(&McuLFS_lfs, &dir, &info);
      if (res < 0)
      {
        McuShell_SendStr((const uint8_t *)"FAILED lfs_dir_read()!\r\n", io->stdErr);
        xSemaphoreGiveRecursive(fileSystemAccessMutex);
        return ERR_FAILED;
      }
      if (res == 0)
      { /* no more files */
        break;
      }

      if(!(McuUtility_strcmp(info.name,".") == 0 || McuUtility_strcmp(info.name,"..") == 0 ))
      {
        switch (info.type)
        {
        case LFS_TYPE_REG:
          McuShell_SendStr((const uint8_t *)"F:", io->stdOut);
          break;
        case LFS_TYPE_DIR:
          McuShell_SendStr((const uint8_t *)"D:", io->stdOut);
          break;
        default:
          McuShell_SendStr((const uint8_t *)"?:", io->stdOut);
          break;
        }

        McuShell_SendStr((const uint8_t *)info.name, io->stdOut);
        McuShell_SendStr((const uint8_t *)"\r\n", io->stdOut);
      }

    } /* for */
    res = lfs_dir_close(&McuLFS_lfs, &dir);
    if (res != LFS_ERR_OK) {
      McuShell_SendStr((const uint8_t *)"FAILED lfs_dir_close()!\r\n", io->stdErr);
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
    xSemaphoreGiveRecursive(fileSystemAccessMutex);
    return ERR_OK;
  }
  else
  {
    return ERR_BUSY;
  }
}

uint8_t McuLFS_CopyFile(const char *srcPath, const char *dstPath,McuShell_ConstStdIOType *io)
{
  lfs_file_t fsrc, fdst;
  int result, nofBytesRead;
  uint8_t buffer[32]; /* copy buffer */
  uint8_t res = ERR_OK;

  if(xSemaphoreTakeRecursive(fileSystemAccessMutex,pdMS_TO_TICKS(McuLFS_ACCESS_MUTEX_WAIT_TIME_MS)))
  {
    if (!McuLFS_isMounted)
    {
      if (io != NULL)
      {
        McuShell_SendStr((const uint8_t *)"ERROR: File system is not mounted.\r\n", io->stdErr);
      }
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }

    /* open source file */
    result = lfs_file_open(&McuLFS_lfs, &fsrc, srcPath, LFS_O_RDONLY);
    if (result < 0)
    {
      McuShell_SendStr((const unsigned char*) "*** Failed opening source file!\r\n",io->stdErr);
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
    /* create destination file */
    result = lfs_file_open(&McuLFS_lfs, &fdst, dstPath, LFS_O_WRONLY | LFS_O_CREAT);
    if (result < 0)
    {
      (void) lfs_file_close(&McuLFS_lfs, &fsrc);
      McuShell_SendStr((const unsigned char*) "*** Failed opening destination file!\r\n", io->stdErr);
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
    /* now copy source to destination */
    for (;;)
    {
      nofBytesRead = lfs_file_read(&McuLFS_lfs, &fsrc, buffer, sizeof(buffer));
      if (nofBytesRead < 0)
      {
        McuShell_SendStr((const unsigned char*) "*** Failed reading source file!\r\n",io->stdErr);
        res = ERR_FAILED;
        break;
      }
      if (nofBytesRead == 0) { /* end of file */
        break;
      }
      result = lfs_file_write(&McuLFS_lfs, &fdst, buffer, nofBytesRead);
      if (result < 0)
      {
        McuShell_SendStr((const unsigned char*) "*** Failed writing destination file!\r\n", io->stdErr);
        res = ERR_FAILED;
        break;
      }
    } /* for */
    /* close all files */
    result = lfs_file_close(&McuLFS_lfs, &fsrc);
    if (result < 0)
    {
      McuShell_SendStr((const unsigned char*) "*** Failed closing source file!\r\n",  io->stdErr);
      res = ERR_FAILED;
    }
    result = lfs_file_close(&McuLFS_lfs, &fdst);
    if (result < 0)
    {
      McuShell_SendStr((const unsigned char*) "*** Failed closing destination file!\r\n", io->stdErr);
      res = ERR_FAILED;
    }
    xSemaphoreGiveRecursive(fileSystemAccessMutex);
    return res;
  }
  else
  {
    return ERR_BUSY;
  }
}

uint8_t McuLFS_MoveFile(const char *srcPath, const char *dstPath,McuShell_ConstStdIOType *io)
{
  if(xSemaphoreTakeRecursive(fileSystemAccessMutex,pdMS_TO_TICKS(McuLFS_ACCESS_MUTEX_WAIT_TIME_MS)))
  {
    if (!McuLFS_isMounted)
    {
      if (io != NULL)
      {
        McuShell_SendStr((const uint8_t *)"ERROR: File system is not mounted.\r\n", io->stdErr);
      }
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
    if (lfs_rename(&McuLFS_lfs, srcPath, dstPath) < 0)
    {
      if (io != NULL)
      {
        McuShell_SendStr((const uint8_t *)"ERROR: failed renaming file or directory.\r\n",io->stdErr);
      }
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
    xSemaphoreGiveRecursive(fileSystemAccessMutex);
    return ERR_OK;
  }
  else
  {
    return ERR_BUSY;
  }
}


/*
 * Used to read out data from Files for SDEP communication
 */
uint8_t McuLFS_ReadFile(lfs_file_t* file, bool readFromBeginning, size_t nofBytes, McuShell_ConstStdIOType *io)
{
  static int32_t filePos;
  size_t fileSize;
  uint8_t buf[1024];

  if( nofBytes > 1024)
  {
    nofBytes = 1024;
  }

  if(xSemaphoreTakeRecursive(fileSystemAccessMutex,pdMS_TO_TICKS(McuLFS_ACCESS_MUTEX_WAIT_TIME_MS)))
  {
    if(readFromBeginning)
    {
      lfs_file_rewind(&McuLFS_lfs,file);
      filePos = 0;
    }
    else
    {
      lfs_file_seek(&McuLFS_lfs,file, filePos,LFS_SEEK_SET);
    }

    fileSize = lfs_file_size(&McuLFS_lfs, file);
    filePos = lfs_file_tell(&McuLFS_lfs, file);
    fileSize = fileSize - filePos;

    if (fileSize < 0)
    {
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }

    if(fileSize > nofBytes)
    {
      if (lfs_file_read(&McuLFS_lfs, file, buf, nofBytes) < 0)
      {
        return ERR_FAILED;
      }
      McuShell_SendData(buf,nofBytes,io->stdErr);
      filePos = filePos + nofBytes;
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_OK;
    }
    else
    {
      if (lfs_file_read(&McuLFS_lfs, file, buf, fileSize) < 0)
      {
        return ERR_FAILED;
      }
      McuShell_SendData(buf,fileSize,io->stdErr);
      filePos = filePos + fileSize;
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_PARAM_SIZE; //EOF
    }
  }
  else
  {
    return ERR_BUSY;
  }
}


uint8_t McuLFS_getLiDoSampleOutOfFile(lfs_file_t* file,uint8_t* sampleBuf,size_t bufSize,uint8_t* nofReadChars)
{
  sampleBuf[0] = '\0';

  if(xSemaphoreTakeRecursive(fileSystemAccessMutex,pdMS_TO_TICKS(McuLFS_ACCESS_MUTEX_WAIT_TIME_MS)))
  {
    *nofReadChars = lfs_file_read(&McuLFS_lfs, file, sampleBuf, bufSize);
    xSemaphoreGiveRecursive(fileSystemAccessMutex);
    return ERR_OK;
  }
  else
  {
    return ERR_BUSY;
  }
}

uint8_t McuLFS_openFile(lfs_file_t* file, uint8_t* filename) {
  if(xSemaphoreTakeRecursive(fileSystemAccessMutex,pdMS_TO_TICKS(McuLFS_ACCESS_MUTEX_WAIT_TIME_MS))) {
    if (lfs_file_open(&McuLFS_lfs, file, (const char*)filename, LFS_O_RDWR | LFS_O_CREAT| LFS_O_APPEND) < 0) {
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
    xSemaphoreGiveRecursive(fileSystemAccessMutex);
    return ERR_OK;
  } else {
    return ERR_BUSY;
  }
}

uint8_t McuLFS_closeFile(lfs_file_t* file) {
  if(xSemaphoreTakeRecursive(fileSystemAccessMutex,pdMS_TO_TICKS(McuLFS_ACCESS_MUTEX_WAIT_TIME_MS))) {
    if(lfs_file_close(&McuLFS_lfs, file) ==0) {
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_OK;
    } else {
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
  } else {
    return ERR_BUSY;
  }
}

uint8_t McuLFS_writeLine(lfs_file_t* file,uint8_t* line) {
  uint8_t lineBuf[200];
  McuUtility_strcpy(lineBuf, sizeof(lineBuf), line);
  McuUtility_strcat(lineBuf, sizeof(lineBuf), (unsigned char*)"\r\n");

  if(xSemaphoreTakeRecursive(fileSystemAccessMutex,pdMS_TO_TICKS(McuLFS_ACCESS_MUTEX_WAIT_TIME_MS))) {
    if (lfs_file_write(&McuLFS_lfs, file, lineBuf, McuUtility_strlen((char*)lineBuf)) < 0) {
      lfs_file_close(&McuLFS_lfs, file);
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
    xSemaphoreGiveRecursive(fileSystemAccessMutex);
    return ERR_OK;
  } else {
    return ERR_BUSY;
  }
}

uint8_t McuLFS_readLine(lfs_file_t* file,uint8_t* lineBuf,size_t bufSize,uint8_t* nofReadChars)
{
  lineBuf[0] = '\0';
  uint8_t ch;
  *nofReadChars = 0;

  if(xSemaphoreTakeRecursive(fileSystemAccessMutex,pdMS_TO_TICKS(McuLFS_ACCESS_MUTEX_WAIT_TIME_MS)))
  {
    while(lfs_file_read(&McuLFS_lfs, file, &ch, 1) != 0 &&  ch != '\n')
    {
      (*nofReadChars)++;
      McuUtility_chcat(lineBuf,200,ch);
    }
    McuUtility_chcat(lineBuf,200,ch);
    xSemaphoreGiveRecursive(fileSystemAccessMutex);
    return ERR_OK;
  }
  else
  {
    return ERR_BUSY;
  }

}


//Function for the Shell PrintHex command
static uint8_t readFromFile(void *hndl, uint32_t addr, uint8_t *buf, size_t bufSize)
{
  lfs_file_t *fp;

  fp = (lfs_file_t*)hndl;
  if (lfs_file_read(&McuLFS_lfs, fp, buf, bufSize) < 0)
  {
    return ERR_FAILED;
  }
  return ERR_OK;
}

uint8_t McuLFS_PrintFile(const char *filePath, McuShell_ConstStdIOType *io, bool inHex)
{
  lfs_file_t file;
  uint8_t res = ERR_OK;
  int32_t fileSize;
  int result;

  if(xSemaphoreTakeRecursive(fileSystemAccessMutex,pdMS_TO_TICKS(McuLFS_ACCESS_MUTEX_WAIT_TIME_MS)) == pdTRUE)
  {
    if (io == NULL)
    {
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED; /* printing a file without an I/O channel does not make any sense */
    }
    if (!McuLFS_isMounted)
    {
      if (io != NULL)
      {
        McuShell_SendStr((const uint8_t *)"ERROR: File system is not mounted.\r\n", io->stdErr);
      }
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
    result = lfs_file_open(&McuLFS_lfs, &file, filePath, LFS_O_RDONLY);
    if (result < 0)
    {
      if (io != NULL)
      {
        McuShell_SendStr((const uint8_t *)"ERROR: Failed opening file.\r\n", io->stdErr);
      }
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
    fileSize = lfs_file_size(&McuLFS_lfs, &file);
    if (fileSize < 0)
    {
      if (io != NULL)
      {
        McuShell_SendStr((const uint8_t *)"ERROR: getting file size\r\n", io->stdErr);
        (void) lfs_file_close(&McuLFS_lfs, &file);
      }
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
    if (inHex) {
      res = McuShell_PrintMemory(&file, 0, fileSize-1, 4, 16, readFromFile, io);
      if (res != ERR_OK)
      {
        McuShell_SendStr((const uint8_t *)"ERROR while calling PrintMemory()\r\n", io->stdErr);
      }
    } else {
      uint8_t ch;

      while(fileSize>0) {
        if (lfs_file_read(&McuLFS_lfs, &file, &ch, sizeof(ch)) < 0) {
          break; /* error case */
        }
        McuShell_SendCh(ch, io->stdOut); /* print character */
        fileSize--;
      }
    }
    (void) lfs_file_close(&McuLFS_lfs, &file);
    xSemaphoreGiveRecursive(fileSystemAccessMutex);
    return res;
  } else {
    return ERR_BUSY;
  }
}

uint8_t McuLFS_RemoveFile(const char *filePath, McuShell_ConstStdIOType *io)
{
  int result;

  if(xSemaphoreTakeRecursive(fileSystemAccessMutex,pdMS_TO_TICKS(McuLFS_ACCESS_MUTEX_WAIT_TIME_MS)))
  {
    if (!McuLFS_isMounted)
    {
      if (io != NULL)
      {
        McuShell_SendStr((const uint8_t *)"ERROR: File system is not mounted.\r\n", io->stdErr);
      }
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }
    result = lfs_remove(&McuLFS_lfs, filePath);
    if (result < 0)
    {
      if (io != NULL)
      {
        McuShell_SendStr((const uint8_t *)"ERROR: Failed removing file.\r\n", io->stdErr);
      }
      xSemaphoreGiveRecursive(fileSystemAccessMutex);
      return ERR_FAILED;
    }

    xSemaphoreGiveRecursive(fileSystemAccessMutex);
    return ERR_OK;
  }
  else
  {
    return ERR_BUSY;
  }
}

lfs_t* McuLFS_GetFileSystem(void)
{
  return &McuLFS_lfs;
}

uint8_t McuLFS_RunBenchmark(const McuShell_ConstStdIOType *io) {
  lfs_file_t file;
  uint32_t i;
  uint8_t read_buf[10];
  TIMEREC time, startTime;
  int32_t start_mseconds, mseconds;

  if (!McuLFS_isMounted) {
    if (io!=NULL) {
      McuShell_SendStr((const unsigned char*)"ERROR: File system is not mounted.\r\n", io->stdErr);
    }
    return ERR_FAILED;
  }
  /* write benchmark */
  McuShell_SendStr((const unsigned char*)"Benchmark: write/copy/read a 100kB file:\r\n", io->stdOut);
  McuShell_SendStr((const unsigned char*)"Delete existing benchmark files...\r\n", io->stdOut);
  (void)McuLFS_RemoveFile((const char*)"./bench.txt", io);
  (void)McuLFS_RemoveFile((const char*)"./copy.txt", io);

  McuShell_SendStr((const unsigned char*)"Create benchmark file...\r\n", io->stdOut);
  (void)McuTimeDate_GetTime(&startTime);
  if (lfs_file_open(&McuLFS_lfs, &file, "./bench.txt", LFS_O_WRONLY | LFS_O_CREAT)<0) {
    McuShell_SendStr((const unsigned char*)"*** Failed creating benchmark file!\r\n", io->stdErr);
    return ERR_FAILED;
  }
  for(i=0;i<10240;i++) {
    if (lfs_file_write(&McuLFS_lfs, &file, "benchmark ", sizeof("benchmark ")-1)<0) {
      McuShell_SendStr((const unsigned char*)"*** Failed writing file!\r\n", io->stdErr);
      (void)lfs_file_close(&McuLFS_lfs, &file);
      return ERR_FAILED;
    }
  }
  (void)lfs_file_close(&McuLFS_lfs, &file);
  (void)McuTimeDate_GetTime(&time);
  start_mseconds = startTime.Hour*60*60*1000 + startTime.Min*60*1000 + startTime.Sec*1000
#if TmDt1_HAS_SEC100_IN_TIMEREC
  + startTime.Sec100*10
#endif
  ;
  mseconds = time.Hour*60*60*1000 + time.Min*60*1000 + time.Sec*1000
#if TmDt1_HAS_SEC100_IN_TIMEREC
  + time.Sec100*10
#endif
  - start_mseconds;
  McuShell_SendNum32s(mseconds, io->stdOut);
  McuShell_SendStr((const unsigned char*)" ms for writing (", io->stdOut);
  McuShell_SendNum32s((100*1000)/mseconds, io->stdOut);
  McuShell_SendStr((const unsigned char*)" kB/s)\r\n", io->stdOut);

  /* read benchmark */
  McuShell_SendStr((const unsigned char*)"Read 100kB benchmark file...\r\n", io->stdOut);
  (void)McuTimeDate_GetTime(&startTime);
  if (lfs_file_open(&McuLFS_lfs, &file, "./bench.txt", LFS_O_RDONLY)<0) {
    McuShell_SendStr((const unsigned char*)"*** Failed opening benchmark file!\r\n", io->stdErr);
    return ERR_FAILED;
  }
  for(i=0;i<10240;i++) {
    if (lfs_file_read(&McuLFS_lfs, &file, &read_buf[0], sizeof(read_buf))<0) {
      McuShell_SendStr((const unsigned char*)"*** Failed reading file!\r\n", io->stdErr);
      (void)lfs_file_close(&McuLFS_lfs, &file);
      return ERR_FAILED;
    }
  }
  (void)lfs_file_close(&McuLFS_lfs, &file);
  (void)McuTimeDate_GetTime(&time);
  start_mseconds = startTime.Hour*60*60*1000 + startTime.Min*60*1000 + startTime.Sec*1000
#if TmDt1_HAS_SEC100_IN_TIMEREC
  + startTime.Sec100*10
#endif
  ;
  mseconds = time.Hour*60*60*1000 + time.Min*60*1000 + time.Sec*1000
#if TmDt1_HAS_SEC100_IN_TIMEREC
  + time.Sec100*10
#endif
  - start_mseconds;
  McuShell_SendNum32s(mseconds, io->stdOut);
  McuShell_SendStr((const unsigned char*)" ms for reading (", io->stdOut);
  McuShell_SendNum32s((100*1000)/mseconds, io->stdOut);
  McuShell_SendStr((const unsigned char*)" kB/s)\r\n", io->stdOut);

  /* copy benchmark */
  McuShell_SendStr((const unsigned char*)"Copy 100kB file...\r\n", io->stdOut);
  (void)McuTimeDate_GetTime(&startTime);
  (void)McuLFS_CopyFile((const char*)"./bench.txt", (const char*)"./copy.txt", io);
  (void)McuTimeDate_GetTime(&time);
  start_mseconds = startTime.Hour*60*60*1000 + startTime.Min*60*1000 + startTime.Sec*1000
#if TmDt1_HAS_SEC100_IN_TIMEREC
  + startTime.Sec100*10
#endif
  ;
  mseconds = time.Hour*60*60*1000 + time.Min*60*1000 + time.Sec*1000
#if TmDt1_HAS_SEC100_IN_TIMEREC
  + time.Sec100*10
#endif
  - start_mseconds;
  McuShell_SendNum32s(mseconds, io->stdOut);
  McuShell_SendStr((const unsigned char*)" ms for copy (", io->stdOut);
  McuShell_SendNum32s((100*1000)/mseconds, io->stdOut);
  McuShell_SendStr((const unsigned char*)" kB/s)\r\n", io->stdOut);
  McuShell_SendStr((const unsigned char*)"done!\r\n", io->stdOut);
  return ERR_OK;
}

static uint8_t McuLFS_PrintStatus(McuShell_ConstStdIOType *io)
{
  uint8_t buf[24];

  McuShell_SendStatusStr((const unsigned char*) "FS", (const unsigned char*) "\r\n", io->stdOut);
  McuShell_SendStatusStr((const unsigned char*) "  mounted",  McuLFS_isMounted ? (const uint8_t *)"yes\r\n" : (const uint8_t *)"no\r\n", io->stdOut);

  McuUtility_Num32uToStr(buf, sizeof(buf), McuLFS_cfg.block_count * McuLFS_cfg.block_size);
  McuUtility_strcat(buf, sizeof(buf), (const uint8_t *)" bytes\r\n");
  McuShell_SendStatusStr((const unsigned char*) "  space", buf, io->stdOut);

  McuUtility_Num32uToStr(buf, sizeof(buf), McuLFS_cfg.read_size);
  McuUtility_strcat(buf, sizeof(buf), (const uint8_t *)"\r\n");
  McuShell_SendStatusStr((const unsigned char*) "  read_size", buf, io->stdOut);

  McuUtility_Num32uToStr(buf, sizeof(buf), McuLFS_cfg.prog_size);
  McuUtility_strcat(buf, sizeof(buf), (const uint8_t *)"\r\n");
  McuShell_SendStatusStr((const unsigned char*) "  prog_size", buf, io->stdOut);

  McuUtility_Num32uToStr(buf, sizeof(buf), McuLFS_cfg.block_size);
  McuUtility_strcat(buf, sizeof(buf), (const uint8_t *)"\r\n");
  McuShell_SendStatusStr((const unsigned char*) "  block_size", buf, io->stdOut);

  McuUtility_Num32uToStr(buf, sizeof(buf), McuLFS_cfg.block_count);
  McuUtility_strcat(buf, sizeof(buf), (const uint8_t *)"\r\n");
  McuShell_SendStatusStr((const unsigned char*) "  block_count", buf, io->stdOut);

  McuUtility_Num32uToStr(buf, sizeof(buf), McuLFS_cfg.lookahead_size);
  McuUtility_strcat(buf, sizeof(buf), (const uint8_t *)"\r\n");
  McuShell_SendStatusStr((const unsigned char*) "  lookahead", buf, io->stdOut);
  return ERR_OK;
}

uint8_t McuLFS_ParseCommand(const unsigned char* cmd, bool *handled,const McuShell_StdIOType *io)
{
  unsigned char fileNameSrc[McuLFS_FILE_NAME_SIZE],fileNameDst[McuLFS_FILE_NAME_SIZE];
  size_t lenRead;

  if (McuUtility_strcmp((char*)cmd, McuShell_CMD_HELP) == 0|| McuUtility_strcmp((char*)cmd, "FS help") == 0) {
    McuShell_SendHelpStr((unsigned char*) "FS", (const unsigned char*) "Group of FileSystem (LittleFS) commands\r\n", io->stdOut);
    McuShell_SendHelpStr((unsigned char*) "  help|status",  (const unsigned char*) "Print help or status information\r\n",  io->stdOut);
    McuShell_SendHelpStr((unsigned char*) "  format",(const unsigned char*) "Format the file system\r\n",io->stdOut);
    McuShell_SendHelpStr((unsigned char*) "  mount",(const unsigned char*) "Mount the file system\r\n", io->stdOut);
    McuShell_SendHelpStr((unsigned char*) "  unmount",(const unsigned char*) "unmount the file system\r\n", io->stdOut);
    McuShell_SendHelpStr((unsigned char*) "  ls",(const unsigned char*) "List directory and files of root\r\n", io->stdOut);
    McuShell_SendHelpStr((unsigned char*) "  ls <dir>",(const unsigned char*) "List directory and files of dir (ex. /Samples)\r\n", io->stdOut);
    McuShell_SendHelpStr((unsigned char*) "  rm <file>",(const unsigned char*) "Remove a file\r\n", io->stdOut);
    McuShell_SendHelpStr((unsigned char*) "  mv <src> <dst>",(const unsigned char*) "Rename a file\r\n", io->stdOut);
    McuShell_SendHelpStr((unsigned char*) "  cp <src> <dst>",(const unsigned char*) "Copy a file\r\n", io->stdOut);
    McuShell_SendHelpStr((unsigned char*) "  printhex <file>",(const unsigned char*) "Print the file data in hexadecimal format\r\n",io->stdOut);
    McuShell_SendHelpStr((unsigned char*) "  printtxt <file>",(const unsigned char*) "Print the file data in text format\r\n",io->stdOut);
    McuShell_SendHelpStr((unsigned char*) "  benchmark",(const unsigned char*) "Run a benchmark to measure performance\r\n",io->stdOut);
    *handled = TRUE;
    return ERR_OK;
  } else if (McuUtility_strcmp((char*)cmd, McuShell_CMD_STATUS) == 0|| McuUtility_strcmp((char*)cmd, "FS status") == 0) {
    *handled = TRUE;
    return McuLFS_PrintStatus(io);
  } else if (McuUtility_strcmp((char*)cmd, "FS format") == 0) {
    *handled = TRUE;
    return McuLFS_Format(io);
  } else if (McuUtility_strcmp((char*)cmd, "FS mount") == 0) {
    *handled = TRUE;
    return McuLFS_Mount(io);
  } else if (McuUtility_strcmp((char*)cmd, "FS unmount") == 0) {
    *handled = TRUE;
    return McuLFS_Unmount(io);
  } else if (McuUtility_strcmp((char*)cmd, "FS ls") == 0) {
    *handled = TRUE;
    return McuLFS_Dir(NULL, io);
  } else if (McuUtility_strncmp((char* )cmd, "FS ls ", sizeof("FS ls ") - 1) == 0) {
    *handled = TRUE;
    if ((McuUtility_ReadEscapedName(cmd + sizeof("FS ls ") - 1,fileNameSrc, sizeof(fileNameSrc), &lenRead, NULL, NULL) == ERR_OK))  {
      return McuLFS_Dir((const char*)fileNameSrc, io);
    }
    return ERR_FAILED;
  } else if (McuUtility_strcmp((char*)cmd, "FS benchmark") == 0) {
    *handled = TRUE;
    return McuLFS_RunBenchmark(io);
  } else if (McuUtility_strncmp((char* )cmd, "FS printhex ", sizeof("FS printhex ") - 1) == 0) {
    *handled = TRUE;
    if ((McuUtility_ReadEscapedName(cmd + sizeof("FS printhex ") - 1,fileNameSrc, sizeof(fileNameSrc), &lenRead, NULL, NULL) == ERR_OK)) {
      return McuLFS_PrintFile((const char*)fileNameSrc, io, TRUE);
    }
    return ERR_FAILED;
  } else if (McuUtility_strncmp((char* )cmd, "FS printtxt ", sizeof("FS printtxt ") - 1) == 0) {
      *handled = TRUE;
      if ((McuUtility_ReadEscapedName(cmd + sizeof("FS printtxt ") - 1,fileNameSrc, sizeof(fileNameSrc), &lenRead, NULL, NULL) == ERR_OK)) {
        return McuLFS_PrintFile((const char*)fileNameSrc, io, FALSE);
      }
      return ERR_FAILED;
  } else if (McuUtility_strncmp((char*)cmd, "FS rm ", sizeof("FS rm ")-1) == 0) {
    *handled = TRUE;
    if ((McuUtility_ReadEscapedName(cmd + sizeof("FS rm ") - 1, fileNameSrc, sizeof(fileNameSrc), &lenRead, NULL, NULL) == ERR_OK)) {
      return McuLFS_RemoveFile((const char*)fileNameSrc, io);
    }
    return ERR_FAILED;
  } else if (McuUtility_strncmp((char*)cmd, "FS mv ", sizeof("FS mv ")-1) == 0) {
    *handled = TRUE;
    if ((McuUtility_ReadEscapedName(cmd + sizeof("FS mv ") - 1, fileNameSrc, sizeof(fileNameSrc), &lenRead, NULL, NULL) == ERR_OK)
        && *(cmd + sizeof("FS cp ") - 1 + lenRead) == ' '
        && (McuUtility_ReadEscapedName(cmd + sizeof("FS mv ") - 1 + lenRead + 1, fileNameDst,sizeof(fileNameDst), NULL, NULL, NULL) == ERR_OK))
    {
      return McuLFS_MoveFile((const char*)fileNameSrc, (const char*)fileNameDst, io);
    }
    return ERR_FAILED;
  } else if (McuUtility_strncmp((char*)cmd, "FS cp ", sizeof("FS cp ")-1) == 0) {
    *handled = TRUE;
    if ((McuUtility_ReadEscapedName(cmd + sizeof("FS cp ") - 1, fileNameSrc, sizeof(fileNameSrc), &lenRead, NULL, NULL) == ERR_OK)
        && *(cmd + sizeof("FS cp ") - 1 + lenRead) == ' '
        && (McuUtility_ReadEscapedName(cmd + sizeof("FS cp ") - 1 + lenRead + 1, fileNameDst,sizeof(fileNameDst), NULL, NULL, NULL) == ERR_OK))
    {
      return McuLFS_CopyFile((const char*)fileNameSrc, (const char*)fileNameDst, io);
    }
    return ERR_FAILED;
  }
  return ERR_OK;
}

void McuLFS_GetFileAccessSemaphore(SemaphoreHandle_t *sema) {
  *sema = fileSystemAccessMutex;
}

uint8_t McuLFS_Init(void) {
  fileSystemAccessMutex = xSemaphoreCreateRecursiveMutex();
  if( fileSystemAccessMutex == NULL) {
      for(;;) {} /* Error */
  }
  xSemaphoreGiveRecursive(fileSystemAccessMutex);
  if (McuLFSMem_Init() != ERR_OK) {
    return ERR_FAILED;
  }
  return McuLFS_Mount(NULL);
}

uint8_t McuLFS_FormatInit(void) {
  fileSystemAccessMutex = xSemaphoreCreateRecursiveMutex();
  if( fileSystemAccessMutex == NULL) {
      for(;;) {} /* Error */
  }
  xSemaphoreGiveRecursive(fileSystemAccessMutex);
  if (McuLFSMem_Init() != ERR_OK) {
    return ERR_FAILED;
  }
  McuLFS_Format(NULL);
  return McuLFS_Mount(NULL);
}
