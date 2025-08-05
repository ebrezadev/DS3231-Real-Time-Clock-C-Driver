#include <stdio.h>
#include "ds3231.h"
#include "interface.h"

ds3231_handle_t handle;
ds3231_error_code_t error;
ds3231_time_and_calendar_t time_struct;
char *log_message;

const char *month[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
const char *day[] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};

#define PRINT_ERROR(str, error)                         \
	do                                                  \
	{                                                   \
		ds3231_error_string(error, &log_message);       \
		printf("%s %s\n", str, log_message); \
	} while (0)

int main()
{
	printf("HELLO\n");
	handle.interface.delay_function = ds3231_delay_function;
	handle.interface.interface_deinit = ds3231_interface_deinit;
	handle.interface.interface_init = ds3231_interface_init;
	handle.interface.read_array = ds3231_read_array;
	handle.interface.write_array = ds3231_write_array;
	handle.interface.interface_ack_test = ds3231_interface_ack_test;

	error = ds3231_init(&handle);
	PRINT_ERROR("INIT ERR:", error);
	
	if (error == DS3231_ERROR_DS3231_NOT_CONNECTED)
	{
		PRINT_ERROR("DS3231 NOT CONNECTED", error);
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

	error = ds3231_32khz_wave_control(&handle, DS3231_TRUE);
	PRINT_ERROR("32K ERR: ", error);

	error = ds3231_int_sqw_pin_select(&handle, DS3231_PIN_SQUAREWAVE);
	PRINT_ERROR("SQW ERR: ", error);

	error = ds3231_sqw_output_wave_frequency(&handle, DS3231_SQW_WAVE_1024HZ);
	PRINT_ERROR("SQW FREQ ERR: ", error);

	error = ds3231_int_sqw_pin_select(&handle, DS3231_PIN_INTERRUPT);
	PRINT_ERROR("INTPIN ERR: ", error);

	for (;;)
	{
		error = ds3231_get_all_time_and_calendar(&handle, &time_struct);
		// PRINT_ERROR("GETALL ERR: ", error);
		if (error == DS3231_ERROR_DS3231_NOT_CONNECTED)
		{
			PRINT_ERROR("DS3231 NOT CONNECTED ", error);
			for (;;)
				;
		}

		printf("%d %d %d %d %d %d %d\n", time_struct.year, time_struct.month, time_struct.date, time_struct.day, time_struct.hour, time_struct.minute, time_struct.second);
		float temperature;
		error = ds3231_get_temperature(&handle, &temperature);
		PRINT_ERROR("TEMP ERR: ", error);
		if (error == DS3231_ERROR_DS3231_NOT_CONNECTED)
		{
			PRINT_ERROR("CONNECTION ERROR", error);
			for (;;)
				;
		}

		printf("%f\n", temperature);
		// ds3231_alarm_1_flag_clear(&handle);
		sleep(1);
	}

	return 0;
}
