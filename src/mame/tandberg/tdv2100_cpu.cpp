// license:BSD-3-Clause
// copyright-holders:Frode van der Meeren
/***************************************************************************

	Tandberg TDV-2100 Series CPU module

	Adds a 8080 CPU with 2KB of RAM and up to 8KB of ROM. This is used by
	either the TDV-2114, TDV-2116 or the TDV-2124 machines.


	Input stobes:

	    * Interrupt 1
	    * Interrupt 2
	    * Interrupt 3
	    * Interrupt 4
	    * Interrupt 5
	    * Interrupt 6
	    * Interrupt 7

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

		* Add CPU
		* Add RAM/ROM
	    * Add Display CPU interface and strobes
	    * Add Interrupts
		* Add expansion bus

****************************************************************************/

#include "emu.h"
#include "tdv2100_cpu.h"

DEFINE_DEVICE_TYPE(TANDBERG_TDV2114_CPU, tandberg_tdv2114_cpu_device, "tandberg_tdv2114_cpu", "Tandberg TDV-2100 series CPU card with XMON/D rev.5 ROM");
DEFINE_DEVICE_TYPE(TANDBERG_TDV2124_CPU, tandberg_tdv2124_cpu_device, "tandberg_tdv2124_cpu", "Tandberg TDV-2100 series CPU card with XMON/F rev.3 ROM");

tandberg_tdv2100_cpu_device::tandberg_tdv2100_cpu_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock):
	device_t(mconfig, type, tag, owner, clock),
	m_rom(*this, "kernel_rom"),
	m_ram(*this, "work_ram", 0x800, ENDIANNESS_LITTLE),
	m_uart(*this, "uart"),
	m_uart_clock(*this, "uart_clock"),
	m_rs232(*this, "serial")
{}

///////////////////////////////////////////////////////////////////////////////
//
// General machine state
//

void tandberg_tdv2100_cpu_device::device_start()
{
}

void tandberg_tdv2100_cpu_device::device_reset()
{
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
