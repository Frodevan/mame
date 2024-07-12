// license:BSD-3-Clause
// copyright-holders:Frode van der Meeren
/***************************************************************************

    Tandberg TDV-2100 Series expansion - 16K SRAM card

****************************************************************************/

#include "emu.h"
#include "tdv2100_sram.h"

DEFINE_DEVICE_TYPE(TDV2100_EXP_SRAM, tandberg_tdv2100_sram_device, "tandberg_tdv2100_exp_sram", "16K SRAM expansion board for Tandberg TDV-2100 Series")

tandberg_tdv2100_sram_device::tandberg_tdv2100_sram_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock):
	device_t(mconfig, TDV2100_EXP_SRAM, tag, owner, clock),
	device_tandberg_tdv2100_card_interface(mconfig, *this)
{
}

void tandberg_tdv2100_sram_device::device_start()
{
	m_ram.resize(0x4000);
	m_bus->install_ram(0x4000, 0x7FFF, &m_ram[0]);	
}

void tandberg_tdv2100_sram_device::device_reset()
{
}
