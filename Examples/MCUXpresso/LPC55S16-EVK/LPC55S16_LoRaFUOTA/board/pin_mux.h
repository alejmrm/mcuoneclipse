/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

#define IOCON_PIO_DIGITAL_EN 0x0100u  /*!<@brief Enables digital function */
#define IOCON_PIO_FUNC1 0x01u         /*!<@brief Selects pin function 1 */
#define IOCON_PIO_INV_DI 0x00u        /*!<@brief Input function is not inverted */
#define IOCON_PIO_MODE_INACT 0x00u    /*!<@brief No addition pin function */
#define IOCON_PIO_OPENDRAIN_DI 0x00u  /*!<@brief Open drain is disabled */
#define IOCON_PIO_SLEW_STANDARD 0x00u /*!<@brief Standard mode, output slew rate control is enabled */

/*! @name FC0_RXD_SDA_MOSI_DATA (number 92), U11[14]/U22[14]/FC0_USART_RXD
  @{ */
/* @} */

/*! @name FC0_TXD_SCL_MISO_WS (number 94), U11[13]/U22[13]/FC0_USART_TXD
  @{ */
/* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitDEBUG_UARTPins(void); /* Function assigned for the Cortex-M33 */

#define IOCON_PIO_ASW_DI 0x00u        /*!<@brief Analog switch is open (disabled) */
#define IOCON_PIO_DIGITAL_EN 0x0100u  /*!<@brief Enables digital function */
#define IOCON_PIO_FUNC6 0x06u         /*!<@brief Selects pin function 6 */
#define IOCON_PIO_INV_DI 0x00u        /*!<@brief Input function is not inverted */
#define IOCON_PIO_MODE_INACT 0x00u    /*!<@brief No addition pin function */
#define IOCON_PIO_MODE_PULLDOWN 0x10u /*!<@brief Selects pull-down function */
#define IOCON_PIO_MODE_PULLUP 0x20u   /*!<@brief Selects pull-up function */
#define IOCON_PIO_OPENDRAIN_DI 0x00u  /*!<@brief Open drain is disabled */
#define IOCON_PIO_SLEW_STANDARD 0x00u /*!<@brief Standard mode, output slew rate control is enabled */

/*! @name SWCLK (number 13), J10[8]/U18[4]/N4M_SWDCLK
  @{ */
/* @} */

/*! @name SWDIO (number 12), J10[6]/U18[8]/N4M_SWDIO
  @{ */
/* @} */

/*! @name SWO (number 21), J9[1]/U18[12]/N4M_SWO
  @{ */
/* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitSWD_DEBUGPins(void); /* Function assigned for the Cortex-M33 */

#define IOCON_PIO_DIGITAL_EN 0x0100u  /*!<@brief Enables digital function */
#define IOCON_PIO_FUNC7 0x07u         /*!<@brief Selects pin function 7 */
#define IOCON_PIO_INV_DI 0x00u        /*!<@brief Input function is not inverted */
#define IOCON_PIO_MODE_INACT 0x00u    /*!<@brief No addition pin function */
#define IOCON_PIO_OPENDRAIN_DI 0x00u  /*!<@brief Open drain is disabled */
#define IOCON_PIO_SLEW_STANDARD 0x00u /*!<@brief Standard mode, output slew rate control is enabled */

/*! @name USB0_DM (number 98), J3[2]/FS_USB0_DN
  @{ */
/* @} */

/*! @name USB0_DP (number 97), J3[3]/FS_USB0_DP
  @{ */
/* @} */

/*! @name USB0_VBUS (number 78), USB0_VBUS
  @{ */
/* @} */

/*! @name USB1_DM (number 35), J4[2]/HS_USB1_DN
  @{ */
/* @} */

/*! @name USB1_DP (number 34), J4[3]/HS_USB1_DP
  @{ */
/* @} */

/*! @name USB1_VBUS (number 36), HS_USB_5V0/USB1_VBUS
  @{ */
/* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitUSBPins(void); /* Function assigned for the Cortex-M33 */

#define IOCON_PIO_DIGITAL_EN 0x0100u  /*!<@brief Enables digital function */
#define IOCON_PIO_FUNC0 0x00u         /*!<@brief Selects pin function 0 */
#define IOCON_PIO_INV_DI 0x00u        /*!<@brief Input function is not inverted */
#define IOCON_PIO_MODE_PULLUP 0x20u   /*!<@brief Selects pull-up function */
#define IOCON_PIO_OPENDRAIN_DI 0x00u  /*!<@brief Open drain is disabled */
#define IOCON_PIO_SLEW_STANDARD 0x00u /*!<@brief Standard mode, output slew rate control is enabled */

/*! @name PIO1_4 (number 1), Q5[G1]/J12[6]/ARD_LEDR_PWM
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITLEDSPINS_LED_RED_GPIO GPIO                /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITLEDSPINS_LED_RED_GPIO_PIN_MASK (1U << 4U) /*!<@brief GPIO pin mask */
#define BOARD_INITLEDSPINS_LED_RED_PORT 1U                  /*!<@brief PORT peripheral base pointer */
#define BOARD_INITLEDSPINS_LED_RED_PIN 4U                   /*!<@brief PORT pin number */
#define BOARD_INITLEDSPINS_LED_RED_PIN_MASK (1U << 4U)      /*!<@brief PORT pin mask */
                                                            /* @} */

/*! @name PIO1_6 (number 5), Q4[G1]/J12[10]/ARD_LEDB_PWM
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITLEDSPINS_LED_BLUE_GPIO GPIO                /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITLEDSPINS_LED_BLUE_GPIO_PIN_MASK (1U << 6U) /*!<@brief GPIO pin mask */
#define BOARD_INITLEDSPINS_LED_BLUE_PORT 1U                  /*!<@brief PORT peripheral base pointer */
#define BOARD_INITLEDSPINS_LED_BLUE_PIN 6U                   /*!<@brief PORT pin number */
#define BOARD_INITLEDSPINS_LED_BLUE_PIN_MASK (1U << 6U)      /*!<@brief PORT pin mask */
                                                             /* @} */

/*! @name PIO1_7 (number 9), Q4[G2]/J12[8]/ARD_LEDG_PWM
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITLEDSPINS_LED_GREEN_GPIO GPIO                /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITLEDSPINS_LED_GREEN_GPIO_PIN_MASK (1U << 7U) /*!<@brief GPIO pin mask */
#define BOARD_INITLEDSPINS_LED_GREEN_PORT 1U                  /*!<@brief PORT peripheral base pointer */
#define BOARD_INITLEDSPINS_LED_GREEN_PIN 7U                   /*!<@brief PORT pin number */
#define BOARD_INITLEDSPINS_LED_GREEN_PIN_MASK (1U << 7U)      /*!<@brief PORT pin mask */
                                                              /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLEDsPins(void); /* Function assigned for the Cortex-M33 */

#define IOCON_PIO_ASW_DI 0x00u        /*!<@brief Analog switch is open (disabled) */
#define IOCON_PIO_DIGITAL_EN 0x0100u  /*!<@brief Enables digital function */
#define IOCON_PIO_FUNC0 0x00u         /*!<@brief Selects pin function 0 */
#define IOCON_PIO_INV_DI 0x00u        /*!<@brief Input function is not inverted */
#define IOCON_PIO_MODE_PULLUP 0x20u   /*!<@brief Selects pull-up function */
#define IOCON_PIO_OPENDRAIN_DI 0x00u  /*!<@brief Open drain is disabled */
#define IOCON_PIO_SLEW_STANDARD 0x00u /*!<@brief Standard mode, output slew rate control is enabled */

/*! @name PIO1_18 (number 64), SW1/J8[2]/J12[15]/MIK_EXP_BTN_WK
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITBUTTONSPINS_WAKEUP_GPIO GPIO                 /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITBUTTONSPINS_WAKEUP_GPIO_PIN_MASK (1U << 18U) /*!<@brief GPIO pin mask */
#define BOARD_INITBUTTONSPINS_WAKEUP_PORT 1U                   /*!<@brief PORT peripheral base pointer */
#define BOARD_INITBUTTONSPINS_WAKEUP_PIN 18U                   /*!<@brief PORT pin number */
#define BOARD_INITBUTTONSPINS_WAKEUP_PIN_MASK (1U << 18U)      /*!<@brief PORT pin mask */
                                                               /* @} */

/*! @name PIO1_9 (number 10), SW3/J12[2]/ARD_BTN_USR_P1_9
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITBUTTONSPINS_USR_GPIO GPIO                /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITBUTTONSPINS_USR_GPIO_PIN_MASK (1U << 9U) /*!<@brief GPIO pin mask */
#define BOARD_INITBUTTONSPINS_USR_PORT 1U                  /*!<@brief PORT peripheral base pointer */
#define BOARD_INITBUTTONSPINS_USR_PIN 9U                   /*!<@brief PORT pin number */
#define BOARD_INITBUTTONSPINS_USR_PIN_MASK (1U << 9U)      /*!<@brief PORT pin mask */
                                                           /* @} */

/*! @name RESETN (number 32), SW2/J7[2]/J10[9]/J10[10]/U18[16]/N4M_RESET#
  @{ */
/* @} */

/*! @name PIO0_5 (number 88), J9[7]/J17[7]/J18[2]/SW4/JS3[1]/U22[11]/N4M_ISP_MODE
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITBUTTONSPINS_ISP_GPIO GPIO                /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITBUTTONSPINS_ISP_GPIO_PIN_MASK (1U << 5U) /*!<@brief GPIO pin mask */
#define BOARD_INITBUTTONSPINS_ISP_PORT 0U                  /*!<@brief PORT peripheral base pointer */
#define BOARD_INITBUTTONSPINS_ISP_PIN 5U                   /*!<@brief PORT pin number */
#define BOARD_INITBUTTONSPINS_ISP_PIN_MASK (1U << 5U)      /*!<@brief PORT pin mask */
                                                           /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitBUTTONsPins(void); /* Function assigned for the Cortex-M33 */

#define IOCON_PIO_DIGITAL_EN 0x0100u  /*!<@brief Enables digital function */
#define IOCON_PIO_FUNC0 0x00u         /*!<@brief Selects pin function 0 */
#define IOCON_PIO_FUNC1 0x01u         /*!<@brief Selects pin function 1 */
#define IOCON_PIO_FUNC2 0x02u         /*!<@brief Selects pin function 2 */
#define IOCON_PIO_FUNC5 0x05u         /*!<@brief Selects pin function 5 */
#define IOCON_PIO_FUNC7 0x07u         /*!<@brief Selects pin function 7 */
#define IOCON_PIO_INV_DI 0x00u        /*!<@brief Input function is not inverted */
#define IOCON_PIO_MODE_INACT 0x00u    /*!<@brief No addition pin function */
#define IOCON_PIO_MODE_PULLUP 0x20u   /*!<@brief Selects pull-up function */
#define IOCON_PIO_OPENDRAIN_DI 0x00u  /*!<@brief Open drain is disabled */
#define IOCON_PIO_SLEW_STANDARD 0x00u /*!<@brief Standard mode, output slew rate control is enabled */
#define MCLKIO_MCLKIO_INPUT 0x00u     /*!<@brief MCLK control.: input mode. */

/*! @name FC4_RXD_SDA_MOSI_DATA (number 30), U12[6]/U13[3]/J8[6]/J9[4]/FC4_I2C_SDA
  @{ */
/* @} */

/*! @name FC4_TXD_SCL_MISO_WS (number 4), U12[4]/U13[2]/J8[5]/J9[2]/FC4_I2C_SCL
  @{ */
/* @} */

/*! @name FC7_RXD_SDA_MOSI_DATA (number 74), J19[2]/J9[9]/FC7_I2S_TX
  @{ */
/* @} */

/*! @name FC7_TXD_SCL_MISO_WS (number 90), U13[30]/J9[11]/FC7_I2S_WS
  @{ */
/* @} */

/*! @name FC7_SCK (number 76), U13[29]/J9[13]/FC7_I2S_BCLK
  @{ */
/* @} */

/*! @name FC6_RXD_SDA_MOSI_DATA (number 2), J18[2]/J9[19]/FC6_I2S_RX
  @{ */
/* @} */

/*! @name PIO1_25 (number 77), J9[15]/J12[5]/I2S_IRQ
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITI2SPINS_I2S_IRQ_GPIO GPIO                 /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITI2SPINS_I2S_IRQ_GPIO_PIN_MASK (1U << 25U) /*!<@brief GPIO pin mask */
#define BOARD_INITI2SPINS_I2S_IRQ_PORT 1U                   /*!<@brief PORT peripheral base pointer */
#define BOARD_INITI2SPINS_I2S_IRQ_PIN 25U                   /*!<@brief PORT pin number */
#define BOARD_INITI2SPINS_I2S_IRQ_PIN_MASK (1U << 25U)      /*!<@brief PORT pin mask */
                                                            /* @} */

/*! @name MCLK (number 91), J13[7]/J13[8]/U13[28]/I2S_MCLK
  @{ */
/* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitI2SPins(void); /* Function assigned for the Cortex-M33 */

#define IOCON_PIO_DIGITAL_EN 0x0100u  /*!<@brief Enables digital function */
#define IOCON_PIO_FUNC0 0x00u         /*!<@brief Selects pin function 0 */
#define IOCON_PIO_FUNC5 0x05u         /*!<@brief Selects pin function 5 */
#define IOCON_PIO_INV_DI 0x00u        /*!<@brief Input function is not inverted */
#define IOCON_PIO_MODE_INACT 0x00u    /*!<@brief No addition pin function */
#define IOCON_PIO_MODE_PULLUP 0x20u   /*!<@brief Selects pull-up function */
#define IOCON_PIO_OPENDRAIN_DI 0x00u  /*!<@brief Open drain is disabled */
#define IOCON_PIO_SLEW_STANDARD 0x00u /*!<@brief Standard mode, output slew rate control is enabled */

/*! @name FC4_RXD_SDA_MOSI_DATA (number 30), U12[6]/U13[3]/J8[6]/J9[4]/FC4_I2C_SDA
  @{ */
/* @} */

/*! @name FC4_TXD_SCL_MISO_WS (number 4), U12[4]/U13[2]/J8[5]/J9[2]/FC4_I2C_SCL
  @{ */
/* @} */

/*! @name PIO1_26 (number 68), U12[11]/J12[13]/J12[17]/ACC_INT_EXP_P1_26
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITACCELPINS_ACCL_INTR_GPIO GPIO                 /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITACCELPINS_ACCL_INTR_GPIO_PIN_MASK (1U << 26U) /*!<@brief GPIO pin mask */
#define BOARD_INITACCELPINS_ACCL_INTR_PORT 1U                   /*!<@brief PORT peripheral base pointer */
#define BOARD_INITACCELPINS_ACCL_INTR_PIN 26U                   /*!<@brief PORT pin number */
#define BOARD_INITACCELPINS_ACCL_INTR_PIN_MASK (1U << 26U)      /*!<@brief PORT pin mask */
                                                                /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitACCELPins(void); /* Function assigned for the Cortex-M33 */

#define IOCON_PIO_DIGITAL_EN 0x0100u  /*!<@brief Enables digital function */
#define IOCON_PIO_FUNC9 0x09u         /*!<@brief Selects pin function 9 */
#define IOCON_PIO_INV_DI 0x00u        /*!<@brief Input function is not inverted */
#define IOCON_PIO_MODE_INACT 0x00u    /*!<@brief No addition pin function */
#define IOCON_PIO_OPENDRAIN_DI 0x00u  /*!<@brief Open drain is disabled */
#define IOCON_PIO_SLEW_STANDARD 0x00u /*!<@brief Standard mode, output slew rate control is enabled */

/*! @name CAN0_TD (number 85), U26[3]/CAN_TXD
  @{ */
/* @} */

/*! @name CAN0_RD (number 41), U25[3]/CAN_RXD
  @{ */
/* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitCANPins(void); /* Function assigned for the Cortex-M33 */

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
