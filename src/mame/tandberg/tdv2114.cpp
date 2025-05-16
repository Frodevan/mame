// license:BSD-3-Clause
// copyright-holders:Frode van der Meeren
/***************************************************************************

	Tandberg TDV-2114/24 Computer

	This driver uses the TDV-2100 series Display Logic module together with
	a CPU, memory and a floppy disk drive controller module. This is the
	computers of the TDV-2100 series.

	More optional modules exist, such as a QIC tape interface, different
	configurations of RAM/ROM, cluster interface, as well as loadable
	characters (char 0-32).

****************************************************************************/

#include "emu.h"

#include "tdv2100_disp_logic.h"
#include "tdv2100_kbd.h"
#include "tdv2100_cpu.h"

namespace {

class tdv2114_state : public driver_device
{
public:
	tdv2114_state(const machine_config &mconfig, device_type type, const char *tag) :
		driver_device(mconfig, type, tag),
		m_terminal(*this, "terminal"),
		m_keyboard(*this, "keyboard"),
		m_cpu(*this, "processor")
	{}

	void tdv2114_5(machine_config &config);
	void tdv2124_2(machine_config &config);
	void tdv2124_3(machine_config &config);

private:
	required_device<tandberg_tdv2100_disp_logic_device> m_terminal;
	required_device<tandberg_tdv2100_keyboard_device> m_keyboard;
	required_device<tandberg_tdv2100_cpu_device> m_cpu;
};

void tdv2114_state::tdv2114_5(machine_config& config)
{
	TANDBERG_TDV2114_DISPLAY_LOGIC(config, m_terminal);
	m_terminal->write_waitl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::waitl_w));
	m_terminal->write_onlil_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::onlil_w));
	m_terminal->write_carl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::carl_w));
	m_terminal->write_errorl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::errorl_w));
	m_terminal->write_enql_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::enql_w));
	m_terminal->write_ackl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::ackl_w));
	m_terminal->write_nakl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::nakl_w));
	m_terminal->write_vsync_int_callback().set(m_cpu, FUNC(tandberg_tdv2100_cpu_device::ireq_1_w));
	m_terminal->write_general_int_callback().set(m_cpu, FUNC(tandberg_tdv2100_cpu_device::ireq_3_w));

	TANDBERG_TDV2100_KEYBOARD(config, m_keyboard);
	m_keyboard->write_kstr_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::process_keyboard_char));
	m_keyboard->write_cleark_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::cleark_w));
	m_keyboard->write_linek_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::linek_w));
	m_keyboard->write_transk_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::transk_w));
	m_keyboard->write_break_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::break_w));

	TANDBERG_TDV2114_5_CPU(config, m_cpu);
	m_cpu->write_iack_1_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::ack_vsync_int_w));
	m_cpu->write_iack_3_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::ack_general_int_w));
	m_cpu->write_io_port_e4_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::transmit_cpu_data));
	m_cpu->write_io_port_e5_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::process_cpu_char));
	m_cpu->write_io_port_e6_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::process_cpu_data));
	m_cpu->write_io_port_e7_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::handle_cpu_command));
	m_cpu->read_io_port_e4().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::get_received_data));
	m_cpu->read_io_port_e5().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::get_char_data));
	m_cpu->read_io_port_e6().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::get_keyboard_data));
	m_cpu->read_io_port_e7().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::get_terminal_status));
	m_cpu->read_io_port_f6().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::get_interrupt_status));
	m_cpu->read_io_port_f7().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::get_uart_status));
}

void tdv2114_state::tdv2124_2(machine_config& config)
{
	TANDBERG_TDV2114_DISPLAY_LOGIC(config, m_terminal);
	m_terminal->write_waitl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::waitl_w));
	m_terminal->write_onlil_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::onlil_w));
	m_terminal->write_carl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::carl_w));
	m_terminal->write_errorl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::errorl_w));
	m_terminal->write_enql_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::enql_w));
	m_terminal->write_ackl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::ackl_w));
	m_terminal->write_nakl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::nakl_w));
	m_terminal->write_vsync_int_callback().set(m_cpu, FUNC(tandberg_tdv2100_cpu_device::ireq_1_w));
	m_terminal->write_general_int_callback().set(m_cpu, FUNC(tandberg_tdv2100_cpu_device::ireq_3_w));

	TANDBERG_TDV2100_KEYBOARD(config, m_keyboard);
	m_keyboard->write_kstr_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::process_keyboard_char));
	m_keyboard->write_cleark_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::cleark_w));
	m_keyboard->write_linek_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::linek_w));
	m_keyboard->write_transk_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::transk_w));
	m_keyboard->write_break_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::break_w));

	TANDBERG_TDV2124_2_CPU(config, m_cpu);
	m_cpu->write_iack_1_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::ack_vsync_int_w));
	m_cpu->write_iack_3_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::ack_general_int_w));
	m_cpu->write_io_port_e4_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::transmit_cpu_data));
	m_cpu->write_io_port_e5_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::process_cpu_char));
	m_cpu->write_io_port_e6_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::process_cpu_data));
	m_cpu->write_io_port_e7_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::handle_cpu_command));
	m_cpu->read_io_port_e4().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::get_received_data));
	m_cpu->read_io_port_e5().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::get_char_data));
	m_cpu->read_io_port_e6().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::get_keyboard_data));
	m_cpu->read_io_port_e7().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::get_terminal_status));
	m_cpu->read_io_port_f6().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::get_interrupt_status));
	m_cpu->read_io_port_f7().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::get_uart_status));
}

void tdv2114_state::tdv2124_3(machine_config& config)
{
	TANDBERG_TDV2114_DISPLAY_LOGIC(config, m_terminal);
	m_terminal->write_waitl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::waitl_w));
	m_terminal->write_onlil_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::onlil_w));
	m_terminal->write_carl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::carl_w));
	m_terminal->write_errorl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::errorl_w));
	m_terminal->write_enql_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::enql_w));
	m_terminal->write_ackl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::ackl_w));
	m_terminal->write_nakl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::nakl_w));
	m_terminal->write_vsync_int_callback().set(m_cpu, FUNC(tandberg_tdv2100_cpu_device::ireq_1_w));
	m_terminal->write_general_int_callback().set(m_cpu, FUNC(tandberg_tdv2100_cpu_device::ireq_3_w));

	TANDBERG_TDV2100_KEYBOARD(config, m_keyboard);
	m_keyboard->write_kstr_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::process_keyboard_char));
	m_keyboard->write_cleark_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::cleark_w));
	m_keyboard->write_linek_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::linek_w));
	m_keyboard->write_transk_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::transk_w));
	m_keyboard->write_break_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::break_w));

	TANDBERG_TDV2124_3_CPU(config, m_cpu);
	m_cpu->write_iack_1_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::ack_vsync_int_w));
	m_cpu->write_iack_3_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::ack_general_int_w));
	m_cpu->write_io_port_e4_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::transmit_cpu_data));
	m_cpu->write_io_port_e5_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::process_cpu_char));
	m_cpu->write_io_port_e6_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::process_cpu_data));
	m_cpu->write_io_port_e7_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::handle_cpu_command));
	m_cpu->read_io_port_e4().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::get_received_data));
	m_cpu->read_io_port_e5().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::get_char_data));
	m_cpu->read_io_port_e6().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::get_keyboard_data));
	m_cpu->read_io_port_e7().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::get_terminal_status));
	m_cpu->read_io_port_f6().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::get_interrupt_status));
	m_cpu->read_io_port_f7().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::get_uart_status));
}

static INPUT_PORTS_START( tdv2115l )
INPUT_PORTS_END

// ROM definition
ROM_START( tdv2114_5 )
ROM_END

// ROM definition
ROM_START( tdv2124_2 )
ROM_END

// ROM definition
ROM_START( tdv2124_3 )
ROM_END

} // anonymous namespace

//    YEAR  NAME        PARENT  COMPAT  MACHINE     INPUT     CLASS           INIT        COMPANY     FULLNAME                   FLAGS
COMP( 1976, tdv2114_5,  0,      0,      tdv2114_5,  tdv2115l, tdv2114_state,  empty_init, "Tandberg", "TDV-2114 XMON/CD rev.5",  MACHINE_SUPPORTS_SAVE )
COMP( 1980, tdv2124_2,  0,      0,      tdv2124_2,  tdv2115l, tdv2114_state,  empty_init, "Tandberg", "TDV-2124 XMON/F rev.2",   MACHINE_SUPPORTS_SAVE )
COMP( 1980, tdv2124_3,  0,      0,      tdv2124_3,  tdv2115l, tdv2114_state,  empty_init, "Tandberg", "TDV-2124 XMON/F rev.3",   MACHINE_SUPPORTS_SAVE )
