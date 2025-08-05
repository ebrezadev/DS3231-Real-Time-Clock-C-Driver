/**
 * @file ds3231_constants.h
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
#ifndef __DS3231_CONSTANTS_H__
#define __DS3231_CONSTANTS_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "ds3231_typedefs.h"
#include "ds3231_config.h"
#include "ds3231_error.h"

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif


/*Constants related to DS3231_MASK_AND_RANGE_LUT array*/
	enum
	{
#if DS3231_INCLUDE_SAFE_RANGE_CHECK
		DS3231_RANGE_MINIMUM_SECOND = 0,
		DS3231_RANGE_MINIMUM_MINUTE = 0,
		DS3231_RANGE_MINIMUM_HOUR = 0,
		DS3231_RANGE_MINIMUM_DAY = 1,
		DS3231_RANGE_MINIMUM_DATE = 1,
		DS3231_RANGE_MINIMUM_MONTH = 1,
		DS3231_RANGE_MINIMUM_YEAR = 1900,
		DS3231_RANGE_MAXIMUM_SECOND = 59,
		DS3231_RANGE_MAXIMUM_MINUTE = 59,
		DS3231_RANGE_MAXIMUM_HOUR = 23,
		DS3231_RANGE_MAXIMUM_DAY = 7,
		DS3231_RANGE_MAXIMUM_DATE = 31,
		DS3231_RANGE_MAXIMUM_MONTH = 12,
		DS3231_RANGE_MAXIMUM_YEAR = 2099,
#endif
		DS3231_MASK_SECOND = 0XFF,
		DS3231_MASK_MINUTE = 0XFF,
		DS3231_MASK_HOUR = 0X3F,
		DS3231_MASK_DAY = 0XFF,
		DS3231_MASK_DATE = 0XFF,
		DS3231_MASK_MONTH = 0X1F,
		DS3231_MASK_YEAR = 0XFF
	};


#if DS3231_INCLUDE_SAFE_RANGE_CHECK
/*An array of structs used in safe range check and mask values*/
static const ds3231_mask_and_range_t DS3231_MASK_AND_RANGE_LUT[7] = {
	{DS3231_MASK_SECOND, DS3231_RANGE_MINIMUM_SECOND, DS3231_RANGE_MAXIMUM_SECOND, DS3231_ERROR_RANGE_SECOND},
	{DS3231_MASK_MINUTE, DS3231_RANGE_MINIMUM_MINUTE, DS3231_RANGE_MAXIMUM_MINUTE, DS3231_ERROR_RANGE_MINUTE},
	{DS3231_MASK_HOUR, DS3231_RANGE_MINIMUM_HOUR, DS3231_RANGE_MAXIMUM_HOUR, DS3231_ERROR_RANGE_HOUR},
	{DS3231_MASK_DAY, DS3231_RANGE_MINIMUM_DAY, DS3231_RANGE_MAXIMUM_DAY, DS3231_ERROR_RANGE_DAY},
	{DS3231_MASK_DATE, DS3231_RANGE_MINIMUM_DATE, DS3231_RANGE_MAXIMUM_DATE, DS3231_ERROR_RANGE_DATE},
	{DS3231_MASK_MONTH, DS3231_RANGE_MINIMUM_MONTH, DS3231_RANGE_MAXIMUM_MONTH, DS3231_ERROR_RANGE_MONTH},
	{DS3231_MASK_YEAR, DS3231_RANGE_MINIMUM_YEAR, DS3231_RANGE_MAXIMUM_YEAR, DS3231_ERROR_RANGE_YEAR}
};
#else
static const ds3231_mask_and_range_t DS3231_MASK_AND_RANGE_LUT[7] = {
	{DS3231_MASK_SECOND},
	{DS3231_MASK_MINUTE},
	{DS3231_MASK_HOUR},
	{DS3231_MASK_DAY},
	{DS3231_MASK_DATE},
	{DS3231_MASK_MONTH},
	{DS3231_MASK_YEAR}
};
#endif


/*An array of default register values used in reset*/
static const uint8_t REGISTER_DEFAULT_VALUE[] = {
	0X00,
	0X00,
	0X00,
	0X01,
	0X01,
	0X01,
	0X00,
	0X00,
	0X00,
	0X00,
	0X01,
	0X00,
	0X00,
	0X01,
	0X1C,
	0X00,
	0X00
};


/*Alarm 1 mask bits for rate selection */
static const uint8_t DS3231_ALARM_1_MASK_BITS[6][5] = {
	{1,1,1,1,0},
	{0,1,1,1,0},
	{0,0,1,1,0},
	{0,0,0,1,0},
	{0,0,0,0,0},
	{0,0,0,0,1}
};


/*Alarm 2 mask bits for rate selection */
static const uint8_t DS3231_ALARM_2_MASK_BITS[5][4] = {
	{1,1,1,0},
	{0,1,1,0},
	{0,0,1,0},
	{0,0,0,0},
	{0,0,0,1}
};


	/*Constant delay value in milliseconds to check OSF bit*/
	static const int DS3231_OSC_FLAG_DELAY_MS = 1000;
	/*OSC Stop Flag = TRUE*/
	static const int DS3231_OSCILLATOR_STOPPED = 1;
	static const int DS3231_NUMBER_OF_TIME_REGISTERS = 7;

#if DS3231_INCLUDE_TEMPERATURE
	static const uint32_t DS3231_TEMPERATURE_READ_DELAY = 5;
	static const uint32_t DS3231_TEMPERATURE_READ_TIMEOUT = 250;
#endif

static const uint8_t DS3231_VERSION_MAJOR = 2;
static const uint8_t DS3231_VERSION_MINOR = 0;

#ifdef __cplusplus
}
#endif

#endif
