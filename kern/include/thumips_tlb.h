/*
 * =====================================================================================
 *
 *       Filename:  thumips_tlb.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/06/2012 10:20:30 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Chen Yuheng (Chen Yuheng), chyh1990@163.com
 *   Organization:  Tsinghua Unv.
 *
 * =====================================================================================
 */
#ifndef _THUMIPS_TLB_H
#define _THUMIPS_TLB_H

#include <asm/mipsregs.h>

#define THUMIPS_TLB_ENTRYL_V (1<<1)
#define THUMIPS_TLB_ENTRYL_D (1<<2)
#define THUMIPS_TLB_ENTRYL_G (1<<0)
#define THUMIPS_TLB_ENTRYH_VPN2_MASK (~0x1FFF)

static inline void write_one_tlb(int index, unsigned int pagemask, unsigned int hi, unsigned int low0, unsigned int low1)
{
	write_c0_entrylo0(low0);
	write_c0_pagemask(pagemask);
	write_c0_entrylo1(low1);
	write_c0_entryhi(hi);
	write_c0_index(index);
	tlb_write_indexed();
}

static inline void tlb_replace_random(unsigned int pagemask, unsigned int hi, unsigned int low0, unsigned int low1)
{
	write_c0_entrylo0(low0);
	write_c0_pagemask(pagemask);
	write_c0_entrylo1(low1);
	write_c0_entryhi(hi);
  tlb_write_random();
}

void tlb_invalidate_all();
#endif
