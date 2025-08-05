#include "interface.h"

int DS3231_file_descriptor;
const char* bus_address;

/*initiates the I2C peripheral and sets its speed*/
int ds3231_interface_init(uint8_t deviceAddress)
{
	bus_address = getenv("I2C_DEV_PATH");
	if(bus_address == NULL)
	{
		bus_address = default_i2c_bus_address;
	}

	DS3231_file_descriptor = open(bus_address, O_RDWR);
	if(DS3231_file_descriptor < 0)
	{
		perror("ERROR OPENING I2C BUS");
		return 1;
	}

	if(ioctl(DS3231_file_descriptor, I2C_SLAVE, deviceAddress) < 0)
	{
		perror("ERROR IN ACQUIRING BUS ACCESS");
		close(DS3231_file_descriptor);
		return 2;
	}

	return 0;
}

/*initiates the I2C peripheral and sets its speed*/
int ds3231_interface_deinit(uint8_t deviceAddress)
{
	close(DS3231_file_descriptor);

	return 0;
}


/*writes an array (data[]) of arbitrary size (dataLength) to I2C address (deviceAddress), starting from an internal register address (startRegisterAddress)*/
int ds3231_write_array(uint8_t deviceAddress, uint8_t startRegisterAddress, uint8_t *data, uint8_t dataLength)
{
	uint8_t buffer[dataLength + 1];
	buffer[0] = startRegisterAddress;
	memcpy(&buffer[1], data, dataLength);

	if(write(DS3231_file_descriptor, buffer, dataLength + 1) != dataLength + 1)
	{
		perror("ERROR IN I2C WRITE");
		close(DS3231_file_descriptor);
		return 1;
	}

	return 0;
}

/*reads an array (data[]) of arbitrary size (dataLength) from I2C address (deviceAddress), starting from an internal register address (startRegisterAddress)*/
int ds3231_read_array(uint8_t deviceAddress, uint8_t startRegisterAddress, uint8_t *data, uint8_t dataLength)
{
	if(write(DS3231_file_descriptor, (void *)&startRegisterAddress, 1) != 1)
	{
		perror("ERROR IN I2C WRITE");
		close(DS3231_file_descriptor);
		return 1;
	}

	if(read(DS3231_file_descriptor, (void *)data, dataLength) != dataLength)
	{
		perror("ERROR IN I2C READ");
		close(DS3231_file_descriptor);
		return 2;
	}

	return 0;
}

int ds3231_interface_ack_test(uint8_t deviceAddress)
{
  uint8_t data = 0;
  
  if(write(DS3231_file_descriptor, &data, 1) != 1)
	{
		perror("ERROR IN I2C WRITE");
		close(DS3231_file_descriptor);
		return 1;
	}

	if(read(DS3231_file_descriptor, &data, 1) != 1)
	{
		perror("ERROR IN I2C READ");
		close(DS3231_file_descriptor);
		return 2;
	}
  
  return 0;
}
	
/*a delay function for milliseconds delay*/
int ds3231_delay_function(uint32_t delayMS)
{
	if(usleep(1000 * delayMS) != 0)
	{
		return 1;
	}

	return 0;
}

