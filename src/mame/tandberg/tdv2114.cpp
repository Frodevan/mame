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

	void tdv2114(machine_config &config);
	void tdv2124(machine_config &config);

private:
	required_device<tandberg_tdv2100_disp_logic_device> m_terminal;
	required_device<tandberg_tdv2100_keyboard_device> m_keyboard;
	required_device<tandberg_tdv2100_cpu_device> m_cpu;
};

void tdv2114_state::tdv2114(machine_config& config)
{
	TANDBERG_TDV2114_DISPLAY_LOGIC(config, m_terminal);
	m_terminal->write_waitl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::waitl_w));
	m_terminal->write_onlil_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::onlil_w));
	m_terminal->write_carl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::carl_w));
	m_terminal->write_errorl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::errorl_w));
	m_terminal->write_enql_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::enql_w));
	m_terminal->write_ackl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::ackl_w));
	m_terminal->write_nakl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::nakl_w));

	TANDBERG_TDV2100_KEYBOARD(config, m_keyboard);
	m_keyboard->write_kstr_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::process_keyboard_char));
	m_keyboard->write_cleark_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::cleark_w));
	m_keyboard->write_linek_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::linek_w));
	m_keyboard->write_transk_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::transk_w));
	m_keyboard->write_break_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::break_w));

	TANDBERG_TDV2114_CPU(config, m_cpu);
}

void tdv2114_state::tdv2124(machine_config& config)
{
	TANDBERG_TDV2114_DISPLAY_LOGIC(config, m_terminal);
	m_terminal->write_waitl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::waitl_w));
	m_terminal->write_onlil_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::onlil_w));
	m_terminal->write_carl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::carl_w));
	m_terminal->write_errorl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::errorl_w));
	m_terminal->write_enql_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::enql_w));
	m_terminal->write_ackl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::ackl_w));
	m_terminal->write_nakl_callback().set(m_keyboard, FUNC(tandberg_tdv2100_keyboard_device::nakl_w));

	TANDBERG_TDV2100_KEYBOARD(config, m_keyboard);
	m_keyboard->write_kstr_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::process_keyboard_char));
	m_keyboard->write_cleark_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::cleark_w));
	m_keyboard->write_linek_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::linek_w));
	m_keyboard->write_transk_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::transk_w));
	m_keyboard->write_break_callback().set(m_terminal, FUNC(tandberg_tdv2100_disp_logic_device::break_w));

	TANDBERG_TDV2124_CPU(config, m_cpu);
}

static INPUT_PORTS_START( tdv2115l )
INPUT_PORTS_END

// ROM definition
ROM_START( tdv2114 )
ROM_END

// ROM definition
ROM_START( tdv2124 )
ROM_END

} // anonymous namespace

//    YEAR  NAME      PARENT  COMPAT  MACHINE   INPUT     CLASS           INIT        COMPANY     FULLNAME     FLAGS
COMP( 1976, tdv2114,  0,      0,      tdv2114,  tdv2115l, tdv2114_state,  empty_init, "Tandberg", "TDV-2114",  MACHINE_SUPPORTS_SAVE )
COMP( 1980, tdv2124,  0,      0,      tdv2124,  tdv2115l, tdv2114_state,  empty_init, "Tandberg", "TDV-2124",  MACHINE_SUPPORTS_SAVE )
