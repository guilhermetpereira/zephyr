/*
 * Copyright (c) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * @addtogroup t_uart_basic
 * @{
 * @defgroup t_uart_config test_uart_config
 * @brief TestPurpose: verify UART configure API settings
 * @details
 * - Test Steps
 *   - Configure: test_uart_configure( )
 *   - Configure Get: test_uart_config_get( )
 * - Expected Results
 *   -# When test UART CONFIG Configure, the value of configurations actually
 *      set will be equal to the original configuration values (from device
 *      tree or run-time configuration to modify those loaded initially from
 *      device tree)
 *   -# When test UART CONFIG Configure Get, the app will get/retrieve the
 *      value of configurations stored at location and to be passed to UART
 *      CONFIG Configure
 * @}
 */

#include "test_uart.h"
struct uart_config uart_cfg_check;

/* test UART configure (set configuration) */
static int test_configure(void)
{
	const struct uart_config uart_cfg = {
		.baudrate = 115200,
		.parity = UART_CFG_PARITY_NONE,
		.stop_bits = UART_CFG_STOP_BITS_1,
		.data_bits = UART_CFG_DATA_BITS_8,
		.flow_ctrl = UART_CFG_FLOW_CTRL_NONE
	};

	struct device *uart_dev = device_get_binding(UART_DEVICE_NAME);

	if (!uart_dev) {
		TC_PRINT("Cannot get UART device\n");
		return TC_FAIL;
	}

	TC_PRINT("This is a configure test.\n");

	/* Verify configure() - set device configuration using data in cfg */
	/* 0 if successful, - error code otherwise */
	int ret = uart_configure(uart_dev, &uart_cfg);

	/* Confirm the values provided are the values set*/
	/* so get the configurations from the device and check */
	uart_config_get(uart_dev, &uart_cfg_check);
	if (memcmp(&uart_cfg, &uart_cfg_check, sizeof(uart_cfg)) != 0) {
		return TC_FAIL;
	}

	return (ret == 0) ? TC_PASS : TC_FAIL;
}

/* test UART configure get (retrieve configuration) */
static int test_config_get(void)
{
	struct device *uart_dev = device_get_binding(UART_DEVICE_NAME);
	struct uart_config uart_cfg;

	if (!uart_dev) {
		TC_PRINT("Cannot get UART device\n");
		return TC_FAIL;
	}

	TC_PRINT("This is a configure_get test.\n");

	/* Verify uart_config_get() - get device configuration, put in cfg */
	/* 0 if successful, - error code otherwise */
	int ret = uart_config_get(uart_dev, &uart_cfg);

	/* Confirm the values from device are the values put in cfg*/
	if (memcmp(&uart_cfg, &uart_cfg_check, sizeof(uart_cfg)) != 0) {
		return TC_FAIL;
	}
	return (ret == 0) ? TC_PASS : TC_FAIL;

}

void test_uart_configure(void)
{
	zassert_true(test_configure() == TC_PASS, NULL);
}

void test_uart_config_get(void)
{
	zassert_true(test_config_get() == TC_PASS, NULL);
}
