#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <string.h>

/*Default bus address in case of no env variable. Works with RPi.*/
static const char *default_i2c_bus_address = "/dev/i2c-1";

int ds3231_interface_init(uint8_t deviceAddress);
int ds3231_interface_deinit(uint8_t deviceAddress);
int ds3231_delay_function(uint32_t delayMS);
int ds3231_write_array(uint8_t deviceAddress, uint8_t startRegisterAddress, uint8_t *data, uint8_t dataLength);
int ds3231_read_array(uint8_t deviceAddress, uint8_t startRegisterAddress, uint8_t *data, uint8_t dataLength);
int ds3231_interface_ack_test(uint8_t deviceAddress);

#endif
