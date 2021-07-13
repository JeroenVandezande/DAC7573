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

/// Initializes the instance struct
/// \param instance a pointer to a struct containing the instance information
/// \param A0A1Address the two lowest bits of this byte represent the state of the A0 and A1 address lines on the chip
/// \param referenceVoltage The voltage applied on the reference pin
/// \return Error code 0 = all OK
int DAC7573_Init(struct DAC7573_t* instance, uint8_t A0A1Address, float referenceVoltage);

/// sets the voltage output on a certain channel
/// \param instance a pointer to a struct containing the instance information
/// \param channel a number between 0 and 3 that selects the output channel
/// \param voltage a voltage between 0 and the reference voltage
/// \return Error code 0 = all OK
int DAC7573_SetVoltage(struct DAC7573_t* instance, uint8_t channel, float voltage);

#endif //DAC7573_LIBRARY_H
