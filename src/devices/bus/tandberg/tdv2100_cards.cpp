// license:BSD-3-Clause
// copyright-holders:Frode van der Meeren
/***************************************************************************

    Tandberg TDV-2100 Series expansion cards

****************************************************************************/

#include "tdv2100_cards.h"

#include "tdv2100_dram.h"
#include "tdv2100_sram.h"

void tdv_2100_cards(device_slot_interface &device)
{
	device.option_add("dram_memory", TDV2100_EXP_DRAM);
	device.option_add("sram_memory", TDV2100_EXP_SRAM);
}
