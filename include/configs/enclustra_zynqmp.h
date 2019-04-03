/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Configuration for Xilinx ZynqMP Flash utility
 *
 * (C) Copyright 2018 Xilinx, Inc.
 * Michal Simek <michal.simek@xilinx.com>
 * Siva Durga Prasad Paladugu <sivadur@xilinx.com>
 */

#ifndef __CONFIG_ENCLUSTRA_ZYNQMP_H
#define __CONFIG_ENCLUSTRA_ZYNQMP_H

#include <configs/xilinx_zynqmp.h>

#undef BOOT_TARGET_DEVICES
#define BOOT_TARGET_DEVICES(func) \
        func(DHCP, dhcp, na) \
        func(MMC, mmc, 1) \
	func(QSPI, qspi, 0) \


/* Undef unneeded configs */
/* Xilinx initial environment variables */
#undef CONFIG_EXTRA_ENV_BOARD_SETTINGS
#define CONFIG_EXTRA_ENV_BOARD_SETTINGS \
	"ethaddr=10:65:30:0f:30:8f\0" \
	PARTS_DEFAULT

#endif /* __CONFIG_ENCLUSTRA_ZYNQMP_H */
