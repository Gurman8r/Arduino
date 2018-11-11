/* InputState.cpp
*	Author: 	Melody Gurman
* 	Modified: 	11/11/2018
* * * * * * * * * * * * * * * */
#include <InputState.h>

InputState::InputState() 
	: m_new(NULL)
	, m_old(NULL)
	, m_map(NULL)
	, m_pud(NULL)
	, m_size(0) 
{
}

InputState::InputState(const InputState& copy) 
	: m_new(NULL)
	, m_old(NULL) 
	, m_map(NULL)
	, m_pud(NULL)
	, m_size(copy.m_size) 
{
	memcpy(m_new, copy.m_new, m_size);
	memcpy(m_old, copy.m_old, m_size);
	memcpy(m_map, copy.m_map, m_size);
	memcpy(m_pud, copy.m_pud, m_size);
}

InputState::~InputState() 
{
	if(m_new) {
		delete[] m_new;
	}
	if(m_old) {
		delete[] m_old;
	}
	if(m_map) {
		delete[] m_map;
	}
	if(m_pud) {
		delete[] m_pud;
	}
}


bool InputState::init(uint8_t size)
{
	if((m_size = size))
	{
		m_new = static_cast<uint8_t*>(calloc(m_size, sizeof(uint8_t)));
		m_old = static_cast<uint8_t*>(calloc(m_size, sizeof(uint8_t)));
		m_map = static_cast<uint8_t*>(calloc(m_size, sizeof(uint8_t)));
		m_pud = static_cast<uint8_t*>(calloc(m_size, sizeof(uint8_t)));
		return true;
	}
	return false;
}


InputState & InputState::begin()
{
	for(uint8_t i = 0; i < m_size; i++)
	{
		const bool value = digitalRead(m_map[i]);
		
		m_new[i] = m_pud[i] ? !value : value; // invert value if INPUT_PULLUP
	}
	return (*this);
}

InputState & InputState::end()
{
	memcpy(m_old, m_new, m_size);
	return (*this);
}

InputState & InputState::set(uint8_t id, uint8_t phys, uint8_t pud)
{
	if(id < m_size) {
		pinMode(phys, INPUT_PULLUP);
		m_map[id] = phys;
		m_pud[id] = pud;
	}
	return (*this);
}


bool InputState::getPin(uint8_t pin) const
{
	return (pin < m_size) && (m_new[pin]);
}

bool InputState::getPinDown(uint8_t pin) const
{
	return (pin < m_size) && (m_new[pin] && !m_old[pin]);
}

bool InputState::getPinUp(uint8_t pin) const
{
	return (pin < m_size) && (!m_new[pin] && m_old[pin]);
}


