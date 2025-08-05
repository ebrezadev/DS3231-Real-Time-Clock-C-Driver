/**
 * @file  ds3231_macros.h
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
#ifndef __DS3231_MACROS_H__
#define __DS3231_MACROS_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "ds3231.h"

/*Used to assert errors*/
#define DS3231_CHECK_AND_RETURN_ERROR(error) \
	do                                       \
	{                                        \
		if (error != DS3231_ERROR_OK)        \
		{                                    \
			return error;                    \
		}                                    \
	} while (0)


#if DS3231_INCLUDE_SAFE_RANGE_CHECK
/*Check the value range for safety*/
#define DS3231_RANGE_ERROR(value, index)                                                                                  \
	do                                                                                                                    \
	{                                                                                                                     \
		if ((value < DS3231_MASK_AND_RANGE_LUT[index].range_min) || (value > DS3231_MASK_AND_RANGE_LUT[index].range_max)) \
		{                                                                                                                 \
			return DS3231_MASK_AND_RANGE_LUT[index].error;                                                                \
		}                                                                                                                 \
	} while (0)
#else
#define DS3231_RANGE_ERROR(value, index) ;
#endif


#if DS3231_INCLUDE_CONNECTION_CHECK
/*Check the ds3231 connected to i2c bus*/
#define DS3231_CONNECTION_CHECK(handle)                                         \
	do                                                                                 \
	{                                                                                  \
		if (handle->interface.interface_ack_test((uint8_t)(handle->i2c_address)) != 0) \
		{                                                                              \
			return DS3231_ERROR_DS3231_NOT_CONNECTED;                                  \
		}                                                                              \
	} while (0)
#else
#define DS3231_CONNECTION_CHECK(handle) ;
#endif


#if DS3231_INCLUDE_EXCLUSION_HOOK
/*Defining mutual exclusion lock and unlock*/
#define DS3231_LOCK(handle)                                                                                                        \
	do                                                                                                                             \
	{                                                                                                                              \
		if (handle->interface.interface_exclusion.interface_lock((void *)handle->interface.interface_exclusion.mutex_handle) != 0) \
		{                                                                                                                          \
			return DS3231_ERROR_INTERFACE_MUTEX_LOCK;                                                                              \
		}                                                                                                                          \
	} while (0)
#define DS3231_UNLOCK(handle)                                                                                                        \
	do                                                                                                                               \
	{                                                                                                                                \
		if (handle->interface.interface_exclusion.interface_unlock((void *)handle->interface.interface_exclusion.mutex_handle) != 0) \
		{                                                                                                                            \
			return DS3231_ERROR_INTERFACE_MUTEX_UNLOCK;                                                                              \
		}                                                                                                                            \
	} while (0)
#else
#define DS3231_LOCK(handle) ;
#define DS3231_UNLOCK(handle) ;
#endif


#if DS3231_INCLUDE_NULL_CHECK
/*Checking for NULL pointers*/
#define DS3231_NULL_CHECK_MACRO(handle, error) \
	do                                         \
	{                                          \
		error = _ds3231_null_check(handle);     \
		DS3231_CHECK_AND_RETURN_ERROR(error);  \
	} while (0)
#else
#define DS3231_NULL_CHECK_MACRO(handle, error) ;
#endif


#if DS3231_INCLUDE_WRITE_VERIFICATION
/*Verify the written bit or byte*/
#define DS3231_VERIFY_BIT(handle, error, register_address, bit_address, expected)                 \
	do                                                                                            \
	{                                                                                             \
		error = _ds3231_write_verify_bit((handle), (register_address), (bit_address), (expected)); \
		DS3231_CHECK_AND_RETURN_ERROR(error);                                                     \
	} while (0)
#define DS3231_VERIFY_BYTES(handle, error, register_address, expected, number_of_bytes)                 \
	do                                                                                                  \
	{                                                                                                   \
		error = _ds3231_write_verify_bytes((handle), (register_address), (expected), (number_of_bytes)); \
		DS3231_CHECK_AND_RETURN_ERROR(error);                                                           \
	} while (0)
#else
#define DS3231_VERIFY_BIT(handle, error, register_address, bit_address, expected) ;
#define DS3231_VERIFY_BYTES(handle, error, register_address, expected, number_of_bytes) ;
#endif

#ifdef __cplusplus
}
#endif

#endif

