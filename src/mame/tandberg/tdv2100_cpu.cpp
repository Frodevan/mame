// license:BSD-3-Clause
// copyright-holders:Frode van der Meeren
/***************************************************************************

	Tandberg TDV-2100 Series CPU module

	Adds a 8080 CPU with 2KB of RAM and up to 8KB of ROM. This is used by
	either the TDV-2114, TDV-2116 or the TDV-2124 machines.


	Input stobes:

	    * Interrupt 1
	    * Interrupt 3

	Output strobes:

	    * IO E4 Read
	    * IO E4 Write
	    * IO E5 Read
	    * IO E5 Write
	    * IO E6 Read
	    * IO E6 Write
	    * IO E7 Read
	    * IO E7 Write
	    * IO F6 Read
	    * IO F7 Read


	TODO:

	    * Add expansion bus
	    * Add DMA
	    * Add CPU Printer UART

****************************************************************************/

#include "emu.h"
#include "cpu/i8085/i8085.h"
#include "tdv2100_cpu.h"
#include "tdv2100_disp_logic.h"

static constexpr XTAL CPU_CLOCK = DOT_CLOCK/10;

DEFINE_DEVICE_TYPE(TANDBERG_TDV2114_CPU, tandberg_tdv2114_cpu_device, "tandberg_tdv2114_cpu", "Tandberg TDV-2100 series CPU card with XMON/D rev.5 ROM");
DEFINE_DEVICE_TYPE(TANDBERG_TDV2124_CPU, tandberg_tdv2124_cpu_device, "tandberg_tdv2124_cpu", "Tandberg TDV-2100 series CPU card with XMON/F rev.3 ROM");

tandberg_tdv2100_cpu_device::tandberg_tdv2100_cpu_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock):
	device_t(mconfig, type, tag, owner, clock),
	m_cpu(*this, "processor"),
	m_rom(*this, "kernel_rom"),
	m_ram(*this, RAM_TAG),
	m_cpu_mem_view(*this, "mem_view"),
	m_uart(*this, "uart"),
	m_uart_clock(*this, "uart_clock"),
	m_rs232(*this, "serial"),
	m_write_iack_1_cb(*this),
	m_write_iack_3_cb(*this),
	m_write_io_port_e4_cb(*this),
	m_write_io_port_e5_cb(*this),
	m_write_io_port_e6_cb(*this),
	m_write_io_port_e7_cb(*this),
	m_read_io_port_e4(*this, 0xff),
	m_read_io_port_e5(*this, 0xff),
	m_read_io_port_e6(*this, 0xff),
	m_read_io_port_e7(*this, 0xff),
	m_read_io_port_f6(*this, 0xff),
	m_read_io_port_f7(*this, 0xff)
{}

///////////////////////////////////////////////////////////////////////////////
//
// General machine state
//

void tandberg_tdv2100_cpu_device::device_start()
{
	save_item(NAME(m_ireq_state));
}

void tandberg_tdv2100_cpu_device::device_reset()
{
	m_ireq_state = 0x00;
	m_cpu_mem_view.select(0);
}

///////////////////////////////////////////////////////////////////////////////
//
// CPU state
//

void tandberg_tdv2100_cpu_device::tdv2100_mem(address_map &map)
{
	map(0x0000, 0x2fff).view(m_cpu_mem_view);
	m_cpu_mem_view[0](0x0000, 0x1fff).rom().region("kernel_rom", 0);
	m_cpu_mem_view[0](0x2000, 0x27ff).rw(m_ram, FUNC(ram_device::read), FUNC(ram_device::write));
	// TODO: 0x2800-0x2fff goes to optional redefinable character expansion for terminal module
	// TODO: Map expansion bus
	//     m_cpu_mem_view[0](0x3000, 0xffff).rw(m_exp, FUNC(ram_device::read), FUNC(ram_device::write));
	//     m_cpu_mem_view[1](0x0000, 0xffff).rw(m_exp, FUNC(ram_device::read), FUNC(ram_device::write));
}

void tandberg_tdv2100_cpu_device::tdv2100_io(address_map &map)
{
	map.unmap_value_high();
	map(0x00, 0xff).rw(FUNC(tandberg_tdv2100_cpu_device::io_r), FUNC(tandberg_tdv2100_cpu_device::io_w));
	// TODO: Map expansion bus
}

uint8_t tandberg_tdv2100_cpu_device::io_r(offs_t addr)
{
	switch(addr)
	{
		case 0xe4:
			return m_read_io_port_e4();

		case 0xe5:
			return m_read_io_port_e5();

		case 0xe6:
			return m_read_io_port_e6();

		case 0xe7:
			return m_read_io_port_e7();

		case 0xf4:
			// Todo: CPU module UART
			return 0xff;

		case 0xf5:
			// Todo: CPU module UART
			return 0xff;

		case 0xf6:
			return m_read_io_port_f6();

		case 0xf7:
			return m_read_io_port_f7();

		default:
			break;
	}
	return 0xff;
}

void tandberg_tdv2100_cpu_device::io_w(offs_t addr, uint8_t data)
{
	switch(addr)
	{
		case 0xe4:
			m_write_io_port_e4_cb(data);
			return;

		case 0xe5:
			m_write_io_port_e5_cb(data);
			return;

		case 0xe6:
			m_write_io_port_e6_cb(data);
			return;

		case 0xe7:
			m_write_io_port_e7_cb(data);
			return;

		case 0xf4:
			// Todo: CPU module UART
			return;

		case 0xf5:
		case 0xf6:
			return; // Note: Reserved for bodge-wire patches

		case 0xf7:
			// TODO: If CP/M patch installed
			//     m_cpu_mem_view.select(1);
			return;

		default:
			break;
	}
	// Todo: Invoke io on expansion bus
}

///////////////////////////////////////////////////////////////////////////////
//
// Interrupts
//

void tandberg_tdv2100_cpu_device::ireq_1_w(uint8_t state)
{
	ireq_change(0x02, state);
}

void tandberg_tdv2100_cpu_device::ireq_3_w(uint8_t state)
{
	ireq_change(0x08, state);
}

void tandberg_tdv2100_cpu_device::ireq_change(uint8_t mask, uint8_t state)
{
	// Note: IREQ lines are initially active negative, but inverted by the encoder
	m_ireq_state = (m_ireq_state&~(mask))|((state)? 0x00 : mask);

	if(m_ireq_state)
	{
		m_cpu->set_input_line(0, ASSERT_LINE);
	}
	else
	{
		m_cpu->set_input_line(0, CLEAR_LINE);
	}
}

IRQ_CALLBACK_MEMBER( tandberg_tdv2100_cpu_device::inta_cb )
{
	// Note: IACK lines are active negative, for clearing IREQ latches on other modules
	if(m_ireq_state&0x02)
	{
		m_write_iack_1_cb(0);
		m_write_iack_1_cb(1);
		return 0xc7 | 0x08;
	}
	else if(m_ireq_state&0x04)
	{
		//m_write_iack_2_cb(0);
		//m_write_iack_2_cb(1);
		return 0xc7 | 0x10;
	}
	else if(m_ireq_state&0x08)
	{
		m_write_iack_3_cb(0);
		m_write_iack_3_cb(1);
		return 0xc7 | 0x18;
	}
	else if(m_ireq_state&0x10)
	{
		//m_write_iack_4_cb(0);
		//m_write_iack_4_cb(1);
		return 0xc7 | 0x20;
	}
	else if(m_ireq_state&0x20)
	{
		//m_write_iack_5_cb(0);
		//m_write_iack_5_cb(1);
		return 0xc7 | 0x28;
	}
	else if(m_ireq_state&0x40)
	{
		//m_write_iack_6_cb(0);
		//m_write_iack_6_cb(1);
		return 0xc7 | 0x30;
	}
	else if(m_ireq_state&0x80)
	{
		//m_write_iack_7_cb(0);
		//m_write_iack_7_cb(1);
		return 0xc7 | 0x38;
	}
	return 0xc7;
}

///////////////////////////////////////////////////////////////////////////////
//
// Printer port (RS-232)
//

INPUT_CHANGED_MEMBER(tandberg_tdv2100_cpu_device::uart_changed)
{
	set_uart_state_from_switches();
}

void tandberg_tdv2100_cpu_device::set_uart_state_from_switches()
{
	
}

///////////////////////////////////////////////////////////////////////////////
//
// Driver config
//

void tandberg_tdv2100_cpu_device::device_add_mconfig(machine_config &mconfig)
{
	I8080(mconfig, m_cpu, CPU_CLOCK);
	m_cpu->set_addrmap(AS_PROGRAM, &tandberg_tdv2100_cpu_device::tdv2100_mem);
	m_cpu->set_addrmap(AS_IO, &tandberg_tdv2100_cpu_device::tdv2100_io);
	m_cpu->set_irq_acknowledge_callback(FUNC(tandberg_tdv2100_cpu_device::inta_cb));

	RAM(mconfig, m_ram).set_default_size("2K").set_default_value(0xff);

	RS232_PORT(mconfig, m_rs232, default_rs232_devices, nullptr);
	//m_rs232->rxd_handler().set(FUNC(tandberg_tdv2100_disp_logic_device::rs232_rxd_w));
	//m_rs232->dcd_handler().set(FUNC(tandberg_tdv2100_disp_logic_device::rs232_dcd_w));
	//m_rs232->dsr_handler().set(FUNC(tandberg_tdv2100_disp_logic_device::rs232_dsr_w));
	//m_rs232->cts_handler().set(FUNC(tandberg_tdv2100_disp_logic_device::rs232_cts_w));
	//m_rs232->ri_handler().set(FUNC(tandberg_tdv2100_disp_logic_device::rs232_ri_w));

	AY51013(mconfig, m_uart);
	//m_uart->write_so_callback().set(FUNC(tandberg_tdv2100_disp_logic_device::rs232_txd_w));
	//m_uart->write_dav_callback().set(FUNC(tandberg_tdv2100_disp_logic_device::uart_rx));
	//m_uart->write_dav_callback().append(FUNC(tandberg_tdv2100_disp_logic_device::check_rs232_rx_error));
	//m_uart->write_tbmt_callback().set(FUNC(tandberg_tdv2100_disp_logic_device::uart_tx));
	//m_uart->write_pe_callback().set(FUNC(tandberg_tdv2100_disp_logic_device::check_rs232_rx_error));

	CLOCK(mconfig, m_uart_clock, 1);
	// NOTE: Frequency set with the rest of the UART settings
	m_uart_clock->signal_handler().set(m_uart, FUNC(ay51013_device::write_rcp));
	m_uart_clock->signal_handler().append(m_uart, FUNC(ay51013_device::write_tcp));
}

static INPUT_PORTS_START( tdv2114 )

	PORT_START("sw_rs232_baud")
		PORT_CONFNAME(0x7, 0x6, "SPEED SELECT [Note: Baud-rate]")                                PORT_CHANGED_MEMBER(DEVICE_SELF, tandberg_tdv2100_cpu_device, uart_changed, 0)
			PORT_CONFSETTING(0x0, "0: 110")
			PORT_CONFSETTING(0x1, "1: 300")
			PORT_CONFSETTING(0x2, "2: 600")
			PORT_CONFSETTING(0x3, "3: 1200")
			PORT_CONFSETTING(0x4, "4: 2400")
			PORT_CONFSETTING(0x5, "5: 4800")
			PORT_CONFSETTING(0x6, "6: 9600")
			PORT_CONFSETTING(0x7, "7: 19200")

INPUT_PORTS_END

ROM_START(tdv2114)
	ROM_REGION( 0x2000, "kernel_rom", ROMREGION_ERASEFF )
	ROM_LOAD( "eprom.2708.u1", 0x0000, 0x0400, CRC(83856611) SHA1(09adb5a8ceeac676bdd7ce44dcbdcb01c66614a3))
	ROM_LOAD( "eprom.2708.u2", 0x0400, 0x0400, CRC(e37d65c5) SHA1(a99e78ca5790ca1d19eab228efdafe5a4b51bf35))
	ROM_LOAD( "eprom.2708.u3", 0x0800, 0x0400, CRC(41c4092e) SHA1(fb36d07516fb0d8365196ad7136cacf0161635d6))
	ROM_LOAD( "eprom.2708.u4", 0x0C00, 0x0400, CRC(af5b5683) SHA1(cf1c077a9e2966bd36d5f75cf350f1744959c7ce))
	ROM_LOAD( "eprom.2708.u5", 0x1000, 0x0400, CRC(69b7e908) SHA1(f6079c88b2a40d4c8651242a9430be5774da0685))
	ROM_LOAD( "eprom.2708.u6", 0x1400, 0x0400, CRC(62d4e49a) SHA1(4d52d7763487f03baf0e0e28bae05285811c308e))
ROM_END

ROM_START(tdv2124)
	ROM_REGION( 0x2000, "kernel_rom", ROMREGION_ERASEFF )
	ROM_LOAD( "961045-3 1.2708.u1", 0x0000, 0x0400, CRC(bf4c8343) SHA1(b6c65267f4d5e04cc1bb88758022f9728bf10339))
	ROM_LOAD( "961045-3 2.2708.u2", 0x0400, 0x0400, CRC(ccd87858) SHA1(3b525891b770d95effa0b92d79fe9d9959676489))
	ROM_LOAD( "961045-3 3.2708.u3", 0x0800, 0x0400, CRC(b89dfc12) SHA1(b584b273b533c2bef345292d3c3f5ee804e4161d))
	ROM_LOAD( "961045-3 4.2708.u4", 0x0C00, 0x0400, CRC(ac59e955) SHA1(e6a633736549b927114a03bbbf49e74bcebca596))
	ROM_LOAD( "961045-3 5.2708.u5", 0x1000, 0x0400, CRC(e56cb758) SHA1(939cf600becb1e285049d58dd1506f4a24317312))
ROM_END

ioport_constructor tandberg_tdv2100_cpu_device::device_input_ports() const
{
	return INPUT_PORTS_NAME(tdv2114);
}

tandberg_tdv2114_cpu_device::tandberg_tdv2114_cpu_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock):
	tandberg_tdv2100_cpu_device(mconfig, TANDBERG_TDV2114_CPU, tag, owner, clock)
{}

const tiny_rom_entry *tandberg_tdv2114_cpu_device::device_rom_region() const
{
	return ROM_NAME(tdv2114);
}

tandberg_tdv2124_cpu_device::tandberg_tdv2124_cpu_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock):
	tandberg_tdv2100_cpu_device(mconfig, TANDBERG_TDV2124_CPU, tag, owner, clock)
{}

const tiny_rom_entry *tandberg_tdv2124_cpu_device::device_rom_region() const
{
	return ROM_NAME(tdv2124);
}
