/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Configuration for Xilinx ZynqMP Flash utility
 *
 * (C) Copyright 2018 Xilinx, Inc.
 * Michal Simek <michal.simek@xilinx.com>
 * Siva Durga Prasad Paladugu <sivadur@xilinx.com>
 */

#ifndef __CONFIG_ZYNQ_MICROZED_H
#define __CONFIG_ZYNQ_MICROZED_H

#include <configs/zynq-common.h>

/* Don't allow to overwrite serial and ethaddr */
#ifndef  CONFIG_ENV_OVERWRITE
#define  CONFIG_ENV_OVERWRITE
#endif

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
#define BOOT_TARGET_DEVICES_MMC(func) func(MMC, mmc, 0)

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
    "boot_targets=dhcp qspi mmc0\0"					\
    "distro_bootcmd="							\
    "for target in ${boot_targets}; do "				\
    "run bootcmd_${target}; "						\
    "env exists modeboot && run $modeboot; "				\
    "done\0"  

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS		\
        "ethaddr=5e:cc:27:0d:56:31\0"		\
	"initrd_high=0x20000000\0"	\
	"fdt_high=0x20000000\0"	\
	"fpgaaddr=0x100000\0"	\
	"fpgafile=system.bit.bin\0"	\
	"loadaddr=0x2080000\0"			\
	"loadfile=uImage\0"			\
	"loadsize=0xF00000\0"	\
	"rootaddr=0x4000000\0"	\
	"rootfile=uramdisk.image.gz\0"	\
	"rootsize=0x5E0000\0"	\
	"treeaddr=0x2000000\0"	\
	"treefile=devicetree.dtb\0"	\
	"treesize=0x20000\0"	\
	"uenvaddr=0x20000\0"	\
	"uenvfile=uEnv.txt\0" \
	"uenvoffs=0xE2FC0000\0"	\
	"uenvsize=0x40000\0"	\
	"tftpboot $fpgaaddr $serverip:$bootpath/$fpgafile && "		\
	"fpgaload=fpga loadb 0 $fpgaaddr $filesize && "			\
	"echo Loaded FPGA bitvector from $fpgaaddr:$filesize\0" \
	"tftpfpga=tftpboot $fpgaaddr $serverip:$bootpath/$fpgafile\0"	\
	"tftpload=tftpboot $loadaddr $serverip:$bootpath/$loadfile\0"	\
	"tftproot=tftpboot $rootaddr $serverip:$bootpath/$rootfile\0"	\
	"tftptree=tftpboot $treeaddr $serverip:$bootpath/$treefile\0"	\
	"importbootenv=echo Importing environment at ${uenvaddr}:${filesize} && " \
	"env import -t ${uenvaddr} ${filesize}\0"			\
	"preboot=run bootcmd_mmc0\0" \
	"nfsboot=env set autoload 0; " \
	"if dhcp; then " \
	"run tftpfpga && " \
	"run tftpload && " \
	"run tftptree && " \
	"bootm $loadaddr - $treeaddr;" \
	"fi\0"								\
	"netboot=env set autoload 0; " \
	"if dhcp; then " \
	"run tftpfpga && " \
	"run tftpload && " \
	"run tftproot && " \
	"run tftptree && " \
	"bootm $loadaddr $rootaddr $treeaddr;" \
	"fi\0"								\
	"qspiboot=if sf probe 0 0 0; then " \
	"sf read ${loadaddr} ${loadoffs} ${loadsize} && "	  \
	"sf read ${treeaddr} ${treeoffs} ${treesize} && "	  \
	"sf read ${rootaddr} ${rootoffs} ${rootsize} && "	  \
	"bootm ${loadaddr} ${rootaddr} ${treeaddr}; " \
	"fi\0"								\
	"sdboot=if mmcinfo; then " \
	"echo Copying Linux from SD to RAM... && "		     \
	"load mmc 0 ${loadaddr} ${loadfile} && "			\
	"load mmc 0 ${treeaddr} ${treefile} && " \
	"setenv bootargs $bootargs root=/dev/mmcblk0p2 rw rootwait && " \
	"bootm ${loadaddr} - $treeaddr; "		\
	"fi\0"								\
	"usbboot=" \
	"if usb start; then " \
	"run uenvboot; "					    \
	"echo Copying Linux from USB to RAM... && "		     \
	"load usb 0 ${loadaddr} ${loadfile} && "			\
	"load usb 0 ${treeaddr} ${treefile} && " \
	"load usb 0 ${rootaddr} ${rootfile} && "	\
	"bootm ${loadaddr} ${rootaddr} ${treeaddr}; " \
	"fi\0"								\
		BOOTENV

#endif /* __CONFIG_ZYNQ_MICROZED_H */
