/*
 * drivers/char/sunxi_sys_info/sunxi-smc.c
 *
 * Copyright(c) 2015-2016 Allwinnertech Co., Ltd.
 *         http://www.allwinnertech.com
 *
 * Author: sunny <superm@allwinnertech.com>
 *
 * allwinner sunxi soc chip version and chip id manager.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __SUNXI_SMC_H
#define __SUNXI_SMC_H

extern int invoke_smc_fn(u32 function_id, u64 arg0, u64 arg1, u64 arg2);
extern u32 sunxi_smc_readl(phys_addr_t addr);
extern int sunxi_smc_writel(u32 value, phys_addr_t addr);
extern int sunxi_smc_probe_secure(void);
extern int sunxi_soc_is_secure(void);
extern int sunxi_smc_copy_arisc_paras(phys_addr_t dest, phys_addr_t src, u32 len);

#ifndef CONFIG_ARM64
static inline u32 sunxi_readl(phys_addr_t addr)
{
#ifdef CONFIG_SUNXI_SMC
	if (sunxi_soc_is_secure())
		return sunxi_smc_readl(addr);
	else
#endif
	return readl(IO_ADDRESS(addr));
}

static inline void sunxi_writel(u32 val, phys_addr_t addr)
{
#ifdef CONFIG_SUNXI_SMC
	if (sunxi_soc_is_secure())
		sunxi_smc_writel(val, addr);
	else
#endif
	writel(val, IO_ADDRESS(addr));
}
#endif

#ifndef CONFIG_SUNXI_SMC
static inline u32 sunxi_smc_readl(phys_addr_t addr)
{
	void __iomem *vaddr = ioremap(addr, 4);
	u32 ret;
	ret = readl(vaddr);
	iounmap(vaddr);
	return ret;
	return readl((void __iomem *)addr);
}
static inline int sunxi_smc_writel(u32 val, phys_addr_t addr)
{
	void __iomem *vaddr = ioremap(addr, 4);
	writel(val, vaddr);
	iounmap(vaddr);
	return 0;
}
#endif


#endif  /* __SUNXI_SMC_H */
