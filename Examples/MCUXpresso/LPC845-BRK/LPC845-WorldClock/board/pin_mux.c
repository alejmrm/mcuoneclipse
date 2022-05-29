/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v11.0
processor: LPC845
package_id: LPC845M301JBD48
mcu_data: ksdk2_0
processor_version: 11.0.1
board: LPC845BREAKOUT
pin_labels:
- {pin_num: '11', pin_signal: PIO1_0/CAPT_X1, label: M2_STEP, identifier: M2_STEP}
- {pin_num: '14', pin_signal: PIO1_1/CAPT_X2, label: MAG_M0, identifier: MAG_M0}
- {pin_num: '16', pin_signal: PIO1_2/CAPT_X3, label: MAG_M6, identifier: MAG_M6}
- {pin_num: '1', pin_signal: PIO1_8/CAPT_YL, label: M1_STEP, identifier: CAPY_LOW;M3_STEP;M1_STEP}
- {pin_num: '2', pin_signal: PIO0_13/ADC_10, label: M1_DIR, identifier: M3_DIR;M1_DIR}
- {pin_num: '3', pin_signal: PIO1_9/CAPT_YH, label: M0_DIR, identifier: CAPY_HIGH;M0_DIR}
- {pin_num: '4', pin_signal: PIO0_12, label: M0_STEP, identifier: K1;M0_STEP}
- {pin_num: '6', pin_signal: PIO0_4/ADC_11, label: M2_DIR, identifier: K3;M1_DIR;M2_DIR}
- {pin_num: '7', pin_signal: PIO0_28/WKTCLKIN, label: M2_STEP, identifier: M1_STEP;M2_STEP}
- {pin_num: '9', pin_signal: PIO0_31/CAPT_X0, label: MAG_M1, identifier: CAPX;MAG_M1}
- {pin_num: '12', pin_signal: PIO0_11/I2C0_SDA, label: M2_DIR, identifier: M2_DIR}
- {pin_num: '13', pin_signal: PIO0_10/I2C0_SCL}
- {pin_num: '15', pin_signal: PIO0_16, label: MAG_M7, identifier: MAG_M7}
- {pin_num: '17', pin_signal: PIO0_27, label: M3_DIR, identifier: M3_DIR;M2_DIR}
- {pin_num: '18', pin_signal: PIO0_26, label: M3_STEP, identifier: M3_STEP}
- {pin_num: '23', pin_signal: PIO1_4/CAPT_X5, label: PIO1_4/CAPT_X5, identifier: RS485_TXRX_EN}
- {pin_num: '25', pin_signal: PIO0_9/XTALOUT, label: M6_STEP, identifier: M5_STEP;M6_STEP}
- {pin_num: '26', pin_signal: PIO0_8/XTALIN, label: M6_DIR, identifier: M5_DIR;M6_DIR}
- {pin_num: '27', pin_signal: PIO1_5/CAPT_X6, label: M7_DIR, identifier: M6_DIR;M7_DIR}
- {pin_num: '28', pin_signal: PIO1_6/CAPT_X7, label: M7_STEP, identifier: M6_STEP;M7_STEP}
- {pin_num: '33', pin_signal: PIO0_7/ADC_0, label: M5_STEP, identifier: M7_STEP;M5_STEP}
- {pin_num: '34', pin_signal: PIO0_6/ADC_1/ACMPVREF, label: M5_DIR, identifier: M7_DIR;M5_DIR}
- {pin_num: '35', pin_signal: PIO1_7/CAPT_X8, label: M4_STEP, identifier: M4_STEP}
- {pin_num: '36', pin_signal: PIO0_0/ACMP_I1, label: M4_DIR, identifier: M4_DIR}
- {pin_num: '38', pin_signal: PIO0_29/DACOUT_1, label: MAG_M5, identifier: MAG_M5}
- {pin_num: '39', pin_signal: PIO0_23/ADC_3/ACMP_I4, label: MAG_M4, identifier: MAG_M4}
- {pin_num: '47', pin_signal: PIO0_18/ADC_8, label: MAG_M3, identifier: MAG_M3}
- {pin_num: '48', pin_signal: PIO0_17/ADC_9/DACOUT_0, label: MAG_M2, identifier: MAG_M2}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_iocon.h"
#include "fsl_swm.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitSWD_DEBUGPins();
    BOARD_InitRS485();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitSWD_DEBUGPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '8', peripheral: SWD, signal: SWCLK, pin_signal: SWCLK/PIO0_3, mode: pullUp, invert: disabled, hysteresis: enabled, opendrain: disabled, smode: bypass,
    clkdiv: div0}
  - {pin_num: '10', peripheral: SWD, signal: SWDIO, pin_signal: SWDIO/PIO0_2, mode: pullUp, invert: disabled, hysteresis: enabled, opendrain: disabled, smode: bypass,
    clkdiv: div0}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitSWD_DEBUGPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M0P */
void BOARD_InitSWD_DEBUGPins(void)
{
    /* Enables clock for IOCON.: enable */
    CLOCK_EnableClock(kCLOCK_Iocon);
    /* Enables clock for switch matrix.: enable */
    CLOCK_EnableClock(kCLOCK_Swm);

    const uint32_t IOCON_INDEX_PIO0_2_config = (/* Selects pull-up function */
                                                IOCON_PIO_MODE_PULLUP |
                                                /* Enable hysteresis */
                                                IOCON_PIO_HYS_EN |
                                                /* Input not invert */
                                                IOCON_PIO_INV_DI |
                                                /* Disables Open-drain function */
                                                IOCON_PIO_OD_DI |
                                                /* Bypass input filter */
                                                IOCON_PIO_SMODE_BYPASS |
                                                /* IOCONCLKDIV0 */
                                                IOCON_PIO_CLKDIV0);
    /* PIO0 PIN2 (coords: 10) is configured as SWD, SWDIO. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_2, IOCON_INDEX_PIO0_2_config);

    const uint32_t IOCON_INDEX_PIO0_3_config = (/* Selects pull-up function */
                                                IOCON_PIO_MODE_PULLUP |
                                                /* Enable hysteresis */
                                                IOCON_PIO_HYS_EN |
                                                /* Input not invert */
                                                IOCON_PIO_INV_DI |
                                                /* Disables Open-drain function */
                                                IOCON_PIO_OD_DI |
                                                /* Bypass input filter */
                                                IOCON_PIO_SMODE_BYPASS |
                                                /* IOCONCLKDIV0 */
                                                IOCON_PIO_CLKDIV0);
    /* PIO0 PIN3 (coords: 8) is configured as SWD, SWCLK. */
    IOCON_PinMuxSet(IOCON, IOCON_INDEX_PIO0_3, IOCON_INDEX_PIO0_3_config);

    /* SWCLK connect to P0_3 */
    SWM_SetFixedPinSelect(SWM0, kSWM_SWCLK, true);

    /* SWDIO connect to P0_2 */
    SWM_SetFixedPinSelect(SWM0, kSWM_SWDIO, true);

    /* Disable clock for switch matrix. */
    CLOCK_DisableClock(kCLOCK_Swm);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitMagPins:
- options: {callFromInitBoot: 'false', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '14', peripheral: GPIO, signal: 'PIO1, 1', pin_signal: PIO1_1/CAPT_X2, direction: INPUT}
  - {pin_num: '48', peripheral: GPIO, signal: 'PIO0, 17', pin_signal: PIO0_17/ADC_9/DACOUT_0, direction: INPUT}
  - {pin_num: '47', peripheral: GPIO, signal: 'PIO0, 18', pin_signal: PIO0_18/ADC_8, direction: INPUT}
  - {pin_num: '39', peripheral: GPIO, signal: 'PIO0, 23', pin_signal: PIO0_23/ADC_3/ACMP_I4, direction: INPUT}
  - {pin_num: '38', peripheral: GPIO, signal: 'PIO0, 29', pin_signal: PIO0_29/DACOUT_1, direction: INPUT}
  - {pin_num: '16', peripheral: GPIO, signal: 'PIO1, 2', pin_signal: PIO1_2/CAPT_X3, direction: INPUT}
  - {pin_num: '15', peripheral: GPIO, signal: 'PIO0, 16', pin_signal: PIO0_16, direction: INPUT}
  - {pin_num: '9', peripheral: GPIO, signal: 'PIO0, 31', pin_signal: PIO0_31/CAPT_X0, identifier: MAG_M1, direction: INPUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitMagPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M0P */
void BOARD_InitMagPins(void)
{
    /* Enables the clock for the GPIO0 module */
    CLOCK_EnableClock(kCLOCK_Gpio0);
    /* Enables the clock for the GPIO1 module */
    CLOCK_EnableClock(kCLOCK_Gpio1);

    gpio_pin_config_t MAG_M7_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO0_16 (pin 15)  */
    GPIO_PinInit(BOARD_INITMAGPINS_MAG_M7_GPIO, BOARD_INITMAGPINS_MAG_M7_PORT, BOARD_INITMAGPINS_MAG_M7_PIN, &MAG_M7_config);

    gpio_pin_config_t MAG_M2_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO0_17 (pin 48)  */
    GPIO_PinInit(BOARD_INITMAGPINS_MAG_M2_GPIO, BOARD_INITMAGPINS_MAG_M2_PORT, BOARD_INITMAGPINS_MAG_M2_PIN, &MAG_M2_config);

    gpio_pin_config_t MAG_M3_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO0_18 (pin 47)  */
    GPIO_PinInit(BOARD_INITMAGPINS_MAG_M3_GPIO, BOARD_INITMAGPINS_MAG_M3_PORT, BOARD_INITMAGPINS_MAG_M3_PIN, &MAG_M3_config);

    gpio_pin_config_t MAG_M4_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO0_23 (pin 39)  */
    GPIO_PinInit(BOARD_INITMAGPINS_MAG_M4_GPIO, BOARD_INITMAGPINS_MAG_M4_PORT, BOARD_INITMAGPINS_MAG_M4_PIN, &MAG_M4_config);

    gpio_pin_config_t MAG_M5_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO0_29 (pin 38)  */
    GPIO_PinInit(BOARD_INITMAGPINS_MAG_M5_GPIO, BOARD_INITMAGPINS_MAG_M5_PORT, BOARD_INITMAGPINS_MAG_M5_PIN, &MAG_M5_config);

    gpio_pin_config_t MAG_M1_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO0_31 (pin 9)  */
    GPIO_PinInit(BOARD_INITMAGPINS_MAG_M1_GPIO, BOARD_INITMAGPINS_MAG_M1_PORT, BOARD_INITMAGPINS_MAG_M1_PIN, &MAG_M1_config);

    gpio_pin_config_t MAG_M0_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO1_1 (pin 14)  */
    GPIO_PinInit(BOARD_INITMAGPINS_MAG_M0_GPIO, BOARD_INITMAGPINS_MAG_M0_PORT, BOARD_INITMAGPINS_MAG_M0_PIN, &MAG_M0_config);

    gpio_pin_config_t MAG_M6_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO1_2 (pin 16)  */
    GPIO_PinInit(BOARD_INITMAGPINS_MAG_M6_GPIO, BOARD_INITMAGPINS_MAG_M6_PORT, BOARD_INITMAGPINS_MAG_M6_PIN, &MAG_M6_config);

    /* Disable clock for switch matrix. */
    CLOCK_DisableClock(kCLOCK_Swm);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitMotorPins:
- options: {callFromInitBoot: 'false', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '3', peripheral: GPIO, signal: 'PIO1, 9', pin_signal: PIO1_9/CAPT_YH, identifier: M0_DIR, direction: OUTPUT}
  - {pin_num: '1', peripheral: GPIO, signal: 'PIO1, 8', pin_signal: PIO1_8/CAPT_YL, identifier: M1_STEP, direction: OUTPUT}
  - {pin_num: '2', peripheral: GPIO, signal: 'PIO0, 13', pin_signal: PIO0_13/ADC_10, identifier: M1_DIR, direction: OUTPUT}
  - {pin_num: '4', peripheral: GPIO, signal: 'PIO0, 12', pin_signal: PIO0_12, identifier: M0_STEP, direction: OUTPUT}
  - {pin_num: '6', peripheral: GPIO, signal: 'PIO0, 4', pin_signal: PIO0_4/ADC_11, identifier: M2_DIR, direction: OUTPUT}
  - {pin_num: '7', peripheral: GPIO, signal: 'PIO0, 28', pin_signal: PIO0_28/WKTCLKIN, identifier: M2_STEP, direction: OUTPUT}
  - {pin_num: '18', peripheral: GPIO, signal: 'PIO0, 26', pin_signal: PIO0_26, direction: OUTPUT}
  - {pin_num: '17', peripheral: GPIO, signal: 'PIO0, 27', pin_signal: PIO0_27, identifier: M3_DIR, direction: OUTPUT}
  - {pin_num: '36', peripheral: GPIO, signal: 'PIO0, 0', pin_signal: PIO0_0/ACMP_I1, direction: OUTPUT}
  - {pin_num: '35', peripheral: GPIO, signal: 'PIO1, 7', pin_signal: PIO1_7/CAPT_X8, direction: OUTPUT}
  - {pin_num: '34', peripheral: GPIO, signal: 'PIO0, 6', pin_signal: PIO0_6/ADC_1/ACMPVREF, identifier: M5_DIR, direction: OUTPUT}
  - {pin_num: '33', peripheral: GPIO, signal: 'PIO0, 7', pin_signal: PIO0_7/ADC_0, identifier: M5_STEP, direction: OUTPUT}
  - {pin_num: '28', peripheral: GPIO, signal: 'PIO1, 6', pin_signal: PIO1_6/CAPT_X7, identifier: M7_STEP, direction: OUTPUT}
  - {pin_num: '27', peripheral: GPIO, signal: 'PIO1, 5', pin_signal: PIO1_5/CAPT_X6, identifier: M7_DIR, direction: OUTPUT}
  - {pin_num: '26', peripheral: GPIO, signal: 'PIO0, 8', pin_signal: PIO0_8/XTALIN, identifier: M6_DIR, direction: OUTPUT}
  - {pin_num: '25', peripheral: GPIO, signal: 'PIO0, 9', pin_signal: PIO0_9/XTALOUT, identifier: M6_STEP, direction: OUTPUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitMotorPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M0P */
void BOARD_InitMotorPins(void)
{
    /* Enables the clock for the GPIO0 module */
    CLOCK_EnableClock(kCLOCK_Gpio0);
    /* Enables the clock for the GPIO1 module */
    CLOCK_EnableClock(kCLOCK_Gpio1);

    gpio_pin_config_t M4_DIR_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO0_0 (pin 36)  */
    GPIO_PinInit(BOARD_INITMOTORPINS_M4_DIR_GPIO, BOARD_INITMOTORPINS_M4_DIR_PORT, BOARD_INITMOTORPINS_M4_DIR_PIN, &M4_DIR_config);

    gpio_pin_config_t M2_DIR_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO0_4 (pin 6)  */
    GPIO_PinInit(BOARD_INITMOTORPINS_M2_DIR_GPIO, BOARD_INITMOTORPINS_M2_DIR_PORT, BOARD_INITMOTORPINS_M2_DIR_PIN, &M2_DIR_config);

    gpio_pin_config_t gpio0_pin0_6 = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO0_6 (pin )  */
    GPIO_PinInit(GPIO, 0U, 6U, &gpio0_pin0_6);

    gpio_pin_config_t gpio0_pin0_7 = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO0_7 (pin )  */
    GPIO_PinInit(GPIO, 0U, 7U, &gpio0_pin0_7);

    gpio_pin_config_t gpio0_pin0_8 = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO0_8 (pin )  */
    GPIO_PinInit(GPIO, 0U, 8U, &gpio0_pin0_8);

    gpio_pin_config_t gpio0_pin0_9 = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO0_9 (pin )  */
    GPIO_PinInit(GPIO, 0U, 9U, &gpio0_pin0_9);

    gpio_pin_config_t M0_STEP_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO0_12 (pin 4)  */
    GPIO_PinInit(BOARD_INITMOTORPINS_M0_STEP_GPIO, BOARD_INITMOTORPINS_M0_STEP_PORT, BOARD_INITMOTORPINS_M0_STEP_PIN, &M0_STEP_config);

    gpio_pin_config_t M1_DIR_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO0_13 (pin 2)  */
    GPIO_PinInit(BOARD_INITMOTORPINS_M1_DIR_GPIO, BOARD_INITMOTORPINS_M1_DIR_PORT, BOARD_INITMOTORPINS_M1_DIR_PIN, &M1_DIR_config);

    gpio_pin_config_t M3_STEP_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO0_26 (pin 18)  */
    GPIO_PinInit(BOARD_INITMOTORPINS_M3_STEP_GPIO, BOARD_INITMOTORPINS_M3_STEP_PORT, BOARD_INITMOTORPINS_M3_STEP_PIN, &M3_STEP_config);

    gpio_pin_config_t M3_DIR_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO0_27 (pin 17)  */
    GPIO_PinInit(BOARD_INITMOTORPINS_M3_DIR_GPIO, BOARD_INITMOTORPINS_M3_DIR_PORT, BOARD_INITMOTORPINS_M3_DIR_PIN, &M3_DIR_config);

    gpio_pin_config_t M2_STEP_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO0_28 (pin 7)  */
    GPIO_PinInit(BOARD_INITMOTORPINS_M2_STEP_GPIO, BOARD_INITMOTORPINS_M2_STEP_PORT, BOARD_INITMOTORPINS_M2_STEP_PIN, &M2_STEP_config);

    gpio_pin_config_t gpio1_pin1_5 = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO1_5 (pin )  */
    GPIO_PinInit(GPIO, 1U, 5U, &gpio1_pin1_5);

    gpio_pin_config_t gpio1_pin1_6 = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO1_6 (pin )  */
    GPIO_PinInit(GPIO, 1U, 6U, &gpio1_pin1_6);

    gpio_pin_config_t M4_STEP_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO1_7 (pin 35)  */
    GPIO_PinInit(BOARD_INITMOTORPINS_M4_STEP_GPIO, BOARD_INITMOTORPINS_M4_STEP_PORT, BOARD_INITMOTORPINS_M4_STEP_PIN, &M4_STEP_config);

    gpio_pin_config_t M1_STEP_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO1_8 (pin 1)  */
    GPIO_PinInit(BOARD_INITMOTORPINS_M1_STEP_GPIO, BOARD_INITMOTORPINS_M1_STEP_PORT, BOARD_INITMOTORPINS_M1_STEP_PIN, &M1_STEP_config);

    gpio_pin_config_t M0_DIR_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U,
    };
    /* Initialize GPIO functionality on pin PIO1_9 (pin 3)  */
    GPIO_PinInit(BOARD_INITMOTORPINS_M0_DIR_GPIO, BOARD_INITMOTORPINS_M0_DIR_PORT, BOARD_INITMOTORPINS_M0_DIR_PIN, &M0_DIR_config);

    /* Disable clock for switch matrix. */
    CLOCK_DisableClock(kCLOCK_Swm);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitRS485:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '23', peripheral: USART0, signal: RTS, pin_signal: PIO1_4/CAPT_X5, direction: OUTPUT}
  - {pin_num: '22', peripheral: USART0, signal: TXD, pin_signal: PIO0_15}
  - {pin_num: '24', peripheral: USART0, signal: RXD, pin_signal: PIO0_1/ACMP_I2/CLKIN}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitRS485
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
/* Function assigned for the Cortex-M0P */
void BOARD_InitRS485(void)
{
    /* Enables clock for switch matrix.: enable */
    CLOCK_EnableClock(kCLOCK_Swm);

    /* USART0_TXD connect to P0_15 */
    SWM_SetMovablePinSelect(SWM0, kSWM_USART0_TXD, kSWM_PortPin_P0_15);

    /* USART0_RXD connect to P0_1 */
    SWM_SetMovablePinSelect(SWM0, kSWM_USART0_RXD, kSWM_PortPin_P0_1);

    /* USART0_RTS connect to P1_4 */
    SWM_SetMovablePinSelect(SWM0, kSWM_USART0_RTS, kSWM_PortPin_P1_4);

    /* Disable clock for switch matrix. */
    CLOCK_DisableClock(kCLOCK_Swm);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
