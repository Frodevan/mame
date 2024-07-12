// license:BSD-3-Clause
// copyright-holders:Frode van der Meeren
/***************************************************************************

    Tandberg TDV-2100 Series expansion - DRAM card
	
	Two different styles of DRAM expansion cards exists, one by Tandberg
	and one by Intel. Functionality of the two cards is in many ways the
	same, but detailed implementation and settings differ.

	Both cards have 4 banks of DRAM, and both suport the 4116 style DRAM
	chips. Intended way of allocating memory is to start adding RAM at top
	of memory first, and then additional RAM is added downwards through
	the memory map in one continous block. With the 8080 CPU card, a total
	of 48KB of RAM is supported, but later CPU cards were modified from
	factory to allow for 54KB expansion RAM in the system.
	
	The Intel card maps the memory into 8KB chunks, and maps these chunks
	thereafter using a patch pad with soldered-on jumper-wires. This way
	individual 8KB chunks can be moved around, enabled/disabled, something
	which adds support for using only the upper/lower half of a bank, in
	case the other half contains bad bits. This card was often sold with
	2 banks 16Kbit chips + 2 banks of 8KBit chips for 48KB total, or in
	systems with the CPU card patch the memory card would have all 4 banks
	use 16Kbit chips for a total of 54KB RAM. In this configuration the
	memory card would have a patch accordingly to disable the lower 10KB
	RAM, since this is reserved by the CPU card.

****************************************************************************/

#ifndef MAME_BUS_TANDBERG_TDV2100_DRAM_H
#define MAME_BUS_TANDBERG_TDV2100_DRAM_H

#pragma once

#include "tdv2100.h"

class tandberg_tdv2100_dram_device : public device_t, public device_tandberg_tdv2100_card_interface
{
	public:
		tandberg_tdv2100_dram_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	private:
		virtual void device_start() override;
};

DECLARE_DEVICE_TYPE(TDV2100_EXP_DRAM, tandberg_tdv2100_dram_device)

#endif // MAME_BUS_TANDBERG_TDV2100_DRAM_H
