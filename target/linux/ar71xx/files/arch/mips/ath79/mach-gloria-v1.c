/*
 *  TP-LINK TL-GLORIA_V1 board support
 *
 *  Copyright (C) 2012 Gabor Juhos <juhosg@openwrt.org>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/gpio.h>

#include <asm/mach-ath79/ath79.h>
#include <asm/mach-ath79/ar71xx_regs.h>

#include <linux/pci.h>
#include <linux/phy.h>
#include <linux/platform_device.h>
#include <linux/ath9k_platform.h>
#include <linux/ar8216_platform.h>
#include <linux/rtl8367.h>

#include "common.h"
#include "dev-ap9x-pci.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-spi.h"
#include "dev-usb.h"
#include "dev-wmac.h"
#include "machtypes.h"

#define GLORIA_V1_GPIO_LED_WLAN2G		13

#define GLORIA_V1_GPIO_BTN_RESET       17

#define GLORIA_V1_GPIO_LED_L1  12
#define GLORIA_V1_GPIO_LED_L2  14
#define GLORIA_V1_GPIO_LED_L3  15
#define GLORIA_V1_GPIO_LED_L4  16

#define GLORIA_V1_GPIO_LED_LAN1  21
#define GLORIA_V1_GPIO_LED_LAN2  22

#define GLORIA_V1_KEYS_POLL_INTERVAL	20	/* msecs */
#define GLORIA_V1_KEYS_DEBOUNCE_INTERVAL	(3 * GLORIA_V1_KEYS_POLL_INTERVAL)

#define GLORIA_V1_MAC0_OFFSET		0
#define GLORIA_V1_MAC1_OFFSET		6
#define GLORIA_V1_WMAC_CALDATA_OFFSET	0x1000
#define GLORIA_V1_PCIE_CALDATA_OFFSET	0x5000

static const char *gloria_v1_part_probes[] = {
	"tp-link",
	NULL,
};

static struct flash_platform_data gloria_v1_flash_data = {
	.part_probes	= gloria_v1_part_probes,
};

static struct gpio_led gloria_v1_leds_gpio[] __initdata = {
	{
		.name		= "gloria:green:lan1",
		.gpio		= GLORIA_V1_GPIO_LED_LAN1,
		.active_low	= 1,
	}, {
		.name		= "gloria:green:lan2",
		.gpio		= GLORIA_V1_GPIO_LED_LAN2,
		.active_low	= 1,
	}, {
		.name		= "gloria:green:wlan2g",
		.gpio		= GLORIA_V1_GPIO_LED_WLAN2G,
		.active_low	= 1,
	}, {
		.name       = "gloria:green:link1",
		.gpio       = GLORIA_V1_GPIO_LED_L1,
		.active_low = 1,
	}, {
		.name       = "gloria:green:link2",
		.gpio       = GLORIA_V1_GPIO_LED_L2,
		.active_low = 1,
	}, {
		.name       = "gloria:green:link3",
		.gpio       = GLORIA_V1_GPIO_LED_L3,
		.active_low = 1,
	}, {
		.name       = "gloria:green:link4",
		.gpio       = GLORIA_V1_GPIO_LED_L4,
		.active_low = 1,
	},
};

static struct gpio_keys_button gloria_v1_gpio_keys[] __initdata = {
	{
		.desc       = "Reset button",
		.type       = EV_KEY,
		.code       = KEY_RESTART,
		.debounce_interval = GLORIA_V1_KEYS_DEBOUNCE_INTERVAL,
		.gpio       = GLORIA_V1_GPIO_BTN_RESET,
		.active_low = 1,
	},
};

static void __init gloria_v1_setup(void)
{
	u8 *mac = (u8 *) KSEG1ADDR(0x1f01fc00);
	u8 *art = (u8 *) KSEG1ADDR(0x1fff0000);
	int i = 0;
	u8 tmpmac[ETH_ALEN];

	ath79_register_m25p80(&gloria_v1_flash_data);

	for (i = 0; i < ARRAY_SIZE(gloria_v1_leds_gpio); i++)
		ath79_gpio_output_select(gloria_v1_leds_gpio[i].gpio,
					 AR934X_GPIO_OUT_GPIO);

	ath79_register_leds_gpio(-1, ARRAY_SIZE(gloria_v1_leds_gpio),
				 gloria_v1_leds_gpio);

	ath79_register_gpio_keys_polled(-1, GLORIA_V1_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(gloria_v1_gpio_keys),
					gloria_v1_gpio_keys);


	ath79_init_mac(tmpmac, mac, 1);
	ath79_register_wmac(art + GLORIA_V1_WMAC_CALDATA_OFFSET, tmpmac);

	ath79_register_mdio(1, 0x0);
	/* GMAC1 is connected to the internal switch */
	ath79_init_mac(ath79_eth1_data.mac_addr, mac, 0);
	ath79_eth1_data.phy_if_mode = PHY_INTERFACE_MODE_GMII;

	ath79_register_eth(1);

}

MIPS_MACHINE(ATH79_MACH_GLORIA_V1, "GLORIA_V1",
		"GLORIA V1",
		gloria_v1_setup);
