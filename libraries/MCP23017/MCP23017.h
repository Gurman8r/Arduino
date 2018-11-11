#ifndef _MCP23017_H_
#define _MCP23017_H_

#include <inttypes.h>

class MCP23017
{
public:
	enum
	{
		IODIRA		= 0x00, //	A | IO Direction A
		IODIRB		= 0x01, //	B | IO Direction B
		IPOLA		= 0x02, //	A | Input Polarity A
		IPOLB		= 0x03, //	B | Input Polarity B
		GPINTENA	= 0x04, //	A | Interrupt Enable A
		GPINTENB	= 0x05, //	B | Interrupt Enable B
		DEFVALA		= 0x06, //	A | Default Value A
		DEFVALB		= 0x07, //	B | Default Value B
		INTCONA		= 0x08, //	A | Interrupt Condition A
		INTCONB		= 0x09, //	B | Interrupt Condition	B
		IOCONA		= 0x0A, //	A | IO Config A
		IOCONB		= 0x0B, //	B | IO Config B
		GPPUA		= 0x0C, //	A | Pullup A
		GPPUB		= 0x0D, //	B | Pullup B
		INTFA		= 0x0E, //	A | Interrupt Flag A
		INTFB		= 0x0F, //	B | Interrupt Flag B
		INTCAPA		= 0x10, //	A | Interrupt Data Capture A
		INTCAPB		= 0x11, //	B | Interrupt Data Capture B
		GPIOA		= 0x12, //	A | General Purpose IO A
		GPIOB		= 0x13, //	B | General Purpose IO B
		OLATA		= 0x14, //	A | Output Latch A
		OLATB		= 0x15, //	B | Output Latch B			
		MAX_REGISTER
	};
public:
	// Constructors/Destructor
	// * * * * * * * * * * * * * * * * //
	
	MCP23017();
	MCP23017(uint8_t addr);
	MCP23017(const MCP23017& copy);
	~MCP23017();
	
	// Util
	// * * * * * * * * * * * * * * * * //

	MCP23017& clearA();
	MCP23017& clearB();
	MCP23017& clearAB();
	
	MCP23017& fillA();
	MCP23017& fillB();
	MCP23017& fillAB();
	
	// Set
	// * * * * * * * * * * * * * * * * //

	MCP23017& setA(uint8_t value);
	MCP23017& setB(uint8_t value);
	MCP23017& setAB(uint16_t value);
	MCP23017& setReg8(uint8_t reg, uint8_t value);
	
	// Get
	// * * * * * * * * * * * * * * * * //

	uint8_t		getReg8(uint8_t reg) const;
	uint8_t 	getA() const;
	uint8_t 	getB() const;
	uint16_t	getAB() const;
	
	
	// Properties
	// * * * * * * * * * * * * * * * * //
	
	inline const uint8_t & addr() const { return m_addr; };

private:
	uint8_t m_addr;
};

#endif // !_MCP23017_H_
