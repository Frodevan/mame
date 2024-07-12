// license:BSD-3-Clause
// copyright-holders:Frode van der Meeren
/***************************************************************************

    Tandberg TDV-2100 Series expansion - 16K SRAM card

****************************************************************************/

#ifndef MAME_BUS_TANDBERG_TDV2100_EXP_SRAM_H
#define MAME_BUS_TANDBERG_TDV2100_EXP_SRAM_H

#pragma once

#include "tdv2100.h"

class tandberg_tdv2100_sram_device : public device_t, public device_tandberg_tdv2100_card_interface
{
	public:
		tandberg_tdv2100_sram_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	private:
		virtual void device_start() override;
};

DECLARE_DEVICE_TYPE(TDV2100_EXP_SRAM, tandberg_tdv2100_sram_device)

#endif // MAME_BUS_TANDBERG_TDV2100_EXP_SRAM_H
