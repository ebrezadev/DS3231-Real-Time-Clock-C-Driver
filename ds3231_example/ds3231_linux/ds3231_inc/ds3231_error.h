/**
 * @file ds3231_error.h
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
#ifndef __DS3231_ERROR_H__
#define __DS3231_ERROR_H__

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * @brief The return errors for DS3231 device driver API
	 *
	 */
	typedef enum
	{
		/*no error*/
		DS3231_ERROR_OK = 0,
		/*error in interface initialization*/		 
		DS3231_ERROR_INTERFACE_INIT, 
		/*error in interface deinitialization*/
		DS3231_ERROR_INTERFACE_DEINIT,
		/*error in interface array read*/
		DS3231_ERROR_INTERFACE_READ,
		/*error in interface array write*/
		DS3231_ERROR_INTERFACE_WRITE,
		/*error in interface delay*/
		DS3231_ERROR_INTERFACE_DELAY,
		/*error in oscillator*/
		DS3231_ERROR_OSCILLATOR_STOPPED, 
		/*error in mismatch between alarm rate and day-day config*/
		DS3231_ERROR_ALARM_RATE_AND_DAY_DATE_MISMATCH,
		/*error in connection*/
		DS3231_ERROR_DS3231_NOT_CONNECTED,
#if DS3231_INCLUDE_EXCLUSION_HOOK
		/*error in interface mutex lock*/
		DS3231_ERROR_INTERFACE_MUTEX_LOCK,
		/*error in interface mutex unlock*/
		DS3231_ERROR_INTERFACE_MUTEX_UNLOCK,
#endif
#if DS3231_INCLUDE_NULL_CHECK
		/*error in dependency functions*/
		DS3231_ERROR_NULL_INTERFACE_FUNCTION_POINTER, 
		/*error in null handle pointer*/
		DS3231_ERROR_NULL_HANDLE,
		/*handle to mutex does not exist*/					  
		DS3231_ERROR_NULL_MUTEX_HANDLE,				  
#endif
#if DS3231_INCLUDE_SAFE_RANGE_CHECK
		/*error in second range. must be between 0 and 59*/
		DS3231_ERROR_RANGE_SECOND,
		/*error in minute range. must be between 0 and 59*/
		DS3231_ERROR_RANGE_MINUTE,
		/*error in hour range. must be between 0 and 23*/
		DS3231_ERROR_RANGE_HOUR,
		/*error in day range. must be between 1 and 7*/
		DS3231_ERROR_RANGE_DAY,
		/*error in date range. must be between 1 and 31*/
		DS3231_ERROR_RANGE_DATE,
		/*error in month range. must be between 1 and 12*/
		DS3231_ERROR_RANGE_MONTH,
		/*error in year range. must be between 1900 and 2099*/
		DS3231_ERROR_RANGE_YEAR,
#endif
#if DS3231_INCLUDE_WRITE_VERIFICATION
		/*error in write verification*/
		DS3231_ERROR_VERIFICATION_FAIL,
#endif
#if DS3231_INCLUDE_TEMPERATURE
		/*error in temperature read busy bit timeout*/
		DS3231_ERROR_TEMPERATURE_BUSY_TIMEOUT,
		/*error in temperature read conversion timeout*/
		DS3231_ERROR_TEMPERATURE_CONVERSION_TIMEOUT
#endif
	} ds3231_error_code_t;


#if DS3231_INCLUDE_ERROR_LOG_STRINGS
	/**
	 * @brief The DS3231 log message strings array
	 *
	 */
	static const char *DS3231_ERROR_LOG_STRING[] =
	{
		"OK",	 
		"INTERFACE INIT", 
		"INTERFACE DEINIT",
		"INTERFACE READ",
		"INTERFACE WRITE",
		"INTERFACE DELAY",
		"OSCILLATOR STOPPED", 
		"ALARM RATE AND DAY DATE MISMATCH",
		"DS3231 NOT CONNECTED",
#if DS3231_INCLUDE_EXCLUSION_HOOK
		"INTERFACE MUTEX LOCK",
		"INTERFACE MUTEX UNLOCK",
#endif
#if DS3231_INCLUDE_NULL_CHECK
		"NULL INTERFACE FUNCTION POINTER", 
		"NULL HANDLE",				  
		"NULL MUTEX HANDLE",				  
#endif
#if DS3231_INCLUDE_SAFE_RANGE_CHECK
		"RANGE SECOND",
		"RANGE MINUTE",
		"RANGE HOUR",
		"RANGE DAY",
		"RANGE DATE",
		"RANGE MONTH",
		"RANGE YEAR",
#endif
#if DS3231_INCLUDE_WRITE_VERIFICATION
		"VERIFICATION FAIL",
#endif
#if DS3231_INCLUDE_TEMPERATURE
		"TEMPERATURE BUSY TIMEOUT",
		"TEMPERATURE CONVERSION TIMEOUT"
#endif
	};
#endif

#ifdef __cplusplus
}
#endif

#endif

