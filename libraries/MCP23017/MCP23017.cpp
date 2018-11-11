/* MCP23017.h
*	Author: 	Melody Gurman
* 	Modified: 	11/11/2018
* * * * * * * * * * * * * * * */

#include <MCP23017.h>

#if defined( __AVR_ATtiny25__ ) | \
	defined( __AVR_ATtiny45__ ) | \
    defined( __AVR_ATtiny85__ )
#	include <TinyWireM.h>
#	define WIRE TinyWireM
#else
#	include <Wire.h>
#	define WIRE Wire
#endif

// Constructors/Destructor
// * * * * * * * * * * * * * * * * //


MCP23017::MCP23017()
	: m_addr(-1)
{
}

MCP23017::MCP23017(uint8_t addr)
	: m_addr(addr)
{
}

MCP23017::MCP23017(const MCP23017 & copy)
	: m_addr(copy.m_addr)
{
}

MCP23017::~MCP23017()
{
}


// Util
// * * * * * * * * * * * * * * * * //

MCP23017 & MCP23017::clearA()
{
	return setA(0x00);
}

MCP23017 & MCP23017::clearB()
{
	return setB(0x00);
}

MCP23017 & MCP23017::clearAB()
{
	return clearA().clearB();
}


MCP23017 & MCP23017::fillA()
{
	return setA(0xFF);
}

MCP23017 & MCP23017::fillB()
{
	return setB(0xFF);
}

MCP23017 & MCP23017::fillAB()
{
	return fillA().fillB();
}


// Set
// * * * * * * * * * * * * * * * * //


MCP23017 & MCP23017::setA(uint8_t value)
{
	return setReg8(MCP23017::GPIOA, value);
}

MCP23017 & MCP23017::setB(uint8_t value)
{
	return setReg8(MCP23017::GPIOB, value);
}

MCP23017 & MCP23017::setAB(uint16_t value)
{
	return setA((uint8_t)(value & 0xFF)).setB((uint8_t)(((value >> 8) & 0xFF)));
}

MCP23017 & MCP23017::setReg8(uint8_t reg, uint8_t value)
{
	WIRE.beginTransmission(addr());
	WIRE.send(reg);
	WIRE.send(value);
	WIRE.endTransmission();
	return (*this);
}

// Get
// * * * * * * * * * * * * * * * * //

uint8_t MCP23017::getA() const
{
	return getReg8(MCP23017::GPIOA);
}

uint8_t MCP23017::getB() const
{
	return getReg8(MCP23017::GPIOB);
}

uint16_t MCP23017::getAB() const
{
	return (uint16_t)((getA() << 8 ) | (getB() & 0xFF));
}

uint8_t MCP23017::getReg8(uint8_t reg) const
{
	WIRE.beginTransmission(addr());
	WIRE.send(reg);
	WIRE.endTransmission();
	WIRE.requestFrom(addr(), 1);
	return WIRE.read();
}

// * * * * * * * * * * * * * * * * //