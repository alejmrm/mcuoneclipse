/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : DQ1.h
**     Project     : FRDM-K64F_Generator
**     Processor   : MK64FN1M0VLL12
**     Component   : SDK_BitIO
**     Version     : Component 01.030, Driver 01.00, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2024-07-29, 05:47, # CodeGen: 836
**     Abstract    :
**          GPIO component usable with NXP SDK
**     Settings    :
**          Component name                                 : DQ1
**          SDK                                            : McuLib
**          GPIO Name                                      : GPIOA
**          PORT Name                                      : PORTA
**          Pin Number                                     : 0
**          Pin Symbol                                     : OneWireData
**          Do Pin Muxing                                  : no
**          Init Direction                                 : Input
**          Pull Resistor                                  : no pull resistor
**          Init Value                                     : 0
**     Contents    :
**         GetDir    - bool DQ1_GetDir(void);
**         SetDir    - void DQ1_SetDir(bool Dir);
**         SetInput  - void DQ1_SetInput(void);
**         SetOutput - void DQ1_SetOutput(void);
**         GetVal    - bool DQ1_GetVal(void);
**         PutVal    - void DQ1_PutVal(bool Val);
**         ClrVal    - void DQ1_ClrVal(void);
**         SetVal    - void DQ1_SetVal(void);
**         NegVal    - void DQ1_NegVal(void);
**         Init      - void DQ1_Init(void);
**         Deinit    - void DQ1_Deinit(void);
**
** * Copyright (c) 2015-2024, Erich Styger
**  * Web:         https://mcuoneclipse.com
**  * SourceForge: https://sourceforge.net/projects/mcuoneclipse
**  * Git:         https://github.com/ErichStyger/McuOnEclipse_PEx
**  * All rights reserved.
**  *
**  * Redistribution and use in source and binary forms, with or without modification,
**  * are permitted provided that the following conditions are met:
**  *
**  * - Redistributions of source code must retain the above copyright notice, this list
**  *   of conditions and the following disclaimer.
**  *
**  * - Redistributions in binary form must reproduce the above copyright notice, this
**  *   list of conditions and the following disclaimer in the documentation and/or
**  *   other materials provided with the distribution.
**  *
**  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**  * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**  * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**  * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**  * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**  * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**  * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
** ###################################################################*/
/*!
** @file DQ1.h
** @version 01.00
** @brief
**          GPIO component usable with NXP SDK
*/         
/*!
**  @addtogroup DQ1_module DQ1 module documentation
**  @{
*/         

/* MODULE DQ1. */

#include "DQ1.h"
#if McuLib_CONFIG_NXP_SDK_2_0_USED
  #if DQ1_CONFIG_DO_PIN_MUXING
    #if McuLib_CONFIG_CPU_IS_LPC
      #include "fsl_iocon.h" /* include SDK header file for I/O connection muxing */
    #else /* Kinetis */
      #include "fsl_port.h" /* include SDK header file for port muxing */
    #endif
  #endif
  #include "fsl_gpio.h" /* include SDK header file for GPIO */
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_KINETIS_1_3
  #include "fsl_gpio_driver.h" /* include SDK header file for GPIO */
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_S32K
  #include "pins_gpio_hw_access.h"
  #include "pins_driver.h" /* include SDK header file for GPIO */
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_NORDIC_NRF5
  #include "nrf_gpio.h"
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_RPI_PICO
  #include "McuGPIO.h"
#else
  #error "Unsupported SDK!"
#endif

#if McuLib_CONFIG_NXP_SDK_2_0_USED
  static const gpio_pin_config_t DQ1_configOutput = {
    kGPIO_DigitalOutput,  /* use as output pin */
    DQ1_CONFIG_INIT_PIN_VALUE,  /* initial value */
  };

  static const gpio_pin_config_t DQ1_configInput = {
    kGPIO_DigitalInput,  /* use as input pin */
    DQ1_CONFIG_INIT_PIN_VALUE,  /* initial value */
  };
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_KINETIS_1_3
  const gpio_output_pin_user_config_t DQ1_OutputConfig[] = {
    {
      .pinName = DQ1_CONFIG_PIN_SYMBOL,
      .config.outputLogic = DQ1_CONFIG_INIT_PIN_VALUE,
    #if FSL_FEATURE_PORT_HAS_SLEW_RATE
      .config.slewRate = kPortSlowSlewRate,
    #endif
    #if FSL_FEATURE_PORT_HAS_OPEN_DRAIN
      .config.isOpenDrainEnabled = true,
    #endif
    #if FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH
      .config.driveStrength = kPortLowDriveStrength,
    #endif
    },
    {
      .pinName = GPIO_PINS_OUT_OF_RANGE,
    }
  };

  const gpio_input_pin_user_config_t DQ1_InputConfig[] = {
    {
      .pinName = DQ1_CONFIG_PIN_SYMBOL,
    #if FSL_FEATURE_PORT_HAS_PULL_ENABLE
      #if DQ1_CONFIG_PULL_RESISTOR==0 /* 0: no pull resistor, 1: pull-up, 2: pull-down, 3: pull-up or no pull, 4: pull-down or no pull: 4: autoselect-pull */
      .config.isPullEnable = false,
      #else
      .config.isPullEnable = true,
      #endif
    #endif
    #if FSL_FEATURE_PORT_HAS_PULL_SELECTION
      #if DQ1_CONFIG_PULL_RESISTOR==1
      .config.pullSelect = kPortPullUp,
      #else
      .config.pullSelect = kPortPullDown,
      #endif
    #endif
    #if FSL_FEATURE_PORT_HAS_PASSIVE_FILTER
      .config.isPassiveFilterEnabled = true,
    #endif
    #if FSL_FEATURE_PORT_HAS_DIGITAL_FILTER
      .config.isDigitalFilterEnabled = true,
    #endif
    #if FSL_FEATURE_GPIO_HAS_INTERRUPT_VECTOR
      .config.interrupt = kPortIntDisabled
    #endif
    },
    {
      .pinName = GPIO_PINS_OUT_OF_RANGE,
    }
  };
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_RPI_PICO
  static McuGPIO_Handle_t pin;
#endif

static bool DQ1_isOutput = false;
/*
** ===================================================================
**     Method      :  ClrVal (component SDK_BitIO)
**
**     Description :
**         Clears the pin value (sets it to a low level)
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void DQ1_ClrVal(void)
{
#if McuLib_CONFIG_NXP_SDK_2_0_USED
  #if McuLib_CONFIG_CPU_IS_LPC
  GPIO_PortClear(DQ1_CONFIG_GPIO_NAME, DQ1_CONFIG_PORT_NAME, 1<<DQ1_CONFIG_PIN_NUMBER);
  #elif McuLib_CONFIG_SDK_VERSION < 250
  GPIO_ClearPinsOutput(DQ1_CONFIG_GPIO_NAME, 1<<DQ1_CONFIG_PIN_NUMBER);
  #else
  GPIO_PortClear(DQ1_CONFIG_GPIO_NAME, 1<<DQ1_CONFIG_PIN_NUMBER);
  #endif
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_KINETIS_1_3
  GPIO_DRV_ClearPinOutput(DQ1_CONFIG_PIN_SYMBOL);
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_S32K
  PINS_GPIO_WritePin(DQ1_CONFIG_PORT_NAME, DQ1_CONFIG_PIN_NUMBER, 0);
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_NORDIC_NRF5
  nrf_gpio_pin_clear(DQ1_CONFIG_PIN_NUMBER);
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_RPI_PICO
  McuGPIO_SetLow(pin);
#endif
}

/*
** ===================================================================
**     Method      :  SetVal (component SDK_BitIO)
**
**     Description :
**         Sets the pin value to a high value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void DQ1_SetVal(void)
{
#if McuLib_CONFIG_NXP_SDK_2_0_USED
  #if McuLib_CONFIG_CPU_IS_LPC
  GPIO_PortSet(DQ1_CONFIG_GPIO_NAME, DQ1_CONFIG_PORT_NAME, 1<<DQ1_CONFIG_PIN_NUMBER);
  #elif McuLib_CONFIG_SDK_VERSION < 250
  GPIO_SetPinsOutput(DQ1_CONFIG_GPIO_NAME, 1<<DQ1_CONFIG_PIN_NUMBER);
  #else
  GPIO_PortSet(DQ1_CONFIG_GPIO_NAME, 1<<DQ1_CONFIG_PIN_NUMBER);
  #endif
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_KINETIS_1_3
  GPIO_DRV_SetPinOutput(DQ1_CONFIG_PIN_SYMBOL);
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_S32K
  PINS_GPIO_WritePin(DQ1_CONFIG_PORT_NAME, DQ1_CONFIG_PIN_NUMBER, 1);
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_NORDIC_NRF5
  nrf_gpio_pin_set(DQ1_CONFIG_PIN_NUMBER);
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_RPI_PICO
  McuGPIO_SetHigh(pin);
#endif
}

/*
** ===================================================================
**     Method      :  NegVal (component SDK_BitIO)
**
**     Description :
**         Toggles/negates the pin value
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void DQ1_NegVal(void)
{
#if McuLib_CONFIG_NXP_SDK_2_0_USED
  #if McuLib_CONFIG_CPU_IS_LPC
  GPIO_PortToggle(DQ1_CONFIG_GPIO_NAME, DQ1_CONFIG_PORT_NAME, 1<<DQ1_CONFIG_PIN_NUMBER);
  #elif McuLib_CONFIG_SDK_VERSION < 250
  GPIO_TogglePinsOutput(DQ1_CONFIG_GPIO_NAME, 1<<DQ1_CONFIG_PIN_NUMBER);
  #else
  GPIO_PortToggle(DQ1_CONFIG_GPIO_NAME, 1<<DQ1_CONFIG_PIN_NUMBER);
  #endif
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_KINETIS_1_3
  GPIO_DRV_TogglePinOutput(DQ1_CONFIG_PIN_SYMBOL);
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_S32K
  pins_channel_type_t val;

  val = PINS_GPIO_GetPinsOutput(DQ1_CONFIG_PORT_NAME);
  if (val&(1<<DQ1_CONFIG_PIN_NUMBER)) {
    PINS_GPIO_WritePin(DQ1_CONFIG_PORT_NAME, DQ1_CONFIG_PIN_NUMBER, 0);
  } else {
    PINS_GPIO_WritePin(DQ1_CONFIG_PORT_NAME, DQ1_CONFIG_PIN_NUMBER, 1);
  }
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_NORDIC_NRF5
  nrf_gpio_pin_toggle(DQ1_CONFIG_PIN_NUMBER);
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_RPI_PICO
  McuGPIO_Toggle(pin);
#endif
}

/*
** ===================================================================
**     Method      :  GetVal (component SDK_BitIO)
**
**     Description :
**         Returns the pin value
**     Parameters  : None
**     Returns     :
**         ---             - Returns the value of the pin:
**                           FALSE/logical level '0' or TRUE/logical
**                           level '1'
** ===================================================================
*/
bool DQ1_GetVal(void)
{
#if McuLib_CONFIG_CPU_IS_LPC
  return GPIO_PinRead(DQ1_CONFIG_GPIO_NAME, DQ1_CONFIG_PORT_NAME, DQ1_CONFIG_PIN_NUMBER);
#elif McuLib_CONFIG_NXP_SDK_2_0_USED
  #if McuLib_CONFIG_SDK_VERSION < 250
  return GPIO_ReadPinInput(DQ1_CONFIG_GPIO_NAME, DQ1_CONFIG_PIN_NUMBER)!=0;
  #else
  return GPIO_PinRead(DQ1_CONFIG_GPIO_NAME, DQ1_CONFIG_PIN_NUMBER)!=0;
  #endif
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_KINETIS_1_3
  return GPIO_DRV_ReadPinInput(DQ1_CONFIG_PIN_SYMBOL)!=0;
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_S32K
  return (PINS_DRV_ReadPins(DQ1_CONFIG_PORT_NAME)&(1<<DQ1_CONFIG_PIN_NUMBER))!=0;
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_NORDIC_NRF5
  return nrf_gpio_pin_read(DQ1_CONFIG_PIN_NUMBER)!=0;
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_RPI_PICO
  return McuGPIO_GetValue(pin);
#else
  return FALSE;
#endif
}

/*
** ===================================================================
**     Method      :  GetDir (component SDK_BitIO)
**
**     Description :
**         Return the direction of the pin (input/output)
**     Parameters  : None
**     Returns     :
**         ---             - FALSE if port is input, TRUE if port is
**                           output
** ===================================================================
*/
bool DQ1_GetDir(void)
{
  return DQ1_isOutput;
}

/*
** ===================================================================
**     Method      :  SetDir (component SDK_BitIO)
**
**     Description :
**         Sets the direction of the pin (input or output)
**     Parameters  :
**         NAME            - DESCRIPTION
**         Dir             - FALSE: input, TRUE: output
**     Returns     : Nothing
** ===================================================================
*/
void DQ1_SetDir(bool Dir)
{
  if (Dir) {
    DQ1_SetOutput();
  } else {
    DQ1_SetInput();
  }
}

/*
** ===================================================================
**     Method      :  SetInput (component SDK_BitIO)
**
**     Description :
**         Sets the pin as input
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void DQ1_SetInput(void)
{
#if McuLib_CONFIG_CPU_IS_LPC
  GPIO_PinInit(DQ1_CONFIG_GPIO_NAME, DQ1_CONFIG_PORT_NAME, DQ1_CONFIG_PIN_NUMBER, &DQ1_configInput);
#elif McuLib_CONFIG_NXP_SDK_2_0_USED
  GPIO_PinInit(DQ1_CONFIG_GPIO_NAME, DQ1_CONFIG_PIN_NUMBER, &DQ1_configInput);
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_KINETIS_1_3
  GPIO_DRV_SetPinDir(DQ1_CONFIG_PIN_SYMBOL, kGpioDigitalInput);
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_S32K
  pins_channel_type_t val;

  val = PINS_GPIO_GetPinsDirection(DQ1_CONFIG_PORT_NAME); /* bit 0: pin is input; 1: pin is output */
  val &= ~(1<<DQ1_CONFIG_PIN_NUMBER); /* clear bit ==> input */
  PINS_DRV_SetPinsDirection(DQ1_CONFIG_PORT_NAME, val);
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_NORDIC_NRF5
  nrf_gpio_cfg_input(DQ1_CONFIG_PIN_NUMBER, NRF_GPIO_PIN_NOPULL);
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_RPI_PICO
  McuGPIO_SetAsInput(pin);
#endif
  DQ1_isOutput = false;
}

/*
** ===================================================================
**     Method      :  SetOutput (component SDK_BitIO)
**
**     Description :
**         Sets the pin as output
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void DQ1_SetOutput(void)
{
#if McuLib_CONFIG_CPU_IS_LPC
  GPIO_PinInit(DQ1_CONFIG_GPIO_NAME, DQ1_CONFIG_PORT_NAME, DQ1_CONFIG_PIN_NUMBER, &DQ1_configOutput);
#elif McuLib_CONFIG_NXP_SDK_2_0_USED
  GPIO_PinInit(DQ1_CONFIG_GPIO_NAME, DQ1_CONFIG_PIN_NUMBER, &DQ1_configOutput);
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_KINETIS_1_3
  GPIO_DRV_SetPinDir(DQ1_CONFIG_PIN_SYMBOL, kGpioDigitalOutput);
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_S32K
  pins_channel_type_t val;

  val = PINS_GPIO_GetPinsDirection(DQ1_CONFIG_PORT_NAME); /* bit 0: pin is input; 1: pin is output */
  val |= (1<<DQ1_CONFIG_PIN_NUMBER); /* set bit ==> output */
  PINS_DRV_SetPinsDirection(DQ1_CONFIG_PORT_NAME, val);
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_NORDIC_NRF5
  nrf_gpio_cfg_output(DQ1_CONFIG_PIN_NUMBER);
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_RPI_PICO
  McuGPIO_SetAsOutput(pin, false /* don't care */);
#endif
  DQ1_isOutput = true;
}

/*
** ===================================================================
**     Method      :  PutVal (component SDK_BitIO)
**
**     Description :
**         Sets the pin value
**     Parameters  :
**         NAME            - DESCRIPTION
**         Val             - Value to set. FALSE/logical '0' or
**                           TRUE/logical '1'
**     Returns     : Nothing
** ===================================================================
*/
void DQ1_PutVal(bool Val)
{
#if McuLib_CONFIG_CPU_IS_LPC
  if (Val) {
    GPIO_PortSet(DQ1_CONFIG_GPIO_NAME, DQ1_CONFIG_PORT_NAME, 1<<DQ1_CONFIG_PIN_NUMBER);
  } else {
    GPIO_PortClear(DQ1_CONFIG_GPIO_NAME, DQ1_CONFIG_PORT_NAME, 1<<DQ1_CONFIG_PIN_NUMBER);
  }
#elif McuLib_CONFIG_NXP_SDK_2_0_USED
  #if McuLib_CONFIG_SDK_VERSION < 250
  if (Val) {
    GPIO_SetPinsOutput(DQ1_CONFIG_GPIO_NAME, 1<<DQ1_CONFIG_PIN_NUMBER);
  } else {
    GPIO_ClearPinsOutput(DQ1_CONFIG_GPIO_NAME, 1<<DQ1_CONFIG_PIN_NUMBER);
  }
  #else
  if (Val) {
    GPIO_PortSet(DQ1_CONFIG_GPIO_NAME, 1<<DQ1_CONFIG_PIN_NUMBER);
  } else {
    GPIO_PortClear(DQ1_CONFIG_GPIO_NAME, 1<<DQ1_CONFIG_PIN_NUMBER);
  }
  #endif
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_KINETIS_1_3
  GPIO_DRV_WritePinOutput(DQ1_CONFIG_PIN_SYMBOL, Val);
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_S32K
  PINS_DRV_WritePin(DQ1_CONFIG_PORT_NAME, DQ1_CONFIG_PIN_NUMBER, Val);
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_NORDIC_NRF5
  /* NYI */
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_RPI_PICO
  McuGPIO_SetValue(pin, Val);
#endif
}

/*
** ===================================================================
**     Method      :  Init (component SDK_BitIO)
**
**     Description :
**         Driver initialization method
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void DQ1_Init(void)
{
#if McuLib_CONFIG_NXP_SDK_2_0_USED
  #if DQ1_CONFIG_DO_PIN_MUXING
      #if McuLib_CONFIG_CPU_IS_LPC
        #define _IOCON_PIO_DIGITAL_EN 0x0100u  /*!<@brief Enables digital function */
        #define _IOCON_PIO_FUNC0 0x00u         /*!<@brief Selects pin function 0 */
        #define _IOCON_PIO_INV_DI 0x00u        /*!<@brief Input function is not inverted */
        #define _IOCON_PIO_MODE_PULLUP 0x10u   /*!<@brief Selects pull-up function */
        #define _IOCON_PIO_OPENDRAIN_DI 0x00u  /*!<@brief Open drain is disabled */
        #define _IOCON_PIO_SLEW_STANDARD 0x00u /*!<@brief Standard mode, output slew rate control is enabled */

        const uint32_t port_pin_config = (/* Pin is configured as PI<portname>_<pinnumber> */
                                      _IOCON_PIO_FUNC0 |
                                      /* Selects pull-up function */
                                      _IOCON_PIO_MODE_PULLUP |
                                      /* Standard mode, output slew rate control is enabled */
                                      _IOCON_PIO_SLEW_STANDARD |
                                      /* Input function is not inverted */
                                      _IOCON_PIO_INV_DI |
                                      /* Enables digital function */
                                      _IOCON_PIO_DIGITAL_EN |
                                      /* Open drain is disabled */
                                      _IOCON_PIO_OPENDRAIN_DI);
        #if (McuLib_CONFIG_CPU_IS_LPC && McuLib_CONFIG_CORTEX_M==0)
          IOCON_PinMuxSet(DQ1_CONFIG_PORT_NAME, DQ1_CONFIG_PIN_NUMBER, port_pin_config);
        #else
          IOCON_PinMuxSet(IOCON, DQ1_CONFIG_PORT_NAME, DQ1_CONFIG_PIN_NUMBER, port_pin_config);
        #endif
      #else
        PORT_SetPinMux(DQ1_CONFIG_PORT_NAME, DQ1_CONFIG_PIN_NUMBER, kPORT_MuxAsGpio); /* mux as GPIO */
      #endif
  #endif
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_KINETIS_1_3
  /*! Pin Muxing not implemented */
  GPIO_DRV_Init(DQ1_InputConfig, DQ1_OutputConfig);
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_S32K
  /* the following needs to be called in the application first:
  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
  */
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_NORDIC_NRF5
  /* nothing needed */
#elif McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_RPI_PICO
  McuGPIO_Config_t config;

  McuGPIO_GetDefaultConfig(&config);
  config.hw.pin = DQ1_CONFIG_PIN_NUMBER;
  config.isInput = true;
  pin = McuGPIO_InitGPIO(&config);
#endif
#if DQ1_CONFIG_INIT_PIN_DIRECTION == DQ1_CONFIG_INIT_PIN_DIRECTION_INPUT
  DQ1_SetInput();
#elif DQ1_CONFIG_INIT_PIN_DIRECTION == DQ1_CONFIG_INIT_PIN_DIRECTION_OUTPUT
  DQ1_SetOutput();
#endif
}

/*
** ===================================================================
**     Method      :  Deinit (component SDK_BitIO)
**
**     Description :
**         Driver de-initialization method
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void DQ1_Deinit(void)
{
#if McuLib_CONFIG_SDK_VERSION_USED == McuLib_CONFIG_SDK_RPI_PICO
  pin = McuGPIO_DeinitGPIO(pin);
#endif
}

/* END DQ1. */

/*!
** @}
*/
