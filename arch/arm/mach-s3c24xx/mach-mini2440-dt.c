// SPDX-License-Identifier: GPL-2.0
//
// Samsung's S3C2416 flattened device tree enabled machine
//
// Copyright (c) 2012 Heiko Stuebner <heiko@sntech.de>
//
// based on mach-exynos/mach-exynos4-dt.c
//
// Copyright (c) 2010-2011 Samsung Electronics Co., Ltd.
//		http://www.samsung.com
// Copyright (c) 2010-2011 Linaro Ltd.
//		www.linaro.org

#include <linux/clocksource.h>
#include <linux/irqchip.h>
#include <linux/serial_s3c.h>

#include <asm/mach/arch.h>
#include <mach/map.h>

#include <plat/cpu.h>
#include <plat/pm.h>

#include <linux/dm9000.h>
#include <linux/platform_device.h>

#include <linux/platform_data/mtd-nand-s3c2410.h>

#include <linux/mtd/mtd.h>
#include <linux/mtd/rawnand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/partitions.h>

#include <plat/devs.h>

#include "common.h"

#define MACH_MINI2440_DM9K_BASE (S3C2410_CS4 + 0x300)

/* DM9000AEP 10/100 ethernet controller */

static struct resource mini2440_dm9k_resource[] = {
	[0] = DEFINE_RES_MEM(MACH_MINI2440_DM9K_BASE, 4),
	[1] = DEFINE_RES_MEM(MACH_MINI2440_DM9K_BASE + 4, 4),
	[2] = DEFINE_RES_NAMED(IRQ_EINT7, 1, NULL, IORESOURCE_IRQ
						| IORESOURCE_IRQ_HIGHEDGE),
};

/*
 * The DM9000 has no eeprom, and it's MAC address is set by
 * the bootloader before starting the kernel.
 */
static struct dm9000_plat_data mini2440_dm9k_pdata = {
	.flags		= (DM9000_PLATF_16BITONLY | DM9000_PLATF_NO_EEPROM),
};

static struct platform_device mini2440_device_eth = {
	.name		= "dm9000",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(mini2440_dm9k_resource),
	.resource	= mini2440_dm9k_resource,
	.dev		= {
		.platform_data	= &mini2440_dm9k_pdata,
	},
};

/* NAND Flash on MINI2440 board */

static struct mtd_partition mini2440_default_nand_part[] __initdata = {
	[0] = {
		.name	= "u-boot",
		.size	= SZ_256K,
		.offset	= 0,
	},
	[1] = {
		.name	= "u-boot-env",
		.size	= SZ_128K,
		.offset	= SZ_256K,
	},
	[2] = {
		.name	= "kernel",
		/* 5 megabytes, for a kernel with no modules
		 * or a uImage with a ramdisk attached
		 */
		.size	= 0x00500000,
		.offset	= SZ_256K + SZ_128K,
	},
	[3] = {
		.name	= "root",
		.offset	= SZ_256K + SZ_128K + 0x00500000,
		.size	= MTDPART_SIZ_FULL,
	},
};

static struct s3c2410_nand_set mini2440_nand_sets[] __initdata = {
	[0] = {
		.name		= "nand",
		.nr_chips	= 1,
		.nr_partitions	= ARRAY_SIZE(mini2440_default_nand_part),
		.partitions	= mini2440_default_nand_part,
		.flash_bbt	= 1, /* we use u-boot to create a BBT */
	},
};

static struct s3c2410_platform_nand mini2440_nand_info __initdata = {
	.tacls		= 0,
	.twrph0		= 25,
	.twrph1		= 15,
	.nr_sets	= ARRAY_SIZE(mini2440_nand_sets),
	.sets		= mini2440_nand_sets,
	.ignore_unset_ecc = 1,
	.ecc_mode	= NAND_ECC_NONE,
};

static void __init mini2440_dt_map_io(void)
{
	s3c24xx_init_io(NULL, 0);
}

static struct platform_device *mini2440_devices[] __initdata = {
	&mini2440_device_eth,
	&s3c_device_nand,
};


static void __init mini2440_dt_machine_init(void)
{
	s3c_nand_set_platdata(&mini2440_nand_info);
	s3c_pm_init();

	platform_add_devices(mini2440_devices, ARRAY_SIZE(mini2440_devices));
}

static const char *const mini2440_dt_compat[] __initconst = {
	"samsung,mini2440",
	NULL
};

DT_MACHINE_START(MINI2440, "Samsung MINI2440 (Flattened Device Tree)")
	/* Maintainer: Heiko Stuebner <heiko@sntech.de> */
	.dt_compat	= mini2440_dt_compat,
	.map_io		= mini2440_dt_map_io,
	.init_irq	= irqchip_init,
	.init_machine	= mini2440_dt_machine_init,
MACHINE_END
