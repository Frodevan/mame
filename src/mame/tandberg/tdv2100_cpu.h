// license:BSD-3-Clause
// copyright-holders:Frode van der Meeren
/***************************************************************************

	Tandberg TDV-2100 Series CPU module

****************************************************************************/

#ifndef MAME_TANDBERG_CPU_H
#define MAME_TANDBERG_CPU_H

#pragma once

#include "machine/clock.h"
#include "machine/ram.h"
#include "machine/ay31015.h"
#include "bus/rs232/rs232.h"

class tandberg_tdv2100_cpu_device : public device_t
{
public:
	tandberg_tdv2100_cpu_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock = 0);

	DECLARE_INPUT_CHANGED_MEMBER(uart_changed);

protected:
	virtual void device_add_mconfig(machine_config &config) override;
	virtual void device_start() override;
	virtual void device_reset() override;
	virtual ioport_constructor device_input_ports() const override;

private:

	void set_uart_state_from_switches();

	required_region_ptr<uint8_t>        m_rom;
	memory_share_creator<uint8_t>       m_ram;
	required_device<ay51013_device>     m_uart;
	required_device<clock_device>       m_uart_clock;
	required_device<rs232_port_device>  m_rs232;
};

class tandberg_tdv2114_cpu_device : public tandberg_tdv2100_cpu_device
{
	public:
		tandberg_tdv2114_cpu_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock = 0);

	protected:
		virtual const tiny_rom_entry *device_rom_region() const override;
};

class tandberg_tdv2124_cpu_device : public tandberg_tdv2100_cpu_device
{
	public:
		tandberg_tdv2124_cpu_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock = 0);

	protected:
		virtual const tiny_rom_entry *device_rom_region() const override;
};

// device type definition
DECLARE_DEVICE_TYPE(TANDBERG_TDV2114_CPU, tandberg_tdv2114_cpu_device)
DECLARE_DEVICE_TYPE(TANDBERG_TDV2124_CPU, tandberg_tdv2124_cpu_device)

#endif // MAME_TANDBERG_CPU_H
