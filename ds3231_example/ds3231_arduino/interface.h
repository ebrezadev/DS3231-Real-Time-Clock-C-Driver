#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include <stdint.h>

	int ds3231_interface_init(uint8_t deviceAddress);
	int ds3231_interface_deinit(uint8_t deviceAddress);
	int ds3231_delay_function(uint32_t delayMS);
	int ds3231_write_array(uint8_t deviceAddress, uint8_t startRegisterAddress, uint8_t *data, uint8_t dataLength);
	int ds3231_read_array(uint8_t deviceAddress, uint8_t startRegisterAddress, uint8_t *data, uint8_t dataLength);
	int ds3231_interface_lock(void *mutex_handle);
	int ds3231_interface_unlock(void *mutex_handle);
	int ds3231_interface_ack_test(uint8_t deviceAddress);

#endif
