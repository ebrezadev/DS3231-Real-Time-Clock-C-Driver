/**
 * @file ds3231_typedefs.h
 * @brief DS3231 Real Time Clock C Driver
 * @author Reza G. Ebrahimi <https://github.com/ebrezadev>
 * @version 2.0
 * @license MIT
 *
 * MIT License
 *
 * Copyright (c) 2025 Reza G. Ebrahimi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#ifndef __DS3231_DEFINITIONS_H__
#define __DS3231_DEFINITIONS_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "ds3231_config.h"
#include "ds3231_error.h"

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif


/************************************************************************************************************ */


	/**
	 * @brief Boolean data type for DS3231.
	 *
	 */
	typedef enum
	{
		DS3231_FALSE = 0,
		DS3231_TRUE = 1
	} ds3231_bool_t;


	/**
	 * @brief Register addresses for DS3231
	 *
	 */
	typedef enum
	{
		DS3231_REGISTER_SECONDS = 0x00,
		DS3231_REGISTER_MINUTES = 0x01,
		DS3231_REGISTER_HOURS = 0x02,
		DS3231_REGISTER_DAY_OF_WEEK = 0x03,
		DS3231_REGISTER_DATE = 0x04,
		DS3231_REGISTER_MONTH = 0x05,
		DS3231_REGISTER_YEAR = 0x06,
		DS3231_REGISTER_ALARM1_SECONDS = 0x07,
		DS3231_REGISTER_ALARM1_MINUTES = 0x08,
		DS3231_REGISTER_ALARM1_HOURS = 0x09,
		DS3231_REGISTER_ALARM1_DAY_OF_WEEK_OR_DATE = 0x0a,
		DS3231_REGISTER_ALARM2_MINUTES = 0x0b,
		DS3231_REGISTER_ALARM2_HOURS = 0x0c,
		DS3231_REGISTER_ALARM2_DAY_OF_WEEK_OR_DATE = 0x0d,
		DS3231_REGISTER_CONTROL = 0x0e,
		DS3231_REGISTER_CONTROL_STATUS = 0x0f,
		DS3231_REGISTER_AGING_OFFSET = 0x10,
		DS3231_REGISTER_TEMP_MSB = 0x11,
		DS3231_REGISTER_TEMP_LSB = 0x12
	} ds3231_register_address_t;


	/**
	 * @brief Collection of time and calendar register addresses.
	 *
	 */
	typedef enum
	{
		DS3231_SECONDS = 0x00,
		DS3231_MINUTES = 0x01,
		DS3231_HOURS = 0x02,
		DS3231_DAY = 0x03,
		DS3231_DATE = 0x04,
		DS3231_MONTH = 0x05,
		DS3231_YEAR = 0x06
	} ds3231_time_register_t;


	/**
	 * @brief Bit addresses in DS3231.
	 *
	 */
	typedef enum
	{
		DS3231_BIT_12_24 = 0X06,
		DS3231_BIT_CENTURY = 0X07,
		DS3231_BIT_A1M1 = 0X07,
		DS3231_BIT_A1M2 = 0X07,
		DS3231_BIT_A1M3 = 0X07,
		DS3231_BIT_A1M4 = 0X07,
		DS3231_BIT_A2M2 = 0X07,
		DS3231_BIT_A2M3 = 0X07,
		DS3231_BIT_A2M4 = 0X07,
		DS3231_BIT_12_24_ALARM1 = 0X06,
		DS3231_BIT_12_24_ALARM2 = 0X06,
		DS3231_BIT_DY_DT_ALARM1 = 0X06,
		DS3231_BIT_DY_DT_ALARM2 = 0X06,
		DS3231_BIT_A1IE = 0X00,
		DS3231_BIT_A2IE = 0X01,
		DS3231_BIT_INTCN = 0X02,
		DS3231_BIT_RS1 = 0X03,
		DS3231_BIT_RS2 = 0X04,
		DS3231_BIT_CONV = 0X05,
		DS3231_BIT_BBSQW = 0X06,
		DS3231_BIT_EOSC = 0X07,
		DS3231_BIT_A1F = 0X00, /*Alarm 1 Flag*/
		DS3231_BIT_A2F = 0X01, /*Alarm 2 Flag*/
		DS3231_BIT_BSY = 0X02,
		DS3231_BIT_EN32KHZ = 0X03,
		DS3231_BIT_OSF = 0X07, /*Oscillator Stop Flag*/
	} ds3231_register_bit_t;


	/**
	 * @brief Days of week in DS3231.
	 *
	 */
	typedef enum
	{
		DS3231_DAY_MONDAY = 0X01,
		DS3231_DAY_TUESDAY = 0X02,
		DS3231_DAY_WEDNESDAY = 0X03,
		DS3231_DAY_THURSDAY = 0X04,
		DS3231_DAY_FRIDAY = 0X05,
		DS3231_DAY_SATURDAY = 0X06,
		DS3231_DAY_SUNDAY = 0X07,
	} ds3231_day_t;


	/**
	 * @brief Months in DS3231.
	 *
	 */
	typedef enum
	{
		DS3231_MONTH_JANUARY = 0X01,
		DS3231_MONTH_FEBRUARY = 0X02,
		DS3231_MONTH_MARCH = 0X03,
		DS3231_MONTH_APRIL = 0X04,
		DS3231_MONTH_MAY = 0X05,
		DS3231_MONTH_JUNE = 0X06,
		DS3231_MONTH_JULY = 0X07,
		DS3231_MONTH_AUGUST = 0X08,
		DS3231_MONTH_SEPTEMBER = 0X09,
		DS3231_MONTH_OCTOBER = 0X0A,
		DS3231_MONTH_NOVEMBER = 0X0B,
		DS3231_MONTH_DECEMBER = 0X0C
	} ds3231_month_t;


	/**
	 * @brief SQW pin squarewave output frequency.
	 *
	 */
	typedef enum
	{
		DS3231_SQW_WAVE_1HZ = 0,
		DS3231_SQW_WAVE_1024HZ,
		DS3231_SQW_WAVE_4096HZ,
		DS3231_SQW_WAVE_8192HZ
	} ds3231_sqw_output_wave_frequency_t;


	/**
	 * @brief SQW/INT output pin selection.
	 *
	 */
	typedef enum
	{
		DS3231_PIN_SQUAREWAVE,
		DS3231_PIN_INTERRUPT
	} ds3231_int_sqw_pin_t;


	/**
	 * @brief Seconds data type. Range: 0 - 59.
	 *
	 */
	typedef uint16_t ds3231_second_t;


	/**
	 * @brief Minutes data type. Range: 0 - 59.
	 *
	 */
	typedef uint16_t ds3231_minute_t;


	/**
	 * @brief Hours data type. Range: 0 - 23.
	 *
	 */
	typedef uint16_t ds3231_hour_t;


	/**
	 * @brief Date data type. Range: 1 - 31.
	 *
	 */
	typedef uint16_t ds3231_date_t;


	/**
	 * @brief Years data type. Range: 1900 - 2099.
	 *
	 */
	typedef uint16_t ds3231_year_t;


	/**
	 * @brief Time and calendar data type.
	 *
	 */
	typedef struct
	{
		ds3231_second_t second;
		ds3231_minute_t minute;
		ds3231_hour_t hour;
		ds3231_day_t day;
		ds3231_date_t date;
		ds3231_month_t month;
		ds3231_year_t year;
	} ds3231_time_and_calendar_t;


#if DS3231_INCLUDE_SAFE_RANGE_CHECK
	/**
	 * @brief Mask and range data type. Consists of register mask, min range, max range and range error.
	 *
	 */
	typedef struct
	{
		uint8_t mask;
		uint16_t range_min;
		uint16_t range_max;
		ds3231_error_code_t error;
	} ds3231_mask_and_range_t;
#else
	typedef struct
	{
		uint8_t mask;
	} ds3231_mask_and_range_t;
#endif


#if DS3231_INCLUDE_ALARM_1 | DS3231_INCLUDE_ALARM_2
	/**
	 * @brief Alarm day or date selection data type.
	 *
	 */
	typedef enum
	{
		DS3231_ALARM_DAY,
		DS3231_ALARM_DATE
	} ds3231_alarm_day_date_select_t;
#endif


#if DS3231_INCLUDE_ALARM_1
	/**
	 * @brief Alarm 1 rate selection data type.
	 *
	 */
	typedef enum
	{
		DS3231_ALARM1_ONCE_PER_SECOND = 0,
		DS3231_ALARM1_MATCH_SECOND,
		DS3231_ALARM1_MATCH_SECOND_MINUTE,
		DS3231_ALARM1_MATCH_SECOND_MINUTE_HOUR,
		DS3231_ALARM1_MATCH_SECOND_MINUTE_HOUR_DATE,
		DS3231_ALARM1_MATCH_SECOND_MINUTE_HOUR_DAY
	} ds3231_alarm_1_rate_t;


	/**
	 * @brief Alarm 1 config data type.
	 *
	 */
	typedef struct
	{
		ds3231_second_t second;
		ds3231_minute_t minute;
		ds3231_hour_t hour;
		union
		{
			ds3231_day_t day;
			ds3231_date_t date;
		} day_date;
		ds3231_alarm_day_date_select_t day_date_type;
		ds3231_alarm_1_rate_t alarm_rate;
	} ds3231_alarm_1_config_t;
#endif


#if DS3231_INCLUDE_ALARM_2
	/**
	 * @brief Alarm 2 rate selection data type.
	 *
	 */
	typedef enum
	{
		DS3231_ALARM2_ONCE_PER_MINUTE = 0,
		DS3231_ALARM2_MATCH_MINUTE,
		DS3231_ALARM2_MATCH_MINUTE_HOUR,
		DS3231_ALARM2_MATCH_MINUTE_HOUR_DATE,
		DS3231_ALARM2_MATCH_MINUTE_HOUR_DAY
	} ds3231_alarm_2_rate_t;


	/**
	 * @brief Alarm 2 config data type.
	 *
	 */
	typedef struct
	{
		ds3231_minute_t minute;
		ds3231_hour_t hour;
		union
		{
			ds3231_day_t day;
			ds3231_date_t date;
		} day_date;
		ds3231_alarm_day_date_select_t day_date_type;
		ds3231_alarm_2_rate_t alarm_rate;
	} ds3231_alarm_2_config_t;
#endif


	/**
	 * @brief I2C address for DS3231. Use I2C_ADDRESS_NONE in case of software mock.
	 *
	 */
	typedef enum
	{
		DS3231_I2C_ADDRESS_NONE = 0,
		DS3231_I2C_ADDRESS = 0X68,
	} ds3231_i2c_address_t;


/*************************************************************************************************************/
	/*Interface dependency pointers*/


	/**
	 * @brief The interface initializer
	 *
	 * Implements the interface (or optionally chip power) initializer, whether I2c or test mock.
	 *
	 * @param deviceAddress: The I2C device address
	 * @return Returns 0 for no error
	 *
	 */
	typedef int (*ds3231_interface_init_fp)(uint8_t deviceAddress);


	/**
	 * @brief The interface de-initializer
	 *
	 * Implements the interface (or optionally chip power) de-initializer, whether I2c or test mock.
	 *
	 * @param deviceAddress: The I2C device address
	 * @return Returns 0 for no error
	 *
	 */
	typedef int (*ds3231_interface_deinit_fp)(uint8_t deviceAddress);


	/**
	 * @brief The delay function
	 *
	 * Implements a delay function in milliseconds.
	 *
	 * @param delayMS: Delay in milliseconds
	 * @return Returns 0 for no error
	 *
	 */
	typedef int (*ds3231_delay_function_fp)(uint32_t delayMS);


	/**
	 * @brief The write array function
	 *
	 * Implements the interface write function
	 *
	 * @param deviceAddress: I2C interface address
	 * @param startRegisterAddress: The address of starting register
	 * @param data: Pointer to the array of data
	 * @param dataLength: Length of data array
	 * @return Returns 0 for no error
	 *
	 */
	typedef int (*ds3231_write_array_fp)(uint8_t deviceAddress, uint8_t startRegisterAddress, uint8_t *data, uint8_t dataLength);


	/**
	 * @brief The read array function
	 *
	 * Implements the interface read function
	 *
	 * @param deviceAddress: I2C interface address
	 * @param startRegisterAddress: The address of starting register
	 * @param data: Pointer to the array of data
	 * @param dataLength: Length of data array
	 * @return Returns 0 for no error
	 *
	 */
	typedef int (*ds3231_read_array_fp)(uint8_t deviceAddress, uint8_t startRegisterAddress, uint8_t *data, uint8_t dataLength);


#if DS3231_INCLUDE_EXCLUSION_HOOK
	/**
	 * @brief The lock hook
	 *
	 * Implements the interface mutex lock function
	 *
	 * @param mutex_handle: The handle to the mutex
	 * @return Returns 0 for no error
	 *
	 */
	typedef int (*ds3231_interface_lock_fp)(void *mutex_handle);


	/**
	 * @brief The unlock hook
	 *
	 * Implements the interface mutex unlock function
	 *
	 * @param mutex_handle: The handle to the mutex
	 * @return Returns 0 for no error
	 *
	 */
	typedef int (*ds3231_interface_unlock_fp)(void *mutex_handle);


	/**
	 * @brief The interface mutual exclusion hooks datatype
	 *
	 * A struct of mutex lock and unlock hooks
	 *
	 */
	typedef struct
	{
		ds3231_interface_lock_fp interface_lock;
		ds3231_interface_unlock_fp interface_unlock;
		void *mutex_handle;
	} ds3231_interface_exclusion_t;
#endif


#if DS3231_INCLUDE_CONNECTION_CHECK
	/**
	 * @brief The ACK test hook
	 *
	 * Implements the interface ACK test function
	 *
	 * @param deviceAddress: The I2C device address
	 * @return Returns 0 for no error
	 *
	 */
	typedef int (*ds3231_interface_ack_test_fp)(uint8_t deviceAddress);
#endif


	/**
	 * @brief The dependency interface structure
	 *
	 * Please define your interface functions and point these function-pointers to them.
	 *
	 */
	typedef struct
	{
		ds3231_interface_init_fp interface_init;
		ds3231_interface_deinit_fp interface_deinit;
		ds3231_delay_function_fp delay_function;
		ds3231_write_array_fp write_array;
		ds3231_read_array_fp read_array;
#if DS3231_INCLUDE_CONNECTION_CHECK
		ds3231_interface_ack_test_fp interface_ack_test;
#endif
#if DS3231_INCLUDE_EXCLUSION_HOOK
		ds3231_interface_exclusion_t interface_exclusion;
#endif
	} ds3231_interface_t;


	/**
	 * @brief The handle to DS3231 instance
	 *
	 * The handle to an instance of DS3231 RTC module. Please set the correct dependency interface.
	 *
	 */
	typedef struct
	{
		ds3231_i2c_address_t i2c_address;
		ds3231_interface_t interface;
	} ds3231_handle_t;


#ifdef __cplusplus
}
#endif

#endif
