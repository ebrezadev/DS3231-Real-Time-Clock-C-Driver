/**
 * @file ds3231_utils.c
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
#include "ds3231.h"
#include "ds3231_macros.h"

/********************************************************/
/********************************************************/
ds3231_error_code_t _ds3231_bit_get(
	const ds3231_handle_t *handle,
	const ds3231_register_address_t register_address,
	const ds3231_register_bit_t register_bit,
	ds3231_bool_t *bit_stat)
{
	uint8_t register_data;

	DS3231_LOCK(handle);
	if (handle->interface.read_array((uint8_t)handle->i2c_address, (uint8_t)register_address, &register_data, 1) != 0)
	{
		DS3231_UNLOCK(handle);
		return DS3231_ERROR_INTERFACE_READ;
	}
	DS3231_UNLOCK(handle);

	*bit_stat = (ds3231_bool_t)((register_data & ((uint8_t)1 << register_bit)) >> register_bit);

	return DS3231_ERROR_OK;
}

/********************************************************/
/********************************************************/
ds3231_error_code_t _ds3231_bit_set(
	const ds3231_handle_t *handle,
	const ds3231_register_address_t register_address,
	const ds3231_register_bit_t register_bit,
	const ds3231_bool_t bit_value)
{
	/*Read, modify, write*/
	uint8_t register_data;

	DS3231_LOCK(handle);
	if (handle->interface.read_array((uint8_t)handle->i2c_address, (uint8_t)register_address, &register_data, 1) != 0)
	{
		DS3231_UNLOCK(handle);
		return DS3231_ERROR_INTERFACE_READ;
	}

	if (bit_value == DS3231_TRUE)
	{
		register_data |= (uint8_t)1 << register_bit;
	}
	else
	{
		register_data &= ~((uint8_t)1 << register_bit);
	}

	if (handle->interface.write_array((uint8_t)handle->i2c_address, (uint8_t)register_address, &register_data, 1) != 0)
	{
		DS3231_UNLOCK(handle);
		return DS3231_ERROR_INTERFACE_WRITE;
	}
	DS3231_UNLOCK(handle);

	return DS3231_ERROR_OK;
}

/********************************************************/
/********************************************************/
ds3231_error_code_t _ds3231_bcd_to_hex(uint8_t *data)
{
	/*Instead of multiplying in 10, we use shift operation for speed*/
	*data = ((*data >> 4) << 1) + ((*data >> 4) << 3) + (*data & 0X0F);

	return DS3231_ERROR_OK;
}

/********************************************************/
/********************************************************/
ds3231_error_code_t _ds3231_hex_to_bcd(uint8_t *data)
{
	*data = (((*data) / 10) << 4) + ((*data) - ((*data) / 10) * 10);

	return DS3231_ERROR_OK;
}

/********************************************************/
/********************************************************/
#if DS3231_INCLUDE_NULL_CHECK
ds3231_error_code_t _ds3231_null_check(const ds3231_handle_t *handle)
{
	/*Check for NULL handle*/
	if (handle == NULL)
	{
		return DS3231_ERROR_NULL_HANDLE;
	}

#if DS3231_INCLUDE_EXCLUSION_HOOK
	/*Check for NULL mutex handle*/
	if (handle->interface.interface_exclusion.mutex_handle == NULL)
	{
		return DS3231_ERROR_NULL_MUTEX_HANDLE;
	}
#endif

	/*Check for NULL interface function pointers*/
	if (
		handle->interface.interface_init == NULL ||
		handle->interface.interface_deinit == NULL ||
		handle->interface.delay_function == NULL ||
#if DS3231_INCLUDE_EXCLUSION_HOOK
		handle->interface.interface_exclusion.interface_lock == NULL ||
		handle->interface.interface_exclusion.interface_unlock == NULL ||
#endif
		handle->interface.read_array == NULL ||
		handle->interface.write_array == NULL)
	{
		return DS3231_ERROR_NULL_INTERFACE_FUNCTION_POINTER;
	}

	return DS3231_ERROR_OK;
}
#endif

/********************************************************/
/********************************************************/
#if DS3231_INCLUDE_WRITE_VERIFICATION
ds3231_error_code_t _ds3231_write_verify_bit(
	const ds3231_handle_t *handle,
	const ds3231_register_address_t register_address,
	const ds3231_register_bit_t bit_address,
	const ds3231_bool_t expected)
{
	ds3231_bool_t bit_stat;
	ds3231_error_code_t error = _ds3231_bit_get(handle, register_address, bit_address, &bit_stat);
	DS3231_CHECK_AND_RETURN_ERROR(error);

	/*Compare and verify bit*/
	if (expected != bit_stat)
	{
		return DS3231_ERROR_VERIFICATION_FAIL;
	}

	return DS3231_ERROR_OK;
}

/********************************************************/
/********************************************************/
ds3231_error_code_t _ds3231_write_verify_bytes(
	const ds3231_handle_t *handle,
	const ds3231_register_address_t register_address,
	const uint8_t *expected,
	const uint8_t number_of_bytes)
{
	uint8_t register_data[number_of_bytes];

	DS3231_LOCK(handle);
	if (handle->interface.read_array((uint8_t)handle->i2c_address, (uint8_t)register_address, register_data, number_of_bytes) != 0)
	{
		DS3231_UNLOCK(handle);
		return DS3231_ERROR_INTERFACE_READ;
	}
	DS3231_UNLOCK(handle);

	/*Compare and verify array of bytes*/
	for (int index = 0; index < number_of_bytes; index++)
	{
		if (expected[index] != register_data[index])
		{
			return DS3231_ERROR_VERIFICATION_FAIL;
		}
	}

	return DS3231_ERROR_OK;
}
#endif
