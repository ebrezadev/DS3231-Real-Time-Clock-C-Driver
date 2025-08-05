/**
 * @file ds3231_temperature.c
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
#if DS3231_INCLUDE_TEMPERATURE
#if DS3231_INCLUDE_TEMPERATURE_FLOAT_MATH
ds3231_error_code_t ds3231_get_temperature(const ds3231_handle_t *handle, float *temperature)
{
	ds3231_error_code_t error;
	DS3231_NULL_CHECK_MACRO(handle, error);
	DS3231_CONNECTION_CHECK(handle);

	/*Check the BSY bit*/
	ds3231_bool_t bit = DS3231_TRUE;
	uint32_t timeout = DS3231_TEMPERATURE_READ_TIMEOUT;

	for (; bit == DS3231_TRUE;)
	{
		if (timeout > DS3231_TEMPERATURE_READ_DELAY)
		{
			timeout -= DS3231_TEMPERATURE_READ_DELAY;
		}
		else
		{
			return DS3231_ERROR_TEMPERATURE_BUSY_TIMEOUT;
		}

		handle->interface.delay_function(DS3231_TEMPERATURE_READ_DELAY);

		error = _ds3231_bit_get(handle, DS3231_REGISTER_CONTROL_STATUS, DS3231_BIT_BSY, &bit);
		DS3231_CHECK_AND_RETURN_ERROR(error);
	}

	/*Set the CONV bit to start conversion of temperature to digital*/
	error = _ds3231_bit_set(handle, DS3231_REGISTER_CONTROL, DS3231_BIT_CONV, DS3231_TRUE);
	DS3231_CHECK_AND_RETURN_ERROR(error);

	DS3231_VERIFY_BIT(handle, error, DS3231_REGISTER_CONTROL, DS3231_BIT_CONV, DS3231_TRUE);

	/*Check the CONV bit*/
	bit = DS3231_TRUE;
	timeout = DS3231_TEMPERATURE_READ_TIMEOUT;

	for (; bit == DS3231_TRUE;)
	{
		if (timeout > DS3231_TEMPERATURE_READ_DELAY)
		{
			timeout -= DS3231_TEMPERATURE_READ_DELAY;
		}
		else
		{
			return DS3231_ERROR_TEMPERATURE_CONVERSION_TIMEOUT;
		}

		handle->interface.delay_function(DS3231_TEMPERATURE_READ_DELAY);

		error = _ds3231_bit_get(handle, DS3231_REGISTER_CONTROL_STATUS, DS3231_BIT_BSY, &bit);
		DS3231_CHECK_AND_RETURN_ERROR(error);
	}

	/*Read the MSB and LSB bytes. data[0] is MSB, data[1] is LSB*/
	uint8_t data[2];

	DS3231_LOCK(handle);
	if (handle->interface.read_array((uint8_t)handle->i2c_address, DS3231_REGISTER_TEMP_MSB, data, 2) != 0)
	{
		DS3231_UNLOCK(handle);
		return DS3231_ERROR_INTERFACE_WRITE;
	}
	DS3231_UNLOCK(handle);

	/*Extract the float temperature based on data*/
	float sign_bit = 1;
	if ((data[0] >> 7) == 1)
	{
		sign_bit = -1;
	}

	*temperature = (float)((int8_t)data[0]) + (float)(sign_bit * ((float)0.25 * (data[1] >> 6)));

	return DS3231_ERROR_OK;
}

#else
/********************************************************/
/********************************************************/
ds3231_error_code_t ds3231_get_temperature(const ds3231_handle_t *handle, int16_t *temperature)
{
	ds3231_error_code_t error;
	DS3231_NULL_CHECK_MACRO(handle, error);
	DS3231_CONNECTION_CHECK(handle);

	/*Check the BSY bit*/
	ds3231_bool_t bit = DS3231_TRUE;
	uint32_t timeout = DS3231_TEMPERATURE_READ_TIMEOUT;

	for (; bit == DS3231_TRUE;)
	{
		if (timeout > DS3231_TEMPERATURE_READ_DELAY)
		{
			timeout -= DS3231_TEMPERATURE_READ_DELAY;
		}
		else
		{
			return DS3231_ERROR_TEMPERATURE_BUSY_TIMEOUT;
		}

		handle->interface.delay_function(DS3231_TEMPERATURE_READ_DELAY);

		error = _ds3231_bit_get(handle, DS3231_REGISTER_CONTROL_STATUS, DS3231_BIT_BSY, &bit);
		DS3231_CHECK_AND_RETURN_ERROR(error);
	}

	/*Set the CONV bit to start conversion of temperature to digital*/
	error = _ds3231_bit_set(handle, DS3231_REGISTER_CONTROL, DS3231_BIT_CONV, DS3231_TRUE);
	DS3231_CHECK_AND_RETURN_ERROR(error);

	DS3231_VERIFY_BIT(handle, error, DS3231_REGISTER_CONTROL, DS3231_BIT_CONV, DS3231_TRUE);

	/*Check the CONV bit*/
	bit = DS3231_TRUE;
	timeout = DS3231_TEMPERATURE_READ_TIMEOUT;

	for (; bit == DS3231_TRUE;)
	{
		if (timeout > DS3231_TEMPERATURE_READ_DELAY)
		{
			timeout -= DS3231_TEMPERATURE_READ_DELAY;
		}
		else
		{
			return DS3231_ERROR_TEMPERATURE_CONVERSION_TIMEOUT;
		}

		handle->interface.delay_function(DS3231_TEMPERATURE_READ_DELAY);

		error = _ds3231_bit_get(handle, DS3231_REGISTER_CONTROL_STATUS, DS3231_BIT_BSY, &bit);
		DS3231_CHECK_AND_RETURN_ERROR(error);
	}

	/*Read the MSB and LSB bytes. data[0] is MSB, data[1] is LSB*/
	uint8_t data[2];

	DS3231_LOCK(handle);
	if (handle->interface.read_array((uint8_t)handle->i2c_address, DS3231_REGISTER_TEMP_MSB, data, 2) != 0)
	{
		DS3231_UNLOCK(handle);
		return DS3231_ERROR_INTERFACE_WRITE;
	}
	DS3231_UNLOCK(handle);

	/*Extract the float temperature based on data*/
	int16_t sign_bit = 1;
	if ((data[0] >> 7) == 1)
	{
		sign_bit = -1;
	}

	*temperature = ((int16_t)data[0] * 100) + sign_bit * 25 * (int16_t)((uint8_t)(data[1] >> 6));

	return DS3231_ERROR_OK;
}
#endif
#endif

