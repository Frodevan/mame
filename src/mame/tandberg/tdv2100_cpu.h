// license:BSD-3-Clause
// copyright-holders:Frode van der Meeren
/***************************************************************************

	Tandberg TDV-2100 Series CPU module

****************************************************************************/

#ifndef MAME_TANDBERG_TDV2100_CPU_H
#define MAME_TANDBERG_TDV2100_CPU_H

#pragma once

#include "machine/clock.h"
#include "machine/ram.h"
#include "machine/ay31015.h"
#include "bus/rs232/rs232.h"

class tandberg_tdv2100_cpu_device : public device_t
{
public:
	tandberg_tdv2100_cpu_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock = 0);

	auto write_iack_1_callback() { return m_write_iack_1_cb.bind(); }
	auto write_iack_3_callback() { return m_write_iack_3_cb.bind(); }

	auto write_io_port_e4_callback() { return m_write_io_port_e4_cb.bind(); }
	auto write_io_port_e5_callback() { return m_write_io_port_e5_cb.bind(); }
	auto write_io_port_e6_callback() { return m_write_io_port_e6_cb.bind(); }
	auto write_io_port_e7_callback() { return m_write_io_port_e7_cb.bind(); }

	auto read_io_port_e4() { return m_read_io_port_e4.bind(); }
	auto read_io_port_e5() { return m_read_io_port_e5.bind(); }
	auto read_io_port_e6() { return m_read_io_port_e6.bind(); }
	auto read_io_port_e7() { return m_read_io_port_e7.bind(); }
	auto read_io_port_f6() { return m_read_io_port_f6.bind(); }
	auto read_io_port_f7() { return m_read_io_port_f7.bind(); }

	void ireq_1_w(uint8_t state);
	void ireq_3_w(uint8_t state);

	DECLARE_INPUT_CHANGED_MEMBER(uart_changed);

protected:
	virtual void device_add_mconfig(machine_config &config) override;
	virtual void device_start() override;
	virtual void device_reset() override;
	virtual ioport_constructor device_input_ports() const override;

private:

	void ireq_change(uint8_t mask, uint8_t state);
	IRQ_CALLBACK_MEMBER(inta_cb);

	void tdv2100_mem(address_map &map);
	void tdv2100_io(address_map &map);
	uint8_t io_r(offs_t addr);
	void io_w(offs_t addr, uint8_t data);
	void set_uart_state_from_switches();

	required_device<cpu_device>             m_cpu;
	required_region_ptr<uint8_t>            m_rom;
	required_device<ram_device>             m_ram;
	memory_view                             m_cpu_mem_view;
	required_device<ay51013_device>         m_uart;
	required_device<clock_device>           m_uart_clock;
	required_device<rs232_port_device>      m_rs232;

	devcb_write_line                    m_write_iack_1_cb;
	devcb_write_line                    m_write_iack_3_cb;

	devcb_write8                        m_write_io_port_e4_cb;
	devcb_write8                        m_write_io_port_e5_cb;
	devcb_write8                        m_write_io_port_e6_cb;
	devcb_write8                        m_write_io_port_e7_cb;

	devcb_read8                         m_read_io_port_e4;
	devcb_read8                         m_read_io_port_e5;
	devcb_read8                         m_read_io_port_e6;
	devcb_read8                         m_read_io_port_e7;
	devcb_read8                         m_read_io_port_f6;
	devcb_read8                         m_read_io_port_f7;

	uint8_t m_ireq_state;
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

#endif // MAME_TANDBERG_TDV2100_CPU_H
