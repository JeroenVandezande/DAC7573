#include "DAC7573.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define BASE_ADDRESS 0b10011000

int DAC7573_Init(struct DAC7573_t* instance, uint8_t A0A1Address, float referenceVoltage)
{
	//apply the A0 and A1 bits to the base address
	instance->I2CAddress = BASE_ADDRESS | (A0A1Address << 1);
	//calculate the voltage multiplier to be used when doing DAC conversions
	instance->_VoltageMultiplier = 4095 / referenceVoltage;
	return 0;
}

int DAC7573_SetVoltage(struct DAC7573_t* instance, uint8_t channel, float voltage)
{
	uint16_t dacval = (uint16_t)round(voltage * instance->_VoltageMultiplier);
	uint8_t controlregister = 0b00010000;
	controlregister = controlregister | (channel << 1);
	uint8_t tempArray[] =
	  {
		controlregister,
		(uint8_t)(dacval >> 4), //consists of eight most significant bits of 12-bit unsigned binary D/A conversion data
		(uint8_t)(dacval << 4)  //consists of the 4 least significant bits of the 12-bit unsigned binary D/A conversion data, followed by 4 don't care bits
	  };
	instance->i2cWriteDataMethod(instance->I2CAddress, tempArray, 3);
	return 0;
}