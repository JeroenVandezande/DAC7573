#include "DAC7573.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define BASE_ADDRESS 0b10011000

int DAC7573_Init(struct DAC7573_t* instance, uint8_t A0A1Address, float referenceVoltage, bool autoSanitize)
{
	//apply the A0 and A1 bits to the base address
	instance->I2CAddress = BASE_ADDRESS | (A0A1Address << 1);
	//calculate the voltage multiplier to be used when doing DAC conversions
	instance->_VoltageMultiplier = 4095 / referenceVoltage;
	instance->_RefVoltage = referenceVoltage;
	instance->_AutoSanitize = autoSanitize;
	return 0;
}

int DAC7573_SetVoltage(struct DAC7573_t* instance, uint8_t channel, float voltage)
{
/*
MSB                                 LSB
A3   A2   L1   L0   X   Sel1  Sel0  PD0
                                      
Bit Name        Bit Number/Description
     A3         Extended address bit                     The state of these bits must match the state of pins A3 and A2 in order for a
     A2         Extended address bit                     proper DAC7573 data update, except in broadcast update mode.
     L1         Load1 (mode select) bit                  Are used for selecting the update mode.
     L2         Load0 (mode select) bit
                    00           Store I2C data. The contents of MS-BYTE and LS-BYTE (or power down information) are stored in the
                                 temporary register of a selected channel. This mode does not change the DAC output of the selected
                                 channel.
                    01           Update selected DAC with I2C data. Most commonly utilized mode. The contents of MS-BYTE and
                                 LS-BYTE (or power down information) are stored in the temporary register and in the DAC register of
                                 the selected channel. This mode changes the DAC output of the selected channel with the new data.
                    10           4-Channel synchronous update. The contents of MS-BYTE and LS-BYTE (or power down information)
                                 are stored in the temporary register and in the DAC register of the selected channel. Simultaneously,
                                 the other three channels get updated with previously stored data from the temporary register. This
                                 mode updates all four channels together.
                    11           Broadcast update mode. This mode has two functions. In broadcast mode, DAC7573 responds
                                 regardless of local address matching, and channel selection becomes irrelevant as all channels update.
                                 This mode is intended to enable up to 64 channels simultaneous update, if used with the I2C broadcast
                                 address (1001 0000).
                                 If Sel1=0               All four channels are updated with the contents of their temporary register
                                                         data.
                                 If Sel1=1               All four channels are updated with the MS-BYTE and LS-BYTE data or
                                                         powerdown.
     Sel1       Buff Sel1 Bit                            Channel select bits
     Sel0       Buff Sel0 Bit
                    00           Channel A
                    01           Channel B
                    10           Channel C
                    11           Channel D
     PD0        Power Down Flag
                    0            Normal operation
                    1            Power-down flag
*/
	uint8_t controlregister = 0b00010000; //Mode 01 : Update selected DAC with I2C data, No Power Down
	controlregister |= (channel << 1);

	// Sanitize Voltage value
	if (voltage > instance->_RefVoltage)
	{
		if(!instance->_AutoSanitize)
		{
			return 1;
		}
		voltage = instance->_RefVoltage;
	}
	if (voltage < 0)
	{
		if(!instance->_AutoSanitize)
		{
			return 2;
		}
		voltage = 0;
	}
	// Calculate DAC value from voltage
	uint16_t dacval = (uint16_t)round(voltage * instance->_VoltageMultiplier);
	// Sanitize DA value
	if (dacval > 4095)
	{
		if(!instance->_AutoSanitize)
		{
			return 3;
		}
		dacval = 4095;
	}
	//Fill data array
	uint8_t tempArray[] =
	  {
		controlregister,
		(uint8_t)(dacval >> 4), //consists of eight most significant bits of 12-bit unsigned binary D/A conversion data
		(uint8_t)(dacval << 4)  //consists of the 4 least significant bits of the 12-bit unsigned binary D/A conversion data, followed by 4 don't care bits
	  };
	//Pass data to transmit method
	if(!(instance->i2cWriteDataMethod(instance->I2CAddress, &tempArray[0], 3)))
	{
		return 4;
	}
	return 0;
}