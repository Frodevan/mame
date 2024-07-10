// license:BSD-3-Clause
// copyright-holders:Frode van der Meeren
/***************************************************************************

    Tandberg TDV-2100 Series expansion bus


    (Component side)         (Solder side)

                   # P1 #

    B1       GND        GND             A1
    B2       +5V        +5V             A2
    B3   IO  AB16*                      A3
    B4   IO  AB17*                      A4
    B5   IO  AB18*                      A5
    B6   IO  AB19*                      A6
    B7   IO  AB9        AB8         IO  A7
    B8   IO  AB11       AB10        IO  A8
    B9   IO  DB1        DB0         IO  A9
    B10  I   GRANT3     GRANT4      I   A10
    B11  I   GRANT2     DB2         IO  A11
    B12  IO  AB15       GRANT1      I   A12
    B13  IO  AB13       AB14        IO  A13
    B14  IO  DB6        AB12        IO  A14
    B15  IO  AB6        DB7         IO  A15
    B16  IO  AB2        AB4         IO  A16
    B17   O   /DR4      /DR1         O  A17
    B18  IO  AB7        AB0         IO  A18
    B19  IO  AB3        AB5         IO  A19
    B20   O   WAIT      AB1         IO  A20
    B21   O   /DR3      /DR2         O  A21
    B22      +12V       +12V            A22
    B23      -5V        -5V             A23
    B24   O  /IREQ4     (/IREQ3)     O  A24
    B25   O  /IREQ2     (/IREQ1)     O  A25
    B26   O  /IREQ7     /IREQ6       O  A26
    B27   O  /IREQ5     /IACK6      I   A27
    B28  I   /IACK5     /IACK4      I   A28
    B29  I   (/IACK3)   /IACK2      I   A29
    B30  I   (/IACK1)   /IACK0      I   A30
    B31  IO  DB3        DB4         IO  A31
    B32  IO  DB5        /IACK7      I   A32

                   # P2 #

    B1                  DOTCLK      I   A1
    B2                  -12V            A2
    B3                  /DMARW      I   A3
    B4                  INTE        I   A4
    B5                  /INTA       I   A5
    B6                  /MEMR       I   A6
    B7                  /OUT        I   A7
    B8                  /INP        I   A8
    B9                  3000        I   A9
    B10                 /SINGLE      O  A10
    B11                 PH2TTL      I   A11
    B12                 STEP         O  A12
    B13                 /RESET      I   A13
    B14                 /MCLR       I   A14
    B15                 /MIN        IO  A15
    B16                 /MW         IO  A16
    B17                 /IOIN       I   A17
    B18                 /WTRQ        O  A18
    B19                 /IOW        I   A19
    B20                 /DMAPLS     I   A20
    B21                 307.2KHz    I   A21
    B22                                 A22
    B23                                 A23
    B24                 FL+12V          A24
    B25                 FL-12V          A25
    B26                                 A26
    B27                                 A27
    B28                                 A28
    B29                                 A29
    B30                 /STROBE     I   A30
    B31                 PH1TTL      I   A31
    B32                                 A32

    Unmarked pins are either unused or reserved for slot-spesific purposes.
    Labels in paranthesis are provided as separate strobes on the CPU module.


        DB0-7       Data bus
        DA0-19      Address bus*
        IREQ1-7     Interrupt requests
        IACK1-7     Interrupt ack
        DR1-4       DMA requests
        GRANT1-4    DMA ack

            * NOTE: AB16-19 are undocumented, and only provided on certain modules.
                    These were never properly taken in use, but their presence may
                    indicate that Tandberg was planning for the posibility of an
                    x86-based computer re-using an extended version of this bus.

        3000        Memory range 0x3000-0x3fff strobe

        INTE        CPU INTE output
        WAIT        CPU WAIT input

        /MCLR       Manual asynchronous reset line
        /RESET      CPU-synchronized reset line

        /WTRQ       CPU Waitstate strobe
        /SINGLE     CPU Single-step enable strobe
        STEP        CPU Single-step clock

        /INTA       CPU general interrupt ack
        /OUT        CPU general write address available
        /INP        CPU IO read address available
        /MEMR       CPU Memory read address available

        /MIN        Memory read data enable strobe
        /MW         Memory write data available
        /IOIN       IO read data enable
        /IOW        IO write data available

        /STROBE     Strobe before CPU memory read cycle
        /DMARW      Strobe before DMA memory R/W cycle
        /DMAPLS     DMA memory R/W strobe

        DOTCLOCK    20.2752MHz Display dot-clock
        PH1TTL      2.02752MHz CPU clock, early 25% dury-cycle
        PH2TTL      2.02752MHz CPU clock, late 50% duty-cycle
        307.2KHz    307.2KHz UART baudrate clock base

****************************************************************************/

#ifndef MAME_BUS_TANDBERG_TDV2100_H
#define MAME_BUS_TANDBERG_TDV2100_H

#pragma once

#include <forward_list>

class tandberg_tdv2100_bus_device;
class tandberg_tdv2100_bus_slot_device;

///////////////////////////////////////////////////////////////////////////////
//
// Interface
//

class device_tandberg_tdv2100_card_interface : public device_interface
{
	friend class tandberg_tdv2100_bus_device;

public:
	virtual ~device_tandberg_tdv2100_card_interface();

	// signals from the Bus to be added later

	void set_tdv2100_bus(tandberg_tdv2100_bus_device *tdv2100_bus) { m_bus = tdv2100_bus; }

protected:
	device_tandberg_tdv2100_card_interface(const machine_config &mconfig, device_t &device);
	virtual void interface_pre_start() override;
	tandberg_tdv2100_bus_device *m_bus;
};

///////////////////////////////////////////////////////////////////////////////
//
// Bus device
//

class tandberg_tdv2100_bus_device : public device_t
{
	public:
		tandberg_tdv2100_bus_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock = 0);

		template <typename T> void set_memspace(T &&tag, int spacenum) { m_memspace.set_tag(std::forward<T>(tag), spacenum); }
		/*
		template <typename T> void set_iospace(T &&tag, int spacenum) { m_iospace.set_tag(std::forward<T>(tag), spacenum); }
		*/
		
		void install_ram(offs_t start, offs_t end, uint8_t *data);

		auto ireq2_callback() { return m_write_ireq2_cb.bind(); }
		auto ireq4_callback() { return m_write_ireq4_cb.bind(); }
		auto ireq5_callback() { return m_write_ireq5_cb.bind(); }
		auto ireq6_callback() { return m_write_ireq6_cb.bind(); }
		auto ireq7_callback() { return m_write_ireq7_cb.bind(); }

		auto dr1_callback() { return m_write_dr1_cb.bind(); }
		auto dr2_callback() { return m_write_dr2_cb.bind(); }
		auto dr3_callback() { return m_write_dr3_cb.bind(); }
		auto dr4_callback() { return m_write_dr4_cb.bind(); }

	private:
		tandberg_tdv2100_bus_device(const machine_config &mconfig,  device_type type, const char *tag, device_t *owner, uint32_t clock);

		virtual void device_start() override;
		virtual void device_reset() override;

		required_address_space m_memspace;
		//required_address_space m_iospace;

		devcb_write_line    m_write_ireq2_cb;
		devcb_write_line    m_write_ireq4_cb;
		devcb_write_line    m_write_ireq5_cb;
		devcb_write_line    m_write_ireq6_cb;
		devcb_write_line    m_write_ireq7_cb;

		devcb_write_line    m_write_dr1_cb;
		devcb_write_line    m_write_dr2_cb;
		devcb_write_line    m_write_dr3_cb;
		devcb_write_line    m_write_dr4_cb;

		std::forward_list<tandberg_tdv2100_bus_slot_device *> m_slot_list;
};

DECLARE_DEVICE_TYPE(TANDBERG_TDV2100_BUS, tandberg_tdv2100_bus_device)

///////////////////////////////////////////////////////////////////////////////
//
// Card slot device
//

class tandberg_tdv2100_bus_slot_device : public device_t,
										 public device_single_card_slot_interface<device_tandberg_tdv2100_card_interface>
{
public:
	// construction/destruction
	template <typename T, typename U>
	tandberg_tdv2100_bus_slot_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock, T &&bus_tag, U &&opts, const char *dflt, bool fixed):
		tandberg_tdv2100_bus_slot_device(mconfig, tag, owner)
	{
		option_reset();
		opts(*this);
		set_default_option(dflt);
		set_fixed(fixed);
		m_bus.set_tag(std::forward<T>(bus_tag));
	}
	tandberg_tdv2100_bus_slot_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock = 0);

protected:
	virtual void device_start() override;
	virtual void device_resolve_objects() override;

private:
	required_device<tandberg_tdv2100_bus_device> m_bus;
};

DECLARE_DEVICE_TYPE(TANDBERG_TDV2100_BUS_SLOT, tandberg_tdv2100_bus_slot_device)

void tdv_2100_cards(device_slot_interface &device);

#endif // MAME_BUS_TANDBERG_TDV2100_H
