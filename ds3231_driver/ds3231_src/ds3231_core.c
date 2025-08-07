/**
 * @file ds3231_core.c
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
ds3231_error_code_t ds3231_init(ds3231_handle_t *handle)
{
	/*Check for NULL pointers*/
	ds3231_error_code_t error;
	DS3231_NULL_CHECK_MACRO(handle, error);

	handle->i2c_address = DS3231_I2C_ADDRESS;

	/*initialize the interface*/
	DS3231_LOCK(handle);
	if (handle->interface.interface_init(handle->i2c_address) != 0)
	{
		DS3231_UNLOCK(handle);
		return DS3231_ERROR_INTERFACE_INIT;
	}
	DS3231_UNLOCK(handle);

	/*Check for disconnected ds3231*/
	DS3231_CONNECTION_CHECK(handle);

	/*By default and for consistency, use 24h format for hour*/
	error = _ds3231_bit_set(handle, DS3231_REGISTER_HOURS, DS3231_BIT_12_24, DS3231_FALSE);
	DS3231_CHECK_AND_RETURN_ERROR(error);

	DS3231_VERIFY_BIT(handle, error, DS3231_REGISTER_HOURS, DS3231_BIT_12_24, DS3231_FALSE);

	return DS3231_ERROR_OK;
}

/********************************************************/
/********************************************************/
ds3231_error_code_t ds3231_deinit(ds3231_handle_t *handle)
{
	DS3231_LOCK(handle);
	if (handle->interface.interface_deinit(handle->i2c_address) != 0)
	{
		DS3231_UNLOCK(handle);
		return DS3231_ERROR_INTERFACE_DEINIT;
	}
	DS3231_UNLOCK(handle);

	return DS3231_ERROR_OK;
}

/********************************************************/
/********************************************************/
ds3231_error_code_t _ds3231_reset(const ds3231_handle_t *handle, const ds3231_register_address_t starting_register, const uint8_t number_of_registers)
{
#if DS3231_INCLUDE_WRITE_VERIFICATION | DS3231_INCLUDE_NULL_CHECK
	ds3231_error_code_t error;
#endif
	DS3231_NULL_CHECK_MACRO(handle, error);

	DS3231_CONNECTION_CHECK(handle);

	/*Write default values to desired registers*/
	DS3231_LOCK(handle);
	if (handle->interface.write_array((uint8_t)handle->i2c_address, (uint8_t)starting_register, (uint8_t *)&REGISTER_DEFAULT_VALUE[(uint8_t)starting_register], number_of_registers) != 0)
	{
		DS3231_UNLOCK(handle);
		return DS3231_ERROR_INTERFACE_WRITE;
	}
	DS3231_UNLOCK(handle);

	DS3231_VERIFY_BYTES(handle, error, starting_register, &REGISTER_DEFAULT_VALUE[(uint8_t)starting_register], number_of_registers);

	return DS3231_ERROR_OK;
}

/********************************************************/
/********************************************************/
ds3231_error_code_t ds3231_is_running(const ds3231_handle_t *handle, ds3231_bool_t *is_running)
{
	ds3231_error_code_t error;
	DS3231_NULL_CHECK_MACRO(handle, error);
	DS3231_CONNECTION_CHECK(handle);

	ds3231_bool_t OSF_bit;

	/*Read the OSF bit*/
	error = _ds3231_bit_get(handle, DS3231_REGISTER_CONTROL_STATUS, DS3231_BIT_OSF, &OSF_bit);
	DS3231_CHECK_AND_RETURN_ERROR(error);

	/*Return OK if oscillator is running*/
	if (OSF_bit != (ds3231_bool_t)DS3231_OSCILLATOR_STOPPED)
	{
		*is_running = DS3231_TRUE;

		return DS3231_ERROR_OK;
	}

	/*the oscillator is stopped. delay for DS3231_OSC_FLAG_DELAY_MS*/
	if (handle->interface.delay_function(DS3231_OSC_FLAG_DELAY_MS) != 0)
	{
		return DS3231_ERROR_INTERFACE_DELAY;
	}

	/*Manually reset the OSF bit*/
	error = _ds3231_bit_set(handle, DS3231_REGISTER_CONTROL_STATUS, DS3231_BIT_OSF, DS3231_FALSE);
	DS3231_CHECK_AND_RETURN_ERROR(error);
	DS3231_VERIFY_BIT(handle, error, DS3231_REGISTER_CONTROL_STATUS, DS3231_BIT_OSF, DS3231_FALSE);

	/*delay for DS3231_OSC_FLAG_DELAY_MS*/
	if (handle->interface.delay_function(DS3231_OSC_FLAG_DELAY_MS) != 0)
	{
		return DS3231_ERROR_INTERFACE_DELAY;
	}

	error = _ds3231_bit_get(handle, DS3231_REGISTER_CONTROL_STATUS, DS3231_BIT_OSF, &OSF_bit);
	DS3231_CHECK_AND_RETURN_ERROR(error);

	/*Return OK if oscillator is running*/
	if (OSF_bit != (ds3231_bool_t)DS3231_OSCILLATOR_STOPPED)
	{
		*is_running = DS3231_TRUE;
	}
	else
	{
		*is_running = DS3231_FALSE;
	}

	return DS3231_ERROR_OK;
}

/********************************************************/
/********************************************************/
ds3231_error_code_t _ds3231_set_time_and_calendar(const ds3231_handle_t *handle, ds3231_time_register_t time_register, uint16_t value)
{
	ds3231_error_code_t error;
	DS3231_NULL_CHECK_MACRO(handle, error);
	DS3231_CONNECTION_CHECK(handle);

	/*Check for error in range*/
	DS3231_RANGE_ERROR(value, time_register);

	/*Determine the century bit in case of year and trim the 16 bit year value into the range of 0 to 99*/
	ds3231_bool_t century_bit = DS3231_FALSE;

	if ((time_register == DS3231_YEAR) && (value < 2000))
	{
		century_bit = DS3231_TRUE;
		value -= 1900;
	}
	else if (time_register == DS3231_YEAR)
	{
		century_bit = DS3231_FALSE;
		value -= 2000;
	}

	/*Convert the value to BCD*/
	uint8_t value_in_bcd = (uint8_t)value;
	error = _ds3231_hex_to_bcd(&value_in_bcd);
	DS3231_CHECK_AND_RETURN_ERROR(error);

	/*Read the time register*/
	uint8_t data;
	DS3231_LOCK(handle);
	if (handle->interface.read_array((uint8_t)handle->i2c_address, (uint8_t)time_register, &data, 1) != 0)
	{
  DS3231_UNLOCK(handle);
		return DS3231_ERROR_INTERFACE_READ;
	}
	DS3231_UNLOCK(handle);

	/*Reverse-mask the data to keep the settings*/
	data &= ~DS3231_MASK_AND_RANGE_LUT[time_register].mask;

	/*Calculate the new data for register*/
	data = data | (value_in_bcd & DS3231_MASK_AND_RANGE_LUT[time_register].mask);

	/*Write the new register value*/
	DS3231_LOCK(handle);
	if (handle->interface.write_array((uint8_t)handle->i2c_address, (uint8_t)time_register, &data, 1) != 0)
	{
		DS3231_UNLOCK(handle);
		return DS3231_ERROR_INTERFACE_WRITE;
	}
	DS3231_UNLOCK(handle);

	DS3231_VERIFY_BYTES(handle, error, (ds3231_register_address_t)time_register, &data, 1);

	if (time_register == DS3231_YEAR)
	{
		error = _ds3231_bit_set(handle, DS3231_REGISTER_MONTH, DS3231_BIT_CENTURY, century_bit);
		DS3231_CHECK_AND_RETURN_ERROR(error);

		DS3231_VERIFY_BIT(handle, error, DS3231_REGISTER_MONTH, DS3231_BIT_CENTURY, century_bit);
	}

	return DS3231_ERROR_OK;
}

/********************************************************/
/********************************************************/
ds3231_error_code_t ds3231_set_all_time_and_calendar(const ds3231_handle_t *handle, ds3231_time_and_calendar_t *time_struct)
{
	ds3231_error_code_t error;
	DS3231_NULL_CHECK_MACRO(handle, error);
	DS3231_CONNECTION_CHECK(handle);

	/*Check for error in range*/
	DS3231_RANGE_ERROR(time_struct->second, DS3231_SECONDS);
	DS3231_RANGE_ERROR(time_struct->minute, DS3231_MINUTES);
	DS3231_RANGE_ERROR(time_struct->hour, DS3231_HOURS);
	DS3231_RANGE_ERROR(time_struct->day, DS3231_DAY);
	DS3231_RANGE_ERROR(time_struct->date, DS3231_DATE);
	DS3231_RANGE_ERROR(time_struct->month, DS3231_MONTH);
	DS3231_RANGE_ERROR(time_struct->year, DS3231_YEAR);

	ds3231_bool_t century_bit;

	/*Determine the century bit and trim the 16 bit year value into the range of 0 to 99*/
	if (time_struct->year < 2000)
	{
		century_bit = (ds3231_bool_t)1;
		time_struct->year -= 1900;
	}
	else
	{
		century_bit = (ds3231_bool_t)0;
		time_struct->year -= 2000;
	}

	/*Copy the struct to the array and convert the array to BCD*/
	uint8_t value_in_bcd[DS3231_NUMBER_OF_TIME_REGISTERS];

	value_in_bcd[DS3231_SECONDS] = (uint8_t)time_struct->second;
	value_in_bcd[DS3231_MINUTES] = (uint8_t)time_struct->minute;
	value_in_bcd[DS3231_HOURS] = (uint8_t)time_struct->hour;
	value_in_bcd[DS3231_DAY] = (uint8_t)time_struct->day;
	value_in_bcd[DS3231_DATE] = (uint8_t)time_struct->date;
	value_in_bcd[DS3231_MONTH] = (uint8_t)time_struct->month;
	value_in_bcd[DS3231_YEAR] = (uint8_t)time_struct->year;

	for (int index = DS3231_SECONDS; index <= DS3231_YEAR; index++)
	{
		error = _ds3231_hex_to_bcd(&value_in_bcd[index]);
		DS3231_CHECK_AND_RETURN_ERROR(error);
	}

	/*Read the time registers*/
	uint8_t data[DS3231_NUMBER_OF_TIME_REGISTERS];

	DS3231_LOCK(handle);
	if (handle->interface.read_array((uint8_t)handle->i2c_address, (uint8_t)DS3231_SECONDS, data, DS3231_NUMBER_OF_TIME_REGISTERS) != 0)
	{
		DS3231_UNLOCK(handle);
		return DS3231_ERROR_INTERFACE_READ;
	}
	DS3231_UNLOCK(handle);

	/*Reverse-mask the data to keep the settings, Calculate the new data for register*/
	for (int index = DS3231_SECONDS; index <= DS3231_YEAR; index++)
	{
		data[index] &= ~DS3231_MASK_AND_RANGE_LUT[index].mask;

		data[index] |= (value_in_bcd[index] & DS3231_MASK_AND_RANGE_LUT[index].mask);
	}

	/*Write the new register values*/
	DS3231_LOCK(handle);
	if (handle->interface.write_array((uint8_t)handle->i2c_address, (uint8_t)DS3231_SECONDS, data, DS3231_NUMBER_OF_TIME_REGISTERS) != 0)
	{
		DS3231_UNLOCK(handle);
		return DS3231_ERROR_INTERFACE_WRITE;
	}
	DS3231_UNLOCK(handle);

	DS3231_VERIFY_BYTES(handle, error, (ds3231_register_address_t)DS3231_SECONDS, data, DS3231_NUMBER_OF_TIME_REGISTERS);

	/*Update the century bit in month register*/
	error = _ds3231_bit_set(handle, DS3231_REGISTER_MONTH, DS3231_BIT_CENTURY, century_bit);
	DS3231_CHECK_AND_RETURN_ERROR(error);

	DS3231_VERIFY_BIT(handle, error, DS3231_REGISTER_MONTH, DS3231_BIT_CENTURY, century_bit);

	return DS3231_ERROR_OK;
}

/********************************************************/
/********************************************************/
ds3231_error_code_t _ds3231_get_time_and_calendar(const ds3231_handle_t *handle, const ds3231_time_register_t time_register, uint16_t *value)
{
	ds3231_error_code_t error;
	DS3231_NULL_CHECK_MACRO(handle, error);
	DS3231_CONNECTION_CHECK(handle);

	uint8_t data;

	DS3231_LOCK(handle);
	if (handle->interface.read_array((uint8_t)handle->i2c_address, (uint8_t)time_register, &data, 1) != 0)
	{
		DS3231_UNLOCK(handle);
		return DS3231_ERROR_INTERFACE_READ;
	}
	DS3231_UNLOCK(handle);

	ds3231_bool_t century_bit;

	/*Get the century bit in case of year*/
	if (time_register == DS3231_YEAR)
	{
		error = _ds3231_bit_get(handle, DS3231_REGISTER_MONTH, DS3231_BIT_CENTURY, &century_bit);
		DS3231_CHECK_AND_RETURN_ERROR(error);
	}

	/*Mask the data*/
	data &= DS3231_MASK_AND_RANGE_LUT[time_register].mask;

	/*Convert from BCD to HEX*/
	error = _ds3231_bcd_to_hex(&data);
	DS3231_CHECK_AND_RETURN_ERROR(error);

	/*Define a 16 bit data to handle the year*/
	uint16_t data_16_bit = (uint16_t)data;

	if ((time_register == DS3231_YEAR) && (century_bit == (ds3231_bool_t)0))
	{
		data_16_bit += 2000;
	}
	else if (time_register == DS3231_YEAR)
	{
		data_16_bit += 1900;
	}

	/*Check data range*/
	DS3231_RANGE_ERROR(data_16_bit, time_register);

	/*Copy the data*/
	*value = data_16_bit;

	return DS3231_ERROR_OK;
}

/********************************************************/
/********************************************************/
ds3231_error_code_t ds3231_get_all_time_and_calendar(const ds3231_handle_t *handle, ds3231_time_and_calendar_t *time_struct)
{
	ds3231_error_code_t error;
	DS3231_NULL_CHECK_MACRO(handle, error);
	DS3231_CONNECTION_CHECK(handle);

	uint8_t data[7];

	DS3231_LOCK(handle);
	if (handle->interface.read_array((uint8_t)handle->i2c_address, (uint8_t)DS3231_REGISTER_SECONDS, data, DS3231_NUMBER_OF_TIME_REGISTERS) != 0)
	{
		DS3231_UNLOCK(handle);
		return DS3231_ERROR_INTERFACE_READ;
	}
	DS3231_UNLOCK(handle);

	ds3231_bool_t century_bit;

	/*Get the century bit*/
	error = _ds3231_bit_get(handle, DS3231_REGISTER_MONTH, DS3231_BIT_CENTURY, &century_bit);
	DS3231_CHECK_AND_RETURN_ERROR(error);

	/*Iterate, mask and range-check all the data*/
	for (int index = (int)DS3231_SECONDS; index <= (int)DS3231_YEAR; index++)
	{
		data[index] &= DS3231_MASK_AND_RANGE_LUT[index].mask;

		error = _ds3231_bcd_to_hex(&data[index]);
		DS3231_CHECK_AND_RETURN_ERROR(error);

		/*Do not range-check the year*/
		if (index == DS3231_YEAR)
		{
			continue;
		}

		DS3231_RANGE_ERROR(data[index], index);
	}

	/*Copy the data into the time-struct*/
	time_struct->second = (uint16_t)data[DS3231_SECONDS];
	time_struct->minute = (uint16_t)data[DS3231_MINUTES];
	time_struct->hour = (uint16_t)data[DS3231_HOURS];
	time_struct->day = (ds3231_day_t)data[DS3231_DAY];
	time_struct->date = (uint16_t)data[DS3231_DATE];
	time_struct->month = (ds3231_month_t)data[DS3231_MONTH];
	time_struct->year = (uint16_t)data[DS3231_YEAR];

	if (century_bit == 0)
	{
		time_struct->year += 2000;
	}
	else
	{
		time_struct->year += 1900;
	}

	/*Range-check the year*/
	DS3231_RANGE_ERROR(time_struct->year, DS3231_YEAR);

	return DS3231_ERROR_OK;
}

/********************************************************/
/********************************************************/
ds3231_error_code_t ds3231_32khz_wave_control(const ds3231_handle_t *handle, const ds3231_bool_t enable)
{
	ds3231_error_code_t error;
	DS3231_NULL_CHECK_MACRO(handle, error);
	DS3231_CONNECTION_CHECK(handle);

	/*Set the en32khz bit*/
	error = _ds3231_bit_set(handle, DS3231_REGISTER_CONTROL_STATUS, DS3231_BIT_EN32KHZ, enable);
	DS3231_CHECK_AND_RETURN_ERROR(error);

	DS3231_VERIFY_BIT(handle, error, DS3231_REGISTER_CONTROL_STATUS, DS3231_BIT_EN32KHZ, enable);

	return DS3231_ERROR_OK;
}

/********************************************************/
/********************************************************/
ds3231_error_code_t ds3231_sqw_output_wave_frequency(const ds3231_handle_t *handle, const ds3231_sqw_output_wave_frequency_t wave_freq)
{
	ds3231_error_code_t error;
	DS3231_NULL_CHECK_MACRO(handle, error);
	DS3231_CONNECTION_CHECK(handle);

	/*Write RS1 and RS2 bits to ds3231*/
	ds3231_bool_t RS1_bool = (ds3231_bool_t)(wave_freq & 1);
	ds3231_bool_t RS2_bool = (ds3231_bool_t)((wave_freq >> 1) & 1);

	error = _ds3231_bit_set(handle, DS3231_REGISTER_CONTROL, DS3231_BIT_RS1, RS1_bool);
	DS3231_CHECK_AND_RETURN_ERROR(error);

	DS3231_VERIFY_BIT(handle, error, DS3231_REGISTER_CONTROL, DS3231_BIT_RS1, RS1_bool);

	error = _ds3231_bit_set(handle, DS3231_REGISTER_CONTROL, DS3231_BIT_RS2, RS2_bool);
	DS3231_CHECK_AND_RETURN_ERROR(error);

	DS3231_VERIFY_BIT(handle, error, DS3231_REGISTER_CONTROL, DS3231_BIT_RS2, RS2_bool);

	return DS3231_ERROR_OK;
}

/********************************************************/
/********************************************************/
ds3231_error_code_t ds3231_int_sqw_pin_select(const ds3231_handle_t *handle, const ds3231_int_sqw_pin_t output_pin)
{
	ds3231_error_code_t error;
	DS3231_NULL_CHECK_MACRO(handle, error);
	DS3231_CONNECTION_CHECK(handle);

	/*Set or reset the INTCN bit*/
	error = _ds3231_bit_set(handle, DS3231_REGISTER_CONTROL, DS3231_BIT_INTCN, (ds3231_bool_t)output_pin);
	DS3231_CHECK_AND_RETURN_ERROR(error);

	DS3231_VERIFY_BIT(handle, error, DS3231_REGISTER_CONTROL, DS3231_BIT_INTCN, (ds3231_bool_t)output_pin);

	return DS3231_ERROR_OK;
}

/********************************************************/
/********************************************************/
#if DS3231_INCLUDE_AGING_OFFSET_CALIBRATION
ds3231_error_code_t ds3231_aging_offset_calibration(const ds3231_handle_t *handle, const int8_t offset)
{
#if DS3231_INCLUDE_SAFE_RANGE_CHECK | DS3231_INCLUDE_WRITE_VERIFICATION
	ds3231_error_code_t error;
#endif
	DS3231_NULL_CHECK_MACRO(handle, error);
	DS3231_CONNECTION_CHECK(handle);

	uint8_t data = (uint8_t)offset;

	DS3231_LOCK(handle);
	if (handle->interface.write_array((uint8_t)handle->i2c_address, DS3231_REGISTER_AGING_OFFSET, &data, 1) != 0)
	{
		DS3231_UNLOCK(handle);
		return DS3231_ERROR_INTERFACE_WRITE;
	}
	DS3231_UNLOCK(handle);

	DS3231_VERIFY_BYTES(handle, error, DS3231_REGISTER_AGING_OFFSET, &data, 1);

	return DS3231_ERROR_OK;
}
#endif

/********************************************************/
/********************************************************/
ds3231_error_code_t ds3231_battery_backed_oscillator_control(const ds3231_handle_t *handle, const ds3231_bool_t bb_osc_control)
{
	ds3231_error_code_t error;
	DS3231_NULL_CHECK_MACRO(handle, error);
	DS3231_CONNECTION_CHECK(handle);

	error = _ds3231_bit_set(handle, DS3231_REGISTER_CONTROL, DS3231_BIT_EOSC, bb_osc_control);
	DS3231_CHECK_AND_RETURN_ERROR(error);

	DS3231_VERIFY_BIT(handle, error, DS3231_REGISTER_CONTROL, DS3231_BIT_EOSC, bb_osc_control);

	return DS3231_ERROR_OK;
}

/********************************************************/
/********************************************************/
ds3231_error_code_t ds3231_battery_backed_sqw_control(const ds3231_handle_t *handle, const ds3231_bool_t bb_sqw_control)
{
	ds3231_error_code_t error;
	DS3231_NULL_CHECK_MACRO(handle, error);
	DS3231_CONNECTION_CHECK(handle);

	error = _ds3231_bit_set(handle, DS3231_REGISTER_CONTROL, DS3231_BIT_BBSQW, bb_sqw_control);
	DS3231_CHECK_AND_RETURN_ERROR(error);

	DS3231_VERIFY_BIT(handle, error, DS3231_REGISTER_CONTROL, DS3231_BIT_BBSQW, bb_sqw_control);

	return DS3231_ERROR_OK;
}

/********************************************************/
/********************************************************/
#if DS3231_INCLUDE_ERROR_LOG_STRINGS
ds3231_error_code_t ds3231_error_string(ds3231_error_code_t error_code, char **message)
{
	*message = (char *)(DS3231_ERROR_LOG_STRING[error_code]);

	return DS3231_ERROR_OK;
}
#endif




