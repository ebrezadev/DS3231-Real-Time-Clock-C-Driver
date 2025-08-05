#include <Arduino.h>
#include "ds3231.h"
#include "interface.h"

ds3231_handle_t handle;
ds3231_error_code_t error;
ds3231_time_and_calendar_t time_struct;
char *log_message;

const char *month[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
const char *day[] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};

#define PRINT_ERROR(str, error)                  \
	do                                           \
	{                                            \
		Serial.print(str);                       \
		ds3231_error_string(error, &log_message); \
		Serial.println(log_message);             \
	} while (0)

void setup()
{
	Serial.begin(115200);

	while (!Serial)
		;

	handle.interface.delay_function = ds3231_delay_function;
	handle.interface.interface_deinit = ds3231_interface_deinit;
	handle.interface.interface_init = ds3231_interface_init;
	handle.interface.read_array = ds3231_read_array;
	handle.interface.write_array = ds3231_write_array;
	handle.interface.interface_ack_test = ds3231_interface_ack_test;

	error = ds3231_init(&handle);
	PRINT_ERROR("INIT ERR: ", error);

	if (error == DS3231_ERROR_DS3231_NOT_CONNECTED)
	{
		PRINT_ERROR("DS3231 NOT CONNECTED ", error);
		for (;;)
			;
	}

	error = ds3231_reset_all(&handle);
	PRINT_ERROR("RESET ERR: ", error);

	error = ds3231_set_hour(&handle, 15);
	PRINT_ERROR("SET ERR: ", error);

	time_struct.year = 1999;
	time_struct.month = DS3231_MONTH_DECEMBER;
	time_struct.day = DS3231_DAY_MONDAY;
	time_struct.date = 31;
	time_struct.minute = 59;
	time_struct.hour = 23;
	time_struct.second = 50;

	error = ds3231_set_all_time_and_calendar(&handle, &time_struct);
	PRINT_ERROR("SET ALL ERR: ", error);

	error = ds3231_32khz_wave_control(&handle, DS3231_FALSE);
	PRINT_ERROR("32K ERR: ", error);

	error = ds3231_int_sqw_pin_select(&handle, DS3231_PIN_SQUAREWAVE);
	PRINT_ERROR("SQW ERR: ", error);

	error = ds3231_sqw_output_wave_frequency(&handle, DS3231_SQW_WAVE_1024HZ);
	PRINT_ERROR("SQW FREQ ERR: ", error);

	ds3231_alarm_1_config_t config_1;

	config_1.alarm_rate = DS3231_ALARM1_MATCH_SECOND_MINUTE_HOUR_DAY;
	config_1.day_date.day = DS3231_DAY_MONDAY;
	config_1.day_date_type = DS3231_ALARM_DAY;
	config_1.hour = 23;
	config_1.minute = 59;
	config_1.second = 59;
	error = ds3231_alarm_1_init(&handle, &config_1);
	PRINT_ERROR("ALARM1 ERR: ", error);

	error = ds3231_alarm_1_interrupt_control(&handle, DS3231_TRUE);
	PRINT_ERROR("INTCONT ERR: ", error);
	error = ds3231_int_sqw_pin_select(&handle, DS3231_PIN_INTERRUPT);
	PRINT_ERROR("INTPIN ERR: ", error); 
}

void loop()
{
	error = ds3231_get_all_time_and_calendar(&handle, &time_struct);
	PRINT_ERROR("GETALL ERR: ", error);
 
	if (error == DS3231_ERROR_DS3231_NOT_CONNECTED)
	{
		PRINT_ERROR("DS3231 NOT CONNECTED ", error);
		for (;;)
			;
	}
 
	Serial.print(time_struct.year);
	Serial.print(" ");
	Serial.print(month[time_struct.month - 1]);
	Serial.print(" ");
	Serial.print(time_struct.date);
	Serial.print(" ");
	Serial.print(day[time_struct.day - 1]);
	Serial.print(" ");
	Serial.print(time_struct.hour);
	Serial.print(" ");
	Serial.print(time_struct.minute);
	Serial.print(" ");
	Serial.print(time_struct.second);
	Serial.print(" ");
	Serial.println(" ");

	float temperature;
	error = ds3231_get_temperature(&handle, &temperature);
	PRINT_ERROR("TEMP ERR: ", error);
 
	if (error == DS3231_ERROR_DS3231_NOT_CONNECTED)
	{
		PRINT_ERROR("DS3231 NOT CONNECTED ", error);
		for (;;)
			;
	}
	Serial.println(temperature);

  ds3231_alarm_1_flag_clear(&handle);
	delay(1000);
}

