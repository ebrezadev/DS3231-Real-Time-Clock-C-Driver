/*DS3231 low level API - Reza Ebrahimi v1.0*/
#include "interface.h"
#include <Wire.h>

#define I2C_SPEED 400000

/*function to initialize I2C peripheral in 100khz or 400khz*/
int ds3231_interface_init(uint8_t deviceAddress)
{
	Wire.begin();
	Wire.setClock(I2C_SPEED);

	return 0;
}

int ds3231_interface_deinit(uint8_t deviceAddress)
{
	Wire.end();

	return 0;
}

int ds3231_delay_function(uint32_t delayMS)
{
	delay(delayMS);

	return 0;
}

int ds3231_write_array(uint8_t deviceAddress, uint8_t startRegisterAddress, uint8_t *data, uint8_t dataLength)
{
	Wire.beginTransmission(deviceAddress);
	Wire.write(startRegisterAddress);
	for (uint8_t index = 0; index < dataLength; index++)
	{
		Wire.write(data[index]);
	}
	Wire.endTransmission(deviceAddress);

	return 0;
}

int ds3231_read_array(uint8_t deviceAddress, uint8_t startRegisterAddress, uint8_t *data, uint8_t dataLength)
{

	Wire.beginTransmission(deviceAddress);
	Wire.write(startRegisterAddress);
	Wire.endTransmission(deviceAddress);

	Wire.requestFrom(deviceAddress, dataLength, 0);
	while (Wire.available())
	{
		*data = Wire.read();
		data++;
	}

	return 0;
}

int ds3231_interface_ack_test(uint8_t deviceAddress)
{
	Wire.beginTransmission(deviceAddress);
	Wire.write(0);
	uint8_t error = Wire.endTransmission(deviceAddress);

	return error;
}

int ds3231_interface_lock(void *mutex_handle)
{

	return 0;
}

int ds3231_interface_unlock(void *mutex_handle)
{

	return 0;
}

