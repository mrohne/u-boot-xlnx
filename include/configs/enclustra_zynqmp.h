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

/* Allow to overwrite serial and ethaddr */
#ifdef  CONFIG_ENV_OVERWRITE
#undef  CONFIG_ENV_OVERWRITE
#endif
#define CONFIG_ENV_OVERWRITE

/* enable preboot to be loaded before CONFIG_BOOTDELAY */
#ifdef  CONFIG_PREBOOT
#undef  CONFIG_PREBOOT
#endif
#define CONFIG_PREBOOT "run bootcmd_mmc1"

#undef BOOTENV_DEV_NAME_DHCP
#define BOOTENV_DEV_NAME_DHCP(devtypeu, devtypel, instance) \
  #devtypel " "  
#undef BOOTENV_DEV_DHCP
#define BOOTENV_DEV_DHCP(devtypeu, devtypel, instance)	\
	"bootcmd_dhcp="					\
	"env set autoload 1; " \
	"if dhcp ${uenvaddr}; then " \
	"echo Importing environment at $fileaddr:$filesize; " \
	"env import -t $fileaddr $filesize; "		\
	"fi; "						\
	"\0"
#undef BOOT_TARGET_DEVICES_DHCP
#define BOOT_TARGET_DEVICES_DHCP(func) func(DHCP, dhcp, 0)

#undef BOOTENV_DEV_NAME_MMC
#define BOOTENV_DEV_NAME_MMC(devtypeu, devtypel, instance) \
  #devtypel #instance " "  
#undef BOOTENV_DEV_MMC
#define BOOTENV_DEV_MMC(devtypeu, devtypel, instance)	\
	"bootcmd_mmc" #instance "="			\
	"setenv devnum " #instance "; "			\
	"mmc rescan; mmc dev ${devnum}; "		\
	"mmc info; mmc part; "				\
	"if load mmc ${devnum} ${uenvaddr} ${uenvfile}; then "	\
	"echo Importing environment at $fileaddr:$filesize; " \
	"env import -t $fileaddr $filesize; "		\
	"fi; "							\
	"\0"
#undef BOOT_TARGET_DEVICES_MMC
#define BOOT_TARGET_DEVICES_MMC(func) func(MMC, mmc, 1)

#undef BOOTENV_DEV_NAME_QSPI
#define BOOTENV_DEV_NAME_QSPI(devtypeu, devtypel, instance) \
  #devtypel #instance " "  
#undef BOOTENV_DEV_QSPI
#define BOOTENV_DEV_QSPI(devtypeu, devtypel, instance) \
	"bootcmd_" #devtypel #instance "="             \
	"sf probe " #instance " 0 0; "					\
	"if sf read $uenvaddr $uenvoffs $uenvsize; then "	\
	"echo Importing environment at $fileaddr:$filesize; " \
	"env import -t $fileaddr $filesize; "		\
	"fi; "								\
	"\0"
#undef BOOT_TARGET_DEVICES_QSPI
#define BOOT_TARGET_DEVICES_QSPI(func) func(QSPI, qspi, 0)

#undef BOOTENV_DEV
#define BOOTENV_DEV(devtypeu, devtypel, instance) \
	BOOTENV_DEV_##devtypeu(devtypeu, devtypel, instance)

#undef BOOTENV_DEV_NAME
#define BOOTENV_DEV_NAME(devtypeu, devtypel, instance)			\
	BOOTENV_DEV_NAME_##devtypeu(devtypeu, devtypel, instance)

#undef BOOT_TARGET_DEVICES
#define BOOT_TARGET_DEVICES(func)      \
  BOOT_TARGET_DEVICES_DHCP(func)       \
    BOOT_TARGET_DEVICES_QSPI(func)     \
    BOOT_TARGET_DEVICES_MMC(func)

#undef BOOTENV
#define BOOTENV		   \
  BOOT_TARGET_DEVICES(BOOTENV_DEV)					\
    "boot_targets=dhcp qspi mmc1\0"					\
    "distro_bootcmd="							\
    "for target in ${boot_targets}; do "				\
    "run bootcmd_${target}; "						\
    "env exists modeboot && run $modeboot; "				\
    "done\0"  

#undef  ENV_MEM_LAYOUT_SETTINGS
#define ENV_MEM_LAYOUT_SETTINGS \
	"uenvsize=0x00080000\0"			\
	"uenvaddr=0x02000000\0"			\
	"rootaddr=0x02100000\0"			\
	"treeaddr=0x04000000\0"			\
	"bootaddr=0x18000000\0"			\
	"fpgaaddr=0x40000000\0"			\

/* Undef unneeded configs */
/* Xilinx initial environment variables */
#undef  CONFIG_EXTRA_ENV_BOARD_SETTINGS
#define CONFIG_EXTRA_ENV_BOARD_SETTINGS \
	"ethaddr=10:65:30:0f:30:8f\0"		\
	"uenvfile=uEnv.txt\0"                   \
	"testaddr=0xdeadbeef\0"			\

#undef  CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS	\
	CONFIG_EXTRA_ENV_BOARD_SETTINGS \
	ENV_MEM_LAYOUT_SETTINGS		\
	BOOTENV				\

#endif /* __CONFIG_ENCLUSTRA_ZYNQMP_H */
