// license:BSD-3-Clause
// copyright-holders:Frode van der Meeren
/***************************************************************************

    Tandberg TDV-2100 Series expansion - DRAM card

****************************************************************************/

#include "tdv2100_dram.h"

DEFINE_DEVICE_TYPE(TDV2100_EXP_DRAM, tandberg_tdv2100_dram_device, "tandberg_tdv2100_exp_dram", "DRAM expansion board (Intel) for Tandberg TDV-2100 Series")

tandberg_tdv2100_dram_device::tandberg_tdv2100_dram_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock):
	device_t(mconfig, TDV2100_EXP_DRAM, tag, owner, clock),
	device_tandberg_tdv2100_card_interface(mconfig, *this)
{
}

void tandberg_tdv2100_dram_device::device_start()
{
}
