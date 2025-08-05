# DS3231 REAL TIME CLOCK PLATFORM INDEPENDENT C DRIVER

- Reza G. Ebrahimi
- Version 2.0

### INTRODUCTION
Here is the MCU independent, lightweight and portable C driver for DS3231 Real Time Clock. "The DS3231 is a low-cost, extremely accurate I2C real-time clock (RTC) with an integrated temperature-compensated crystal oscillator (TCXO) and crystal". The driver is written using standard C straight from Maxim Integrated datasheet and is not influenced by other libraries. In order to port this library to any MCU, you have to provide the interface API.

### DS3231 DRIVER OVERVIEW
- Device driver handle design pattern for unlimited instances of sensors
- Dependency injection pattern for interface allows for even remote or software mocked module
- Stateless and re-entrant
- Possible to be used with baremetal, RTOS or under any general purpose OS like Linux
- Error handling baked into the driver
- Doxygen style comments
- Config file for configurations and turning features on/off
- Production ready

### DS3231 FEATURES
- Real time clock and calendar.
- Square wave generator with 4 programmable frequencies: 1 Hz, 1.024 KHz, 4.096 KHz, 8.192 KHz (shared pin with interrupt).
- Square wave generator with fixed 32.768 KHz frequency (with a dedicated pin).
- Two programmale time-of-day alarms with an interrupt signal or square wave output.
- A temperature sensor with internal compensation for crystal oscillator.

### PINOUT
- 32KHz: 32kHz Output. This open-drain pin requires an external pullup resistor. When enabled, the output operates on either power supply. It may be left open if not used.
- VCC: DC Power Pin for Primary Power Supply. This pin should be decoupled using a 0.1µF to 1.0µF capacitor. If not used, connect to ground.
- INT/SQW: Active-Low Interrupt or Square-Wave Output. This open-drain pin requires an external pullup resistor connected to a supply at 5.5V or less.
- VBAT: Backup Power-Supply Input.
- SDA: Serial Data Input/Output.
- SCL: Serial Clock Input.

### 24H FORMAT
DS3231 supports two formats to keep hours data, '24H' and '12H + AM/PM'. For the sake of consistency and ease of use, **this driver only implements '24H' format**. All hour data written or read from DS3231 have a range of 0 to 23.

### YEAR FORMAT
DS3231 calendar's year can be set from 1900 up to 2099. For convenience, the device driver accepts/outputs an unsigned 16 bit integer in the range of 1900 to 2099 and does the conversions internally. 

### TIME AND CALENDAR READING
To read time and calendar, the application writer can read the entire time and calendar registers at once using:
```c
ds3231_error_code_t ds3231_get_all_time_and_calendar(const ds3231_handle_t *handle, ds3231_time_and_calendar_t *time_struct);
```
Or use the individual 'get' macros, like so:
```c
ds3231_get_hour(ds3231_handle_pointer, uint16_value_pointer);
```

### TIME AND CALENDAR SETTING
To set time and calendar, the application writer can set the entire time and calendar registers at once using:
```c
ds3231_error_code_t ds3231_set_all_time_and_calendar(const ds3231_handle_t *handle, ds3231_time_and_calendar_t *time_struct);
```
Or use the individual 'set' macros, like so:
```c
ds3231_set_hour(ds3231_handle_pointer, uint16_value)
```

### ALARM FEATURE
There are two individual alarms available on DS3231:
- Alarm 1 has 'seconds' feature, alarm 2 doesn't.
- They can be set to alarm on day of the week or date, and not both. To choose one, the data type of `ds3231_alarm_1_config_t` or `ds3231_alarm_2_config_t` has a member of `ds3231_alarm_day_date_select_t`, that should be `DS3231_ALARM_DAY` in the case of day of the week or `DS3231_ALARM_DATE` in the case of date of the month. As an example for day of the week or date of the month:
```c
/*Day of week example*/
ds3231_alarm_1_config_t config_1;

config_1.day_date_type = DS3231_ALARM_DAY;
config_1.day_date.day = DS3231_MONDAY;
/*Setting the second, minute etc*/
```
```c
/*Date of month example*/
ds3231_alarm_1_config_t config_1;

config_1.day_date_type = DS3231_ALARM_DATE;
config_1.day_date.date = 17;
/*Setting the second, minute etc*/
```
- Alarm rate for both alarms are different. Alarm 1 can alarm once per second, alarm 2 can alarm once per minute along with other different alarm rates which can be set using `ds3231_alarm_1_rate_select` or `ds3231_alarm_2_rate_select`. Please note that alram rate is also set using the alarm init functions.
- There are two ways to get notified by an alarm, one is polling method and the other is external interrupt. As an example for polling:
```c
/*Polling method*/
/*Define the config data*/
ds3231_alarm_1_config_t config_1;

config_1.alarm_rate = DS3231_ALARM1_MATCH_SECOND_MINUTE_HOUR_DAY;
config_1.day_date.day = DS3231_DAY_MONDAY;
config_1.day_date_type = DS3231_ALARM_DAY;
config_1.hour = 23;
config_1.minute = 59;
config_1.second = 59;

/*Initialize the alarm*/
error = ds3231_alarm_1_init(&handle, &config_1);
/*Turn on the interrupt feature*/
error = ds3231_alarm_1_interrupt_control(&handle, DS3231_TRUE);

/*Poll and wait*/
ds3231_bool_t flag;
ds3231_alarm_1_flag_poll(&handle, &flag);

if(flag == DS3231_TRUE)
{
  /*Clear the flag manually*/
  ds3231_alarm_1_flag_clear(&handle);
}
```
- As an example for external interrupt:
```c
/*Polling method*/
/*Define the config data*/
ds3231_alarm_1_config_t config_1;

config_1.alarm_rate = DS3231_ALARM1_MATCH_SECOND_MINUTE_HOUR_DAY;
config_1.day_date.day = DS3231_DAY_MONDAY;
config_1.day_date_type = DS3231_ALARM_DAY;
config_1.hour = 23;
config_1.minute = 59;
config_1.second = 59;

/*Initialize the alarm*/
error = ds3231_alarm_1_init(&handle, &config_1);
/*Turn on the interrupt feature*/
error = ds3231_alarm_1_interrupt_control(&handle, DS3231_TRUE);
/*Set the INT/SQW pin to external interrupt*/
error = ds3231_int_sqw_pin_select(&handle, DS3231_PIN_INTERRUPT);

/*...*/
void external_interrupt_hook()
{
  ds3231_alarm_1_flag_clear(&handle);
  /*Do stuff...*/
}
```

### TEMPERATURE FEATURE
DS3231 comes with an internal temperature sensor that can be read. Temperature is represented with a resolution of 0.25°C. You can optionally turn this feature ON/OFF in config file and also set it to floating point or fixed point math.
- In case of floating point turned on, the final reading is the temperature itself, like 23.75. As an example:
```c
float temperature;
error = ds3231_get_temperature(&handle, &temperature);
```
- In case of fixed point math turned on, the final reading is the temperature multiplied by 100, like 2375. As an example:
```c
int16_t temperature;
error = ds3231_get_temperature(&handle, &temperature);
```

### 32KHZ WAVE OUTPUT
If you want the 32KHz squarewave output, you can turn it on or off with:
```c
ds3231_error_code_t ds3231_32khz_wave_control(const ds3231_handle_t *handle, const ds3231_bool_t enable);
```

### SQW PIN OUTPUT SQUAREWAVE
If you want to use the INT/SQW pin for a frequency configurable squarewave and not the alarm external interrupt feature, you can do so and set the frequency with:
```c
ds3231_error_code_t ds3231_int_sqw_pin_select(const ds3231_handle_t *handle, const ds3231_int_sqw_pin_t output_pin);

ds3231_error_code_t ds3231_sqw_output_wave_frequency(const ds3231_handle_t *handle, const ds3231_sqw_output_wave_frequency_t wave_freq);
```
You can set the output frequency from one of 4 values:
```c
DS3231_SQW_WAVE_1HZ
DS3231_SQW_WAVE_1024HZ
DS3231_SQW_WAVE_4096HZ
DS3231_SQW_WAVE_8192HZ
```

### THREAD SAFETY AND MUTUAL EXCLUSION
If DS3231 is used in an RTOS or general purpose environment, thread safety is important and critical sections must be protected with mutual exclusion lock and unlock. This feature is baked in the driver, and can be turned on or off in the config file. If turned on, the application writer must provide the lock and unlock hooks. **Please note that this lock and unlock feature only protects against race conditions in using the I2C bus and doesn't protect if one DS3231 handle is used in different threads**. For more safety please use a gatekeeper task to access one DS3231 or provide extra locks in your application code to access the same handle from different threads or tasks.

### ERROR HANDLING
Each and every API call returns with an error code, which is `DS3231_ERROR_OK` or 0 in case of no error. If error string logging is turned on, this error code number can be passed to `ds3231_error_string()` to have a log string:
```c
char *log_message;

ds3231_error_code_t error = ds3231_alarm_1_interrupt_control(&handle, DS3231_TRUE);

ds3231_error_string(error, &log_message);

/*perror is not implemented in API*/
perror(log_message);
```

### NAMING CONVENTIONS
All API functions start with `ds3231`, for instance:
```c
ds3231_init()
```
Constants start with `DS3231`, for example:
```c
DS3231_TRUE
```
Datatypes and structures start with `ds3231` and end in `t`, for example:
```c
ds3231_register_address_t
```
**There are internal API functions which are not needed by the end user; Please do not call these functions**. These functions start with `_ds3231`, for example:
```c
_ds3231_null_check()
```

### STRUCTURE OF DEVICE DRIVER
- Although the DS3231 driver can be used with microprocessors under a general purpose OS, it is implemented with embedded microcontrollers in mind. As a result there is no dynamically allocated memory and all features can be individually turned off to save code space. 
- It is re-entrant and thread safe, and critical sections are protected with mutual exclusion under an RTOS. The mutex must be provided by the application writer. 
- The driver itself is state-less, and is driven by passing an instance of a handle. There are no limitations on the number of handles and an infinite number of DS3231 modules can be used simultaneously. 
- Features are structured in individual files and can be turned on/off using macros in config header file. 
- The application writer must provide the low level interface logic, which is the I2C write, read etc. It is also possible to simulate a DS3231 interface and use it as a mock test. 
- Error handling is present in all API functions and a list of predefined errors is present in 'ds3231_error.h'. It's also possible to get error log strings using `ds3231_error_string()`. 
- Many custom data types are defined to help and guide the application writer in choosing valid values.
- The temperature sensor inside DS3231 can be read in a float value or a fixed point value. If concerned about code size, avoid using the float API.
- Optionally, all values read from or written to the DS3231 are range checked to avoid writing invalid values, like 25 for hour or 9 for day.
- Optionally, the DS3231 connection is checked in every API function to check for disconnected or non existent I2C module.
- Optionally, the handle is checked in every API function to avoid passing NULL interface pointers to the API.
- Optionally, the application writer can turn on the write verification in each API function.
- Optionally, the application writer can calibrate the oscillator to run faster or slower by checking the 32KHz squarewave frequency and setting an offset as a feedback in the aging offset calibration API function.

## CONFIG FILE
There is a config header file with 11 configurable macros for turning features ON or OFF. Defining each of these as 1 turns that feature ON and defining each as 0 turns that feature OFF:
1. `DS3231_INCLUDE_SAFE_RANGE_CHECK`: Checks the range of all input or output data to be inside the valid range. You can turn it off if you have double checked the time and calendar numbers, but its recommended to leave this on 1.
2. `DS3231_INCLUDE_WRITE_VERIFICATION`: Verifies all the data written to DS3231 internally. This can be turned off if you have passed the debug stage of your code.
3. `DS3231_INCLUDE_CONNECTION_CHECK`: Checks the existence and I2C connection of the DS3231 in every API function call. Leave this on 1 if you are unsure of the connection to avoid I2C hangups and blocks. The interface I2C ACK code must be implemented by application writer.
4. `DS3231_INCLUDE_NULL_CHECK`: Checks the interface pointers to avoid NULL situation, which causes code failure and hard faults. You can turn it off if you are past the debugging stage.
5. `DS3231_INCLUDE_EXCLUSION_HOOK`: In case of a multithreading environment, lock and unlock hooks must be implemented by the application writer which is used internally by the driver to protect critical sections of I2C access. This is mandatory in RTOS and general purpose OS situations when the I2C bus is used by several threads or tasks.
6. `DS3231_INCLUDE_ALARM_1`: Turns the alarm 1 feature ON or OFF.
7. `DS3231_INCLUDE_ALARM_2`: Turns the alarm 2 feature ON or OFF.
8. `DS3231_INCLUDE_TEMPERATURE`: Turns the temperature reading feature ON or OFF.
9. `DS3231_INCLUDE_TEMPERATURE_FLOAT_MATH`: In case of temperature reading feature turned ON, uses float math to provide the temperature. This is huge in size and resource on most architectures with no floating point unit. **You should turn this feature OFF in most cases and use the fixed point calculations instead**.
10. `DS3231_INCLUDE_AGING_OFFSET_CALIBRATION`: DS3231 comes with the feature to calibrate the oscillator by either making it run faster or slower. You should use this only if you know what you're doing; In other cases, keep this turned off.
11. `DS3231_INCLUDE_ERROR_LOG_STRINGS`: In time of debugging or if you have implemented a logging feature on your application, you can use this `ds3231_error_string()` API function and pass the error code as an argument to get a const character string of the error log.

### HOW TO USE
Start with tweaking the config file to turn your desired features on or off. In the next step, define the handle:
```c
ds3231_handle_t handle;
```
Now provide the interface function pointers with external functions, as an example:
```c
handle.interface.delay_function = ds3231_delay_function;
handle.interface.interface_deinit = ds3231_interface_deinit;
handle.interface.interface_init = ds3231_interface_init;
handle.interface.read_array = ds3231_read_array;
handle.interface.write_array = ds3231_write_array;
/*In case of connection check feature turned on:*/
handle.interface.interface_ack_test = ds3231_interface_ack_test;
/*In case of cmutual exclusion feature turned on:*/
handle.interface.interface_exclusion.interface_lock = ds3231_interface_lock;
handle.interface.interface_exclusion.interface_unlock = ds3231_interface_unlock;
handle.interface.interface_exclusion.mutex_handle = &ds3231_mutex;
```
Please note that all of these functions must be provided by the application writer. If any of these function pointers are left as NULL and the NULL checking feature is turned on, there will be an error. The next step would be calling init:
```c
char *log_message;

ds3231_error_code_t error = ds3231_init(&handle);
ds3231_error_string(error, &log_message);

/*perror() is provided by application writer*/
perror(log_message);

if(error != DS3231_ERROR_OK)
{
  /*Do stuff in case of error*/
}
```
Now the API is up and running and you can interact with DS3231 based on the features you have turned on. For example, in case of reading time and calendar values:
```c
ds3231_time_and_calendar_t time_struct;

ds3231_error_code_t error = ds3231_get_all_time_and_calendar(&handle, &time_struct);
```

### VERSION HISTORY
#### VERSION 2.0
Version 2.0 is a complete rewrite of the device driver.
- Device driver handle design pattern for unlimited instances of sensors
- Dependency injection pattern for interface allows for even remote or software mocked module
- Stateless and re-entrant
- Error handling baked into the driver
- Doxygen style comments
- Config file for configurations and turning features on/off
- Production ready