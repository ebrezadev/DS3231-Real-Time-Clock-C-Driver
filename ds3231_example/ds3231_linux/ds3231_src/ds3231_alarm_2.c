/**
 * @file ds3231_alarm_2.c
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
#if DS3231_INCLUDE_ALARM_2
ds3231_error_code_t ds3231_alarm_2_init(const ds3231_handle_t *handle, const ds3231_alarm_2_config_t *config)
{
	ds3231_error_code_t error;
	DS3231_NULL_CHECK_MACRO(handle, error);
	DS3231_CONNECTION_CHECK(handle);

	/*Check for mismatch in alarm rate and day/date selection*/
	if ((config->day_date_type == DS3231_ALARM_DAY) && (config->alarm_rate == DS3231_ALARM2_MATCH_MINUTE_HOUR_DATE))
	{
		return DS3231_ERROR_ALARM_RATE_AND_DAY_DATE_MISMATCH;
	}

	if ((config->day_date_type == DS3231_ALARM_DATE) && (config->alarm_rate == DS3231_ALARM2_MATCH_MINUTE_HOUR_DAY))
	{
		return DS3231_ERROR_ALARM_RATE_AND_DAY_DATE_MISMATCH;
	}

	/*Range-check and write the data into DS3231 registers*/
	switch (config->alarm_rate)
	{
	case DS3231_ALARM2_MATCH_MINUTE:
	{
		DS3231_RANGE_ERROR(config->minute, DS3231_MINUTES);

		uint8_t data = config->minute;
		error = _ds3231_hex_to_bcd(&data);

		DS3231_LOCK(handle);
		if (handle->interface.write_array((uint8_t)handle->i2c_address, DS3231_REGISTER_ALARM2_MINUTES, &data, 1) != 0)
		{
			DS3231_UNLOCK(handle);
			return DS3231_ERROR_INTERFACE_WRITE;
		}
		DS3231_UNLOCK(handle);

		DS3231_VERIFY_BYTES(handle, error, DS3231_REGISTER_ALARM2_MINUTES, &data, 1);

		break;
	}
	case DS3231_ALARM2_MATCH_MINUTE_HOUR:
	{
		DS3231_RANGE_ERROR(config->minute, DS3231_MINUTES);
		DS3231_RANGE_ERROR(config->hour, DS3231_HOURS);

		uint8_t data = config->minute;
		error = _ds3231_hex_to_bcd(&data);
		DS3231_LOCK(handle);
		if (handle->interface.write_array((uint8_t)handle->i2c_address, DS3231_REGISTER_ALARM2_MINUTES, &data, 1) != 0)
		{
			return DS3231_ERROR_INTERFACE_WRITE;
		}
		DS3231_UNLOCK(handle);
		DS3231_VERIFY_BYTES(handle, error, DS3231_REGISTER_ALARM2_MINUTES, &data, 1);

		data = config->hour;
		error = _ds3231_hex_to_bcd(&data);
		DS3231_LOCK(handle);
		if (handle->interface.write_array((uint8_t)handle->i2c_address, DS3231_REGISTER_ALARM2_HOURS, &data, 1) != 0)
		{
			DS3231_UNLOCK(handle);
			return DS3231_ERROR_INTERFACE_WRITE;
		}
		DS3231_UNLOCK(handle);
		DS3231_VERIFY_BYTES(handle, error, DS3231_REGISTER_ALARM2_HOURS, &data, 1);

		break;
	}
	case DS3231_ALARM2_MATCH_MINUTE_HOUR_DATE:
	{
		DS3231_RANGE_ERROR(config->minute, DS3231_MINUTES);
		DS3231_RANGE_ERROR(config->hour, DS3231_HOURS);
		DS3231_RANGE_ERROR(config->day_date.date, DS3231_DATE);

		uint8_t data = config->minute;
		error = _ds3231_hex_to_bcd(&data);
		DS3231_LOCK(handle);
		if (handle->interface.write_array((uint8_t)handle->i2c_address, DS3231_REGISTER_ALARM2_MINUTES, &data, 1) != 0)
		{
			DS3231_UNLOCK(handle);
			return DS3231_ERROR_INTERFACE_WRITE;
		}
		DS3231_UNLOCK(handle);
		DS3231_VERIFY_BYTES(handle, error, DS3231_REGISTER_ALARM2_MINUTES, &data, 1);

		data = config->hour;
		error = _ds3231_hex_to_bcd(&data);
		DS3231_LOCK(handle);
		if (handle->interface.write_array((uint8_t)handle->i2c_address, DS3231_REGISTER_ALARM2_HOURS, &data, 1) != 0)
		{
			DS3231_UNLOCK(handle);
			return DS3231_ERROR_INTERFACE_WRITE;
		}
		DS3231_UNLOCK(handle);
		DS3231_VERIFY_BYTES(handle, error, DS3231_REGISTER_ALARM2_HOURS, &data, 1);

		error = _ds3231_hex_to_bcd(&data);
		data = config->day_date.date & (~(1 << DS3231_BIT_DY_DT_ALARM2));
		DS3231_LOCK(handle);
		if (handle->interface.write_array((uint8_t)handle->i2c_address, DS3231_REGISTER_ALARM2_DAY_OF_WEEK_OR_DATE, &data, 1) != 0)
		{
			DS3231_UNLOCK(handle);
			return DS3231_ERROR_INTERFACE_WRITE;
		}
		DS3231_UNLOCK(handle);
		DS3231_VERIFY_BYTES(handle, error, DS3231_REGISTER_ALARM2_DAY_OF_WEEK_OR_DATE, &data, 1);

		break;
	}
	case DS3231_ALARM2_MATCH_MINUTE_HOUR_DAY:
	{
		DS3231_RANGE_ERROR(config->minute, DS3231_MINUTES);
		DS3231_RANGE_ERROR(config->hour, DS3231_HOURS);
		DS3231_RANGE_ERROR(config->day_date.day, DS3231_DAY);

		uint8_t data = config->minute;
		error = _ds3231_hex_to_bcd(&data);
		DS3231_LOCK(handle);
		if (handle->interface.write_array((uint8_t)handle->i2c_address, DS3231_REGISTER_ALARM2_MINUTES, &data, 1) != 0)
		{
			DS3231_UNLOCK(handle);
			return DS3231_ERROR_INTERFACE_WRITE;
		}
		DS3231_UNLOCK(handle);
		DS3231_VERIFY_BYTES(handle, error, DS3231_REGISTER_ALARM2_MINUTES, &data, 1);

		data = config->hour;
		error = _ds3231_hex_to_bcd(&data);
		DS3231_LOCK(handle);
		if (handle->interface.write_array((uint8_t)handle->i2c_address, DS3231_REGISTER_ALARM2_HOURS, &data, 1) != 0)
		{
			DS3231_UNLOCK(handle);
			return DS3231_ERROR_INTERFACE_WRITE;
		}
		DS3231_UNLOCK(handle);
		DS3231_VERIFY_BYTES(handle, error, DS3231_REGISTER_ALARM2_HOURS, &data, 1);

		error = _ds3231_hex_to_bcd(&data);
		data = config->day_date.day | (1 << DS3231_BIT_DY_DT_ALARM2);
		DS3231_LOCK(handle);
		if (handle->interface.write_array((uint8_t)handle->i2c_address, DS3231_REGISTER_ALARM2_DAY_OF_WEEK_OR_DATE, &data, 1) != 0)
		{
			DS3231_UNLOCK(handle);
			return DS3231_ERROR_INTERFACE_WRITE;
		}
		DS3231_UNLOCK(handle);
		DS3231_VERIFY_BYTES(handle, error, DS3231_REGISTER_ALARM2_DAY_OF_WEEK_OR_DATE, &data, 1);

		break;
	}
	default:
		break;
	}

	/*Set the alarm rate*/
	error = ds3231_alarm_2_rate_select(handle, config->alarm_rate);
	DS3231_CHECK_AND_RETURN_ERROR(error);

	return DS3231_ERROR_OK;
}

/********************************************************/
/********************************************************/
ds3231_error_code_t ds3231_alarm_2_rate_select(const ds3231_handle_t *handle, const ds3231_alarm_2_rate_t alarm_rate)
{
	ds3231_error_code_t error;
	DS3231_NULL_CHECK_MACRO(handle, error);
	DS3231_CONNECTION_CHECK(handle);

	error = _ds3231_bit_set(handle, DS3231_REGISTER_ALARM2_MINUTES, DS3231_BIT_A2M2, (ds3231_bool_t)DS3231_ALARM_2_MASK_BITS[(int)alarm_rate][0]);
	DS3231_CHECK_AND_RETURN_ERROR(error);
	DS3231_VERIFY_BIT(handle, error, DS3231_REGISTER_ALARM2_MINUTES, DS3231_BIT_A2M2, (ds3231_bool_t)DS3231_ALARM_2_MASK_BITS[(int)alarm_rate][0]);

	error = _ds3231_bit_set(handle, DS3231_REGISTER_ALARM2_HOURS, DS3231_BIT_A2M3, (ds3231_bool_t)DS3231_ALARM_2_MASK_BITS[(int)alarm_rate][1]);
	DS3231_CHECK_AND_RETURN_ERROR(error);
	DS3231_VERIFY_BIT(handle, error, DS3231_REGISTER_ALARM2_HOURS, DS3231_BIT_A2M3, (ds3231_bool_t)DS3231_ALARM_2_MASK_BITS[(int)alarm_rate][1]);

	error = _ds3231_bit_set(handle, DS3231_REGISTER_ALARM2_DAY_OF_WEEK_OR_DATE, DS3231_BIT_A2M4, (ds3231_bool_t)DS3231_ALARM_2_MASK_BITS[(int)alarm_rate][2]);
	DS3231_CHECK_AND_RETURN_ERROR(error);
	DS3231_VERIFY_BIT(handle, error, DS3231_REGISTER_ALARM2_DAY_OF_WEEK_OR_DATE, DS3231_BIT_A2M4, (ds3231_bool_t)DS3231_ALARM_2_MASK_BITS[(int)alarm_rate][2]);

	error = _ds3231_bit_set(handle, DS3231_REGISTER_ALARM2_DAY_OF_WEEK_OR_DATE, DS3231_BIT_DY_DT_ALARM2, (ds3231_bool_t)DS3231_ALARM_2_MASK_BITS[(int)alarm_rate][3]);
	DS3231_CHECK_AND_RETURN_ERROR(error);
	DS3231_VERIFY_BIT(handle, error, DS3231_REGISTER_ALARM2_DAY_OF_WEEK_OR_DATE, DS3231_BIT_DY_DT_ALARM2, (ds3231_bool_t)DS3231_ALARM_2_MASK_BITS[(int)alarm_rate][3]);

	return DS3231_ERROR_OK;
}

/********************************************************/
/********************************************************/
ds3231_error_code_t ds3231_alarm_2_interrupt_control(const ds3231_handle_t *handle, const ds3231_bool_t enable)
{
	ds3231_error_code_t error;
	DS3231_NULL_CHECK_MACRO(handle, error);
	DS3231_CONNECTION_CHECK(handle);

	/*Set the A2IE bit*/
	error = _ds3231_bit_set(handle, DS3231_REGISTER_CONTROL, DS3231_BIT_A2IE, enable);
	DS3231_CHECK_AND_RETURN_ERROR(error);

	DS3231_VERIFY_BIT(handle, error, DS3231_REGISTER_CONTROL, DS3231_BIT_A2IE, enable);

	return DS3231_ERROR_OK;
}

/********************************************************/
/********************************************************/
ds3231_error_code_t ds3231_alarm_2_flag_poll(const ds3231_handle_t *handle, ds3231_bool_t *flag_bit)
{
	ds3231_error_code_t error;
	DS3231_NULL_CHECK_MACRO(handle, error);
	DS3231_CONNECTION_CHECK(handle);

	error = _ds3231_bit_get(handle, DS3231_REGISTER_CONTROL_STATUS, DS3231_BIT_A2F, flag_bit);
	DS3231_CHECK_AND_RETURN_ERROR(error);

	return DS3231_ERROR_OK;
}

/********************************************************/
/********************************************************/
ds3231_error_code_t ds3231_alarm_2_flag_clear(const ds3231_handle_t *handle)
{
	ds3231_error_code_t error;
	DS3231_NULL_CHECK_MACRO(handle, error);
	DS3231_CONNECTION_CHECK(handle);

	error = _ds3231_bit_set(handle, DS3231_REGISTER_CONTROL_STATUS, DS3231_BIT_A2F, DS3231_FALSE);
	DS3231_CHECK_AND_RETURN_ERROR(error);

	/*No verification is implemented, since the module may set the flag*/
	return DS3231_ERROR_OK;
}

#endif