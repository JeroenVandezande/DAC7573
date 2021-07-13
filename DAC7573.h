// This is the Texas Instruments DAC7375 DA-Convertor C code

#ifndef DAC7573_LIBRARY_H
#define DAC7573_LIBRARY_H

#include <stdint.h>
#include <stdbool.h>
#include "LL_i2c.h"


struct DAC7573_t
{
	//INTERNAL VARIABLES, DO NOT MODIFY
	float _VoltageMultiplier;
	//PUBLIC INSTANCE MEMBERS
	uint8_t I2CAddress;
	//points to a function to handle the I2C writes
	I2CMaster_WriteMethod_t i2cWriteDataMethod;
};

///
/// \param instance a pointer to a struct containing the instance information
/// \param A0A1Address the two lowest bits of this byte represent the state of the A0 and A1 address lines on the chip
/// \param referenceVoltage The voltage applied on the reference pin
/// \return Error code 0 = all OK
int InitDAC7573(struct DAC7573_t* instance, uint8_t A0A1Address, float referenceVoltage);
int SetDAC7573Voltage(struct DAC7573_t* instance, uint8_t channel, float voltage);

#endif //DAC7573_LIBRARY_H
