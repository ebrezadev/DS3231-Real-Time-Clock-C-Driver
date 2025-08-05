/**
* @file	ds3231_config.h
* @brief DS3231 Real Time Clock C Driver
* @author Reza G. Ebrahimi <https://github.com/ebrezadev>
* @version 2.0
* @Section HOW-TO-USE
* This is the configuration file for DS3231 device driver. There are
* 1 config constants and 11 macros.
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
#ifndef __DS3231_CONFIG_H__
#define __DS3231_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*************************************************************************************/
/*macros*/


/*Feature: turn the value range check on or off*/
#define DS3231_INCLUDE_SAFE_RANGE_CHECK 1
/*Feature: turn the write verification on or off*/
#define DS3231_INCLUDE_WRITE_VERIFICATION 1
/*Feature: turn the ds3231 hardware connection check on or off*/
#define DS3231_INCLUDE_CONNECTION_CHECK 1
/*Feature: turn the NULL interface function pointer check on or off*/
#define DS3231_INCLUDE_NULL_CHECK 1
/*Feature: turn the write verification on or off*/
#define DS3231_INCLUDE_EXCLUSION_HOOK 0
/*Feature: turn the alarm 1 feature on or off*/
#define DS3231_INCLUDE_ALARM_1 0
/*Feature: turn the alarm 2 feature on or off*/
#define DS3231_INCLUDE_ALARM_2 0
/*Feature: turn the temperature sensor feature reading on or off*/
#define DS3231_INCLUDE_TEMPERATURE 1
/*Feature: turn the float temperature on or off*/
#define DS3231_INCLUDE_TEMPERATURE_FLOAT_MATH 1
/*Feature: turn the aging offset calibration on or off*/
#define DS3231_INCLUDE_AGING_OFFSET_CALIBRATION 0
/*Feature: turn the error log strings on or off*/
#define DS3231_INCLUDE_ERROR_LOG_STRINGS 1


/*************************************************************************************/
/*config constants*/


static const uint16_t DS3231_STARTUP_DELAY_IN_MS = 2000;

#ifdef __cplusplus
}
#endif

#endif
