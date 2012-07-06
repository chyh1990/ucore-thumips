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

static inline void write_one_tlb(int index, unsigned int pagemask, unsigned int hi, unsigned int low0, unsigned int low1)
{
	write_c0_entrylo0(low0);
	write_c0_pagemask(pagemask);
	write_c0_entrylo1(low1);
	write_c0_entryhi(hi);
	write_c0_index(index);
	tlb_write_indexed();
}

void tlb_invalidate_all();
void tlb_invalidate(pde_t *pgdir, uintptr_t la);

#endif
