// license:BSD-3-Clause
// copyright-holders:Frode van der Meeren
/***************************************************************************

    Tandberg TDV-2100 Series expansion bus

****************************************************************************/

#include "emu.h"
#include "tdv2100.h"

DEFINE_DEVICE_TYPE(TANDBERG_TDV2100_BUS, tandberg_tdv2100_bus_device, "tdv2100_bus", "Tandberg TDV-2100 series expansion bus device")
DEFINE_DEVICE_TYPE(TANDBERG_TDV2100_BUS_SLOT, tandberg_tdv2100_bus_slot_device, "tdv2100_slot", "Tandberg TDV-2100 series expansion card slot")

///////////////////////////////////////////////////////////////////////////////
//
// Card Interface
//

device_tandberg_tdv2100_card_interface::device_tandberg_tdv2100_card_interface(const machine_config &mconfig, device_t &device):
	device_interface(device, "tdv2100_exp_bus"),
	m_bus(nullptr)
{
}


device_tandberg_tdv2100_card_interface::~device_tandberg_tdv2100_card_interface()
{
}


void device_tandberg_tdv2100_card_interface::interface_pre_start()
{
	if (!m_bus)
	{
		throw device_missing_dependencies();
	}
}

///////////////////////////////////////////////////////////////////////////////
//
// Bus device
//

tandberg_tdv2100_bus_device::tandberg_tdv2100_bus_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock):
	tandberg_tdv2100_bus_device(mconfig, TANDBERG_TDV2100_BUS, tag, owner, clock)
{
}


tandberg_tdv2100_bus_device::tandberg_tdv2100_bus_device(const machine_config &mconfig,  device_type type, const char *tag, device_t *owner, uint32_t clock):
	device_t(mconfig, type, tag, owner, clock),
	m_memspace(*this, finder_base::DUMMY_TAG, -1),
	//m_iospace(*this, finder_base::DUMMY_TAG, -1),
	m_write_ireq2_cb(*this),
	m_write_ireq4_cb(*this),
	m_write_ireq5_cb(*this),
	m_write_ireq6_cb(*this),
	m_write_ireq7_cb(*this),
	m_write_dr1_cb(*this),
	m_write_dr2_cb(*this),
	m_write_dr3_cb(*this),
	m_write_dr4_cb(*this)
{
}


void tandberg_tdv2100_bus_device::install_ram(offs_t start, offs_t end, uint8_t *data)
{
	m_memspace->install_ram(start, end, data);
}


void tandberg_tdv2100_bus_device::device_start()
{
}


void tandberg_tdv2100_bus_device::device_reset()
{
}


///////////////////////////////////////////////////////////////////////////////
//
// Card slot device
//

tandberg_tdv2100_bus_slot_device::tandberg_tdv2100_bus_slot_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock):
	device_t(mconfig, TANDBERG_TDV2100_BUS_SLOT, tag, owner, clock),
	device_single_card_slot_interface<device_tandberg_tdv2100_card_interface>(mconfig, *this),
	m_bus(*this, finder_base::DUMMY_TAG)
{
}


void tandberg_tdv2100_bus_slot_device::device_start()
{
}


void tandberg_tdv2100_bus_slot_device::device_resolve_objects()
{
	device_tandberg_tdv2100_card_interface *const intf(get_card_device());
	if (intf)
	{
		intf->set_tdv2100_bus(m_bus.target());
	}
}
