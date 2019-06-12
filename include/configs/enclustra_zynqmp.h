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

#ifdef BOOTENV_DEV_DHCP
#undef BOOTENV_DEV_DHCP
#endif
#define BOOTENV_DEV_DHCP(devtypeu, devtypel, instance)	\
	"bootcmd_dhcp="					\
	"if dhcp ${uenvaddr} ${bootfile}; then "	\
	"env import -t ${uenvaddr} ${filesize}; "	\
	"fi; "						\
	"run $modeboot; "                               \
	"\0"

#ifdef BOOTENV_DEV_MMC
#undef BOOTENV_DEV_MMC
#endif
#define BOOTENV_DEV_MMC(devtypeu, devtypel, instance)	 \
	"bootcmd_mmc" #instance "="			 \
	"setenv devnum " #instance "; "			 \
	"mmc rescan; mmc dev ${devnum}; "		 \
	"mmc info; mmc part; "				 \
	"if load mmc ${devnum} ${uenvaddr} ${bootfile}; then " \
	"env import -t {uenvaddr} ${filesize}; "	\
	"fi; "				\
        "run $modeboot; "		\
	"\0"

#ifdef BOOTENV_DEV_QSPI
#undef BOOTENV_DEV_QSPI
#endif
#define BOOTENV_DEV_QSPI(devtypeu, devtypel, instance) \
	"bootcmd_" #devtypel #instance "="             \
	"sf probe " #instance " 0 0; " \
	"if sf read $uenvaddr $script_offset_f $script_size_f; then "	\
	"env import -t ${uenvaddr} $script_size_f; " \
	"fi; "				\
        "run $modeboot; "		\
	"\0"

#undef BOOT_TARGET_DEVICES
#define BOOT_TARGET_DEVICES(func) \
        func(DHCP, dhcp, na) \
        func(MMC, mmc, 1) \
	func(QSPI, qspi, 0) \


/* Undef unneeded configs */
/* Xilinx initial environment variables */
#undef CONFIG_EXTRA_ENV_BOARD_SETTINGS
#define CONFIG_EXTRA_ENV_BOARD_SETTINGS \
	"ethaddr=10:65:30:0f:30:8f\0"		\
	"serverip=172.16.0.1\0"			\
	"bootpath=ZynqMP/1065300f308f\0"	\
	"fpgafile=download.bit\0"		\
	"treefile=devicetree.dtb\0"		\
	"rootfile=ramdisk.cpio.gz\0"		\
	"bootfile=Image\0"			\
	"uenvaddr=0x80000\0"			\
	"fpgaaddr=0x100000\0"			\
	"treeaddr=0x4000000\0"			\
	"rootaddr=0x4010000\0"			\
	"bootaddr=0x80000\0"			\
	"netboot="					       \
	"tftpboot $fpgaaddr $serverip:$bootpath/$fpgafile && " \
	"fpga loadb 0 $fpgaaddr $filesize; "		       \
	"tftpboot $bootaddr $serverip:$bootpath/$bootfile; " \
	"tftpboot $rootaddr $serverip:$bootpath/$rootfile; " \
	"tftpboot $treeaddr $serverip:$bootpath/$treefile; " \
	"echo bootm $bootaddr $rootaddr $treeaddr\0" \
	"modeboot=netboot\0" \
	PARTS_DEFAULT

#endif /* __CONFIG_ENCLUSTRA_ZYNQMP_H */
