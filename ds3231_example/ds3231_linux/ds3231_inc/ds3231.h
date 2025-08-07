/**
 * @file	ds3231.h
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
#ifndef __DS3231_H__
#define __DS3231_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "ds3231_typedefs.h"
#include "ds3231_constants.h"
#include "ds3231_error.h"

	/**
	 * @brief The init function
	 *
	 * Implements the init function. Must be called first to initialize.
	 *
	 * @param handle: pointer to a handle of DS3231
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_init(ds3231_handle_t *handle);

	/**
	 * @brief The deinit function
	 *
	 * Implements the deinit function.
	 *
	 * @param handle: pointer to a handle of DS3231
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_deinit(ds3231_handle_t *handle);

	/**
	 * @brief The reset function
	 *
	 * Resets DS3231 registers starting from "starting_registers" up to "starting_registers + number_of_registers - 1"
	 *
	 * @param handle: pointer to a handle of DS3231
	 * @param starting_register: The address of starting register
	 * @param number_of_registers: Number of registers to de set to default values
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t _ds3231_reset(const ds3231_handle_t *handle, const ds3231_register_address_t starting_register, const uint8_t number_of_registers);

/**
 * @brief The time and calendar reset macro
 *
 * Resets the time and calendar registers.
 *
 * @param ds3231_handle_pointer: pointer to a handle of DS3231
 * @return Returns 0 for no error
 */
#define ds3231_reset_time_and_calendar(ds3231_handle_pointer) _ds3231_reset((ds3231_handle_pointer), (ds3231_register_address_t)DS3231_SECONDS, DS3231_NUMBER_OF_TIME_REGISTERS)

/**
 * @brief The seconds reset macro
 *
 * Resets the second registers.
 *
 * @param ds3231_handle_pointer: pointer to a handle of DS3231
 * @return Returns 0 for no error
 */
#define ds3231_reset_second(ds3231_handle_pointer) _ds3231_reset((ds3231_handle_pointer), (ds3231_register_address_t)DS3231_SECONDS, (1))

/**
 * @brief The alarm 1 reset macro
 *
 * Resets the alarm 1 registers.
 *
 * @param ds3231_handle_pointer: pointer to a handle of DS3231
 * @return Returns 0 for no error
 */
#define ds3231_reset_alarm_1(ds3231_handle_pointer) _ds3231_reset((ds3231_handle_pointer), (ds3231_register_address_t)DS3231_REGISTER_ALARM1_SECONDS, (4))

/**
 * @brief The alarm 2 reset macro
 *
 * Resets the alarm 2 registers.
 *
 * @param ds3231_handle_pointer: pointer to a handle of DS3231
 * @return Returns 0 for no error
 */
#define ds3231_reset_alarm_2(ds3231_handle_pointer) _ds3231_reset((ds3231_handle_pointer), (ds3231_register_address_t)DS3231_REGISTER_ALARM2_MINUTES, (3))

/**
 * @brief The control registers reset macro
 *
 * Resets the control registers.
 *
 * @param ds3231_handle_pointer: pointer to a handle of DS3231
 * @return Returns 0 for no error
 */
#define ds3231_reset_control(ds3231_handle_pointer) _ds3231_reset((ds3231_handle_pointer), (ds3231_register_address_t)DS3231_REGISTER_CONTROL, (3))

/**
 * @brief The full reset macro
 *
 * Resets all the registers.
 *
 * @param ds3231_handle_pointer: pointer to a handle of DS3231
 * @return Returns 0 for no error
 */
#define ds3231_reset_all(ds3231_handle_pointer) _ds3231_reset((ds3231_handle_pointer), (ds3231_register_address_t)DS3231_SECONDS, (16))

	/**
	 * @brief The get all time and calendar function
	 *
	 * Gets all the time and calendar registers data
	 *
	 * @param handle: pointer to a handle of DS3231
	 * @param time_struct: pointer to a ds3231_time_and_calendar_t struct
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_get_all_time_and_calendar(const ds3231_handle_t *handle, ds3231_time_and_calendar_t *time_struct);

	/**
	 * @brief The get time and calendar function
	 *
	 * Get the time and calendar registers data selectively in a 16 bit variable.
	 *
	 * @param handle: pointer to a handle of DS3231
	 * @param time_register: address of desired time and clanedar register
	 * @param value: pointer to a uint16 variable
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t _ds3231_get_time_and_calendar(const ds3231_handle_t *handle, const ds3231_time_register_t time_register, uint16_t *value);

/**
 * @brief The get seconds macro
 *
 * Get the seconds value in a 16 bit variable (range: 0 to 59).
 *
 * @param ds3231_handle_pointer: pointer to a handle of DS3231
 * @param uint16_value_pointer: pointer to a uint16 variable
 * @return Returns 0 for no error
 */
#define ds3231_get_second(ds3231_handle_pointer, uint16_value_pointer) _ds3231_get_time_and_calendar((ds3231_handle_pointer), DS3231_SECONDS, (uint16_t *)(uint16_value_pointer))

/**
 * @brief The get minutes macro
 *
 * Get the minutes value in a 16 bit variable (range: 0 to 59).
 *
 * @param ds3231_handle_pointer: pointer to a handle of DS3231
 * @param uint16_value_pointer: pointer to a uint16 variable
 * @return Returns 0 for no error
 */
#define ds3231_get_minute(ds3231_handle_pointer, uint16_value_pointer) _ds3231_get_time_and_calendar((ds3231_handle_pointer), DS3231_MINUTES, (uint16_t *)(uint16_value_pointer))

/**
 * @brief The get hours macro
 *
 * Get the hours value in a 16 bit variable (range: 0 to 23).
 *
 * @param ds3231_handle_pointer: pointer to a handle of DS3231
 * @param uint16_value_pointer: pointer to a uint16 variable
 * @return Returns 0 for no error
 */
#define ds3231_get_hour(ds3231_handle_pointer, uint16_value_pointer) _ds3231_get_time_and_calendar((ds3231_handle_pointer), DS3231_HOURS, (uint16_t *)(uint16_value_pointer))

/**
 * @brief The get day macro
 *
 * Get the days value in a 16 bit variable (range: 1 to 7).
 *
 * @param ds3231_handle_pointer: pointer to a handle of DS3231
 * @param uint16_value_pointer: pointer to a uint16 variable
 * @return Returns 0 for no error
 */
#define ds3231_get_day(ds3231_handle_pointer, uint16_value_pointer) _ds3231_get_time_and_calendar((ds3231_handle_pointer), DS3231_DAY, (uint16_t *)(uint16_value_pointer))

/**
 * @brief The get date macro
 *
 * Get the date value in a 16 bit variable (range: 1 to 31).
 *
 * @param ds3231_handle_pointer: pointer to a handle of DS3231
 * @param uint16_value_pointer: pointer to a uint16 variable
 * @return Returns 0 for no error
 */
#define ds3231_get_date(ds3231_handle_pointer, uint16_value_pointer) _ds3231_get_time_and_calendar((ds3231_handle_pointer), DS3231_DATE, (uint16_t *)(uint16_value_pointer))

/**
 * @brief The get month macro
 *
 * Get the months value in a 16 bit variable (range: 1 to 12).
 *
 * @param ds3231_handle_pointer: pointer to a handle of DS3231
 * @param uint16_value_pointer: pointer to a uint16 variable
 * @return Returns 0 for no error
 */
#define ds3231_get_month(ds3231_handle_pointer, uint16_value_pointer) _ds3231_get_time_and_calendar((ds3231_handle_pointer), DS3231_MONTH, (uint16_t *)(uint16_value_pointer))

/**
 * @brief The get year macro
 *
 * Get the year value in a 16 bit variable (range: 1900 to 2099).
 *
 * @param ds3231_handle_pointer: pointer to a handle of DS3231
 * @param uint16_value_pointer: pointer to a uint16 variable
 * @return Returns 0 for no error
 */
#define ds3231_get_year(ds3231_handle_pointer, uint16_value_pointer) _ds3231_get_time_and_calendar((ds3231_handle_pointer), DS3231_YEAR, (uint16_t *)(uint16_value_pointer))

	/**
	 * @brief The set all function
	 *
	 * Set all of the time and calendar registers in one function.
	 *
	 * @param handle: pointer to a handle of DS3231
	 * @param time_struct: pointer to a struct of ds3231_time_and_calendar_t to set second, minute, hour, day, date, month, year
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_set_all_time_and_calendar(const ds3231_handle_t *handle, ds3231_time_and_calendar_t *time_struct);

	/**
	 * @brief The set time and calendar register function
	 *
	 * Selectively set the time and calendar registers.
	 *
	 * @param handle: pointer to a handle of DS3231
	 * @param time_register: time and calendar register address
	 * @param value: a uint16 value to be written in time and calendar registers
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t _ds3231_set_time_and_calendar(const ds3231_handle_t *handle, const ds3231_time_register_t time_register, uint16_t value);

/**
 * @brief The set second macro
 *
 * Set seconds macro.
 *
 * @param ds3231_handle_pointer: pointer to a handle of DS3231
 * @param uint16_value: a uint16 value to be written in seconds register
 * @return Returns 0 for no error
 */
#define ds3231_set_second(ds3231_handle_pointer, uint16_value) _ds3231_set_time_and_calendar((ds3231_handle_pointer), DS3231_SECONDS, (uint16_t)(uint16_value))

/**
 * @brief The set minute macro
 *
 * Set minutes macro.
 *
 * @param ds3231_handle_pointer: pointer to a handle of DS3231
 * @param uint16_value: a uint16 value to be written in minutes register
 * @return Returns 0 for no error
 */
#define ds3231_set_minute(ds3231_handle_pointer, uint16_value) _ds3231_set_time_and_calendar((ds3231_handle_pointer), DS3231_MINUTES, (uint16_t)(uint16_value))

/**
 * @brief The set hour macro
 *
 * Set hours macro (range: 0 to 23).
 *
 * @param ds3231_handle_pointer: pointer to a handle of DS3231
 * @param uint16_value: a uint16 value to be written in hours register
 * @return Returns 0 for no error
 */
#define ds3231_set_hour(ds3231_handle_pointer, uint16_value) _ds3231_set_time_and_calendar((ds3231_handle_pointer), DS3231_HOURS, (uint16_t)(uint16_value))

/**
 * @brief The set day macro
 *
 * Set days macro (range: 1 to 7).
 *
 * @param ds3231_handle_pointer: pointer to a handle of DS3231
 * @param uint16_value: a uint16 value to be written in days register
 * @return Returns 0 for no error
 */
#define ds3231_set_day(ds3231_handle_pointer, uint16_value) _ds3231_set_time_and_calendar((ds3231_handle_pointer), DS3231_DAY, (uint16_t)(uint16_value))

/**
 * @brief The set date macro
 *
 * Set date macro (range: 1 to 31).
 *
 * @param ds3231_handle_pointer: pointer to a handle of DS3231
 * @param uint16_value: a uint16 value to be written in date register
 * @return Returns 0 for no error
 */
#define ds3231_set_date(ds3231_handle_pointer, uint16_value) _ds3231_set_time_and_calendar((ds3231_handle_pointer), DS3231_DATE, (uint16_t)(uint16_value))

/**
 * @brief The set month macro
 *
 * Set month macro (range: 1 to 12).
 *
 * @param ds3231_handle_pointer: pointer to a handle of DS3231
 * @param uint16_value: a uint16 value to be written in date register
 * @return Returns 0 for no error
 */
#define ds3231_set_month(ds3231_handle_pointer, uint16_value) _ds3231_set_time_and_calendar((ds3231_handle_pointer), DS3231_MONTH, (uint16_t)(uint16_value))

/**
 * @brief The set year macro
 *
 * Set year macro (range: 1900 to 2099).
 *
 * @param ds3231_handle_pointer: pointer to a handle of DS3231
 * @param uint16_value: a uint16 value to be written in year register
 * @return Returns 0 for no error
 */
#define ds3231_set_year(ds3231_handle_pointer, uint16_value) _ds3231_set_time_and_calendar((ds3231_handle_pointer), DS3231_YEAR, (uint16_t)(uint16_value))

	/**
	 * @brief The is_running function
	 *
	 * Checks if the oscillator is running or is stopped. Oscillator can stop working in battery backed mode or in case of crystal malfunction.
	 *
	 * @param handle: pointer to a handle of DS3231
	 * @param is_running: pointer to a ds3231_bool_t variable that becomes DS3231_TRUE if the oscillator is running.
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_is_running(const ds3231_handle_t *handle, ds3231_bool_t *is_running);

	/**
	 * @brief The BCD to HEX function
	 *
	 * Converts a byte of data from BCD to HEX.
	 *
	 * @param data: an unsigned byte of data
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t _ds3231_bcd_to_hex(uint8_t *data);

	/**
	 * @brief The HEX to BCD function
	 *
	 * Converts a byte of data from HEX to BCD.
	 *
	 * @param data: an unsigned byte of data
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t _ds3231_hex_to_bcd(uint8_t *data);

	/**
	 * @brief The bit get function
	 *
	 * Gets a desired bit from a chosen register.
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 * @param register_address: address of the chosen register
	 * @param register_bit: address of the chosen register bit
	 * @param bit_stat: pointer to a ds3231_bool_t variable that returns the bit status
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t _ds3231_bit_get(const ds3231_handle_t *handle, const ds3231_register_address_t register_address, const ds3231_register_bit_t register_bit, ds3231_bool_t *bit_stat);

	/**
	 * @brief The bit set function
	 *
	 * Sets a desired bit from a chosen register.
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 * @param register_address: address of the chosen register
	 * @param register_bit: address of the chosen register bit
	 * @param bit_stat: a ds3231_bool_t value that sets or resets the chosen bit
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t _ds3231_bit_set(const ds3231_handle_t *handle, const ds3231_register_address_t register_address, const ds3231_register_bit_t register_bit, const ds3231_bool_t bit_value);

	/**
	 * @brief The battery-backed oscillator control function
	 *
	 * Sets DS3231 oscillator to work or stop working if switched to battery power source.
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 * @param bb_osc_control: DS3231_TRUE to work in case of battery power source
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_battery_backed_oscillator_control(const ds3231_handle_t *handle, const ds3231_bool_t bb_osc_control);

	/**
	 * @brief The battery-backed squarewave control function
	 *
	 * Sets DS3231 output squarewave from sqw/int pin to work or stop working in case of battery power source
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 * @param bb_sqw_control: DS3231_TRUE to work in case of battery power source
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_battery_backed_sqw_control(const ds3231_handle_t *handle, const ds3231_bool_t bb_sqw_control);

	/**
	 * @brief The 32KHz output pin control function
	 *
	 * Turns DS3231 32KHz output wave on/off
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 * @param pin_control: DS3231_TRUE to turn on
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_32khz_wave_control(const ds3231_handle_t *handle, const ds3231_bool_t pin_control);

	/**
	 * @brief The SQW/INT pin select function
	 *
	 * Sets the SQW/INT output pin to output squarewave (with configurable frequency) or alarm interrupt
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 * @param output_pin: DS3231_PIN_INTERRUPT to set it to alarm interrupt
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_int_sqw_pin_select(const ds3231_handle_t *handle, const ds3231_int_sqw_pin_t output_pin);

	/**
	 * @brief The squarewave frequency selection function
	 *
	 * Sets the frequency of SQW pin in case of squarewave output
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 * @param wave_freq: frequency of SQW pin,: 1024Hz, 2048Hz, 4096Hz, 8192Hz
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_sqw_output_wave_frequency(const ds3231_handle_t *handle, const ds3231_sqw_output_wave_frequency_t wave_freq);

#if DS3231_INCLUDE_AGING_OFFSET_CALIBRATION
	/**
	 * @brief The aging offset calibration function
	 *
	 * Calibrate the crystal frequency. Negative numbers speed up the frequency, positive numbers slow it down. 0 is the default
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 * @param offset: Negative numbers speed up the frequency, positive numbers slow it down. 0 is the default
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_aging_offset_calibration(const ds3231_handle_t *handle, const int8_t offset);
#define ds3231_aging_offset_calibration_reset(handle_pointer) ds3231_aging_offset_calibration((handle_pointer), ((int8_t)(0)));
#endif

#if DS3231_INCLUDE_TEMPERATURE
#if DS3231_INCLUDE_TEMPERATURE_FLOAT_MATH
	/**
	 * @brief The get temperature function
	 *
	 * Gets temperature as a float number. Requires floating point math and is generally slower and bigger in code size compared to fixed point.
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 * @param temperature: pointer to temperature
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_get_temperature(const ds3231_handle_t *handle, float *temperature);
#else
	/**
	 * @brief The get temperature function
	 *
	 * Gets temperature as a int16 number. Doesn't require floating point math and is generally faster and smaller in size compared to floating point.
	 * Please note that the temperature value is multiplied in 100, as an example a temp of 2575 translates to 25.75, and -1000 translates to -10.0.
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 * @param temperature: pointer to temperature
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_get_temperature(const ds3231_handle_t *handle, int16_t *temperature);
#endif
#endif

#if DS3231_INCLUDE_ALARM_1
	/**
	 * @brief The alarm 1 init function
	 *
	 * Initializes alarm 1 with a struct of ds3231_alarm_1_config_t, also does the rate selection. No need to call ds3231_alarm_1_rate_select after this.
	 * Please note that it does not enable the interrupt or set the output pin to interrupt.
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 * @param config: pointer to ds3231_alarm_1_config_t struct
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_alarm_1_init(const ds3231_handle_t *handle, const ds3231_alarm_1_config_t *config);

	/**
	 * @brief The alarm 1 rate select function
	 *
	 * Selects the alarm 1 rate of alarm from a predefined list of rates.
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 * @param alarm_rate: pointer to ds3231_alarm_1_config_t struct
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_alarm_1_rate_select(const ds3231_handle_t *handle, const ds3231_alarm_1_rate_t alarm_rate);

	/**
	 * @brief The alarm 1 interrupt control function
	 *
	 * Enable or disable the interrupt bit flag. Must be enabled in order to poll the interrupt flag or wait for hardware interrupt.
	 * Please note that it should be enabled before setting the output pin of SQW/INT pin to output external hardware interrupts.
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 * @param enable: enable or disable the interrupt flag report.
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_alarm_1_interrupt_control(const ds3231_handle_t *handle, const ds3231_bool_t enable);

	/**
	 * @brief The alarm 1 interrupt poll function
	 *
	 * Polls the alarm 1 interrupt flag.
	 * Please note that you should enable the interrupt beforehand using ds3231_alarm_1_interrupt_control.
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 * @param flag_bit: pointer to a ds3231_bool_t value passed for polling
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_alarm_1_flag_poll(const ds3231_handle_t *handle, ds3231_bool_t *flag_bit);

	/**
	 * @brief The alarm 1 clear interrupt flag function
	 *
	 * Clears the alarm 1 interrupt flag.
	 * Please note that you should enable the interrupt beforehand using ds3231_alarm_1_interrupt_control.
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 *
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_alarm_1_flag_clear(const ds3231_handle_t *handle);
#endif

#if DS3231_INCLUDE_ALARM_2
	/**
	 * @brief The alarm 2 init function
	 *
	 * Initializes alarm 2 with a struct of ds3231_alarm_2_config_t, also does the rate selection. No need to call ds3231_alarm_2_rate_select after this.
	 * Please note that it does not enable the interrupt or set the output pin to interrupt.
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 * @param config: pointer to ds3231_alarm_2_config_t struct
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_alarm_2_init(const ds3231_handle_t *handle, const ds3231_alarm_2_config_t *config);

	/**
	 * @brief The alarm 2 rate select function
	 *
	 * Selects the alarm 2 rate of alarm from a predefined list of rates.
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 * @param alarm_rate: pointer to ds3231_alarm_2_config_t struct
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_alarm_2_rate_select(const ds3231_handle_t *handle, const ds3231_alarm_2_rate_t alarm_rate);

	/**
	 * @brief The alarm 2 interrupt control function
	 *
	 * Enable or disable the interrupt bit flag. Must be enabled in order to poll the interrupt flag or wait for hardware interrupt.
	 * Please note that it should be enabled before setting the output pin of SQW/INT pin to output external hardware interrupts.
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 * @param enable: enable or disable the interrupt flag report.
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_alarm_2_interrupt_control(const ds3231_handle_t *handle, const ds3231_bool_t enable);

	/**
	 * @brief The alarm 2 interrupt poll function
	 *
	 * Polls the alarm 2 interrupt flag.
	 * Please note that you should enable the interrupt beforehand using ds3231_alarm_2_interrupt_control.
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 * @param flag_bit: pointer to a ds3231_bool_t value passed for polling
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_alarm_2_flag_poll(const ds3231_handle_t *handle, ds3231_bool_t *flag_bit);

	/**
	 * @brief The alarm 2 clear interrupt flag function
	 *
	 * Clears the alarm 2 interrupt flag.
	 * Please note that you should enable the interrupt beforehand using ds3231_alarm_2_interrupt_control.
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_alarm_2_flag_clear(const ds3231_handle_t *handle);
#endif

#if DS3231_INCLUDE_NULL_CHECK
	/**
	 * @brief The null check function
	 *
	 * Checks the interface pointers to avoid NULL values.
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t _ds3231_null_check(const ds3231_handle_t *handle);
#endif

#if DS3231_INCLUDE_WRITE_VERIFICATION
	/**
	 * @brief The bit verification function
	 *
	 * Verifies the bit from a register to an expected value.
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 * @param register_address: address to the chosen register
	 * @param bit_address: address to a chosen bit
	 * @param expected: the expected bit value
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t _ds3231_write_verify_bit(const ds3231_handle_t *handle, const ds3231_register_address_t register_address, const ds3231_register_bit_t bit_address, const ds3231_bool_t expected);

	/**
	 * @brief The byte array verification function
	 *
	 * Verifies the byte array to an expected array of values.
	 *
	 * @param handle: pointer to a ds3231_handle_t handle
	 * @param register_address: address to the starting register
	 * @param number_of_bytes: number of bytes in the array
	 * @param expected: pointer to an array of expected values
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t _ds3231_write_verify_bytes(const ds3231_handle_t *handle, const ds3231_register_address_t register_address, const uint8_t *expected, const uint8_t number_of_bytes);
#endif

#if DS3231_INCLUDE_ERROR_LOG_STRINGS
	/**
	 * @brief The error string function
	 *
	 * Turns the error integer codes into error strings for log and debug.
	 *
	 * @param error_code: the integer error code
	 * @param message: address to a pointer of characters, that will point to the log string
	 * @return Returns 0 for no error
	 */
	ds3231_error_code_t ds3231_error_string(ds3231_error_code_t error_code, char **message);
#endif


#ifdef __cplusplus
}
#endif

#endif

