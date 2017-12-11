#include "mbed.h"
#include "ShiftRegisterControlClass.h"

#define MY_DEBUG 0

ShiftRegisterControlClass::ShiftRegisterControlClass
(PinName data, PinName store, PinName clock) : _ds(data), _st(store), _sh(clock)
{
	mask_byte_ = 0x00;//mask_byte zero setting
}


void ShiftRegisterControlClass::ShiftByte(int8_t  data, BitOrd  ord)
{
	uint8_t mask;

	if (ord == MSBFirst) mask = 0x80;
	else                 mask = 0x01;

	for (int i = 0; i < 8; i++) {
		if (data & mask) _ds = 1;
		else             _ds = 0;

#if MY_DEBUG > 0
		printf("%d ", _ds.read());
#endif /* MY_DEBUG */
		if (ord == MSBFirst) mask = mask >> 1;
		else                 mask = mask << 1;

		_sh = 0;
		_sh = 1;
	}

#if MY_DEBUG > 0
	printf("\n");
#endif /* MY_DEBUG */

}

void ShiftRegisterControlClass::ShiftBit(int8_t  data)
{
	_ds = data;
	_sh = 0;
	_sh = 1;
}

void ShiftRegisterControlClass::Latch()
{
	_st = 1;
	_st = 0;
}

void ShiftRegisterControlClass::TurnOnLed(int8_t data)
{
	mask_byte_ = mask_byte_ | data; // write on mask byte
	ShiftByte(mask_byte_, ShiftRegisterControlClass::MSBFirst);
	Latch();
}

void ShiftRegisterControlClass::TurnOffLed(int8_t data)
{
	mask_byte_ = mask_byte_ ^ data; // write on mask byte
	ShiftByte(mask_byte_, ShiftRegisterControlClass::MSBFirst);
	Latch();
}

void ShiftRegisterControlClass::BlinkLed()
{
	for (int i = 0; i<3; i++) {
		LightAllLed();
		wait(0.5);
		BlackOutLed();
		wait(0.5);
	}
}

void ShiftRegisterControlClass::LightAllLed()
{
	// don't write mask byte
	ShiftByte(ShiftRegisterControlClass::kAllOnLedByte, ShiftRegisterControlClass::MSBFirst);
	Latch();
}


void ShiftRegisterControlClass::BlackOutLed()
{
	// don't write mask byte
	ShiftByte(ShiftRegisterControlClass::kAllOffLedByte, ShiftRegisterControlClass::MSBFirst);
	Latch();
}

int8_t ShiftRegisterControlClass::get_my_mask_byte()
{
	return mask_byte_;//return mask byte
}
