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

#include "common.h"

static void __init mini2440_dt_map_io(void)
{
	s3c24xx_init_io(NULL, 0);
}

static void __init mini2440_dt_machine_init(void)
{
	s3c_pm_init();
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
