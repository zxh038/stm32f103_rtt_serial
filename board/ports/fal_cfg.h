/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-5      SummerGift   first version
 */

#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_

#include <rtthread.h>
#include <board.h>

extern const struct fal_flash_dev stm32_onchip_flash;
extern struct fal_flash_dev nor_flash0;

/* flash device table */
#define FAL_FLASH_DEV_TABLE                                          \
{                                                                    \
    &stm32_onchip_flash,                                             \
    &nor_flash0,                                                     \
}
/* ====================== Partition Configuration ========================== */
#ifdef FAL_PART_HAS_TABLE_CFG

/* partition table */
#define FAL_PART_TABLE                                                                          \
{                                                                                               \
    {FAL_PART_MAGIC_WORD,        "app", "onchip_flash",             0,       496*1024, 0},      \
    {FAL_PART_MAGIC_WORD,      "param", "onchip_flash",      496*1024,        16*1024, 0},      \
    {FAL_PART_MAGIC_WORD,       "font",    "norflash0",             0,    6*1024*1024, 0},      \
    {FAL_PART_MAGIC_WORD, "filesystem",    "norflash0",   6*1024*1024,    2*1024*1024, 0},      \
}
#endif /* FAL_PART_HAS_TABLE_CFG */
#endif /* _FAL_CFG_H_ */
