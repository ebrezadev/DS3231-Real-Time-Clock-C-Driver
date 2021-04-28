# DS3231 C DRIVER
  ## INTRODUCTION
Here is the MCU independent, lightweight and portable C driver for DS3231 Real Time Clock. "The DS3231 is a low-cost, extremely accurate I2C real-time clock (RTC) with an integrated temperature- compensated crystal oscillator (TCXO) and crystal". The driver is written using standard C straight from Maxim Integrated datasheet and is not influenced by other libraries. CPU intensive tasks are avoided, and mathematical expressions are written using bitwise operators. In order to port this library to any MCU, you have to tweak the low level API file.

## FEATURES
The DS3231 has four main features:

•	Real time BCD clock with stop/start and AM/PM pr 24 hours capabilities.

•	Square wave generator with 5 programmable frequencies: 1 Hz, 1.024 KHz, 4.096 KHz, 8.192 KHz and 32.768 KHz (with a dedicated pin, related to alarm functions).

•	Two programmale time-of-day alarms with an interrupt signal or square wave output.

•	A temperature sensor with internal compensation for crystal oscillator.

All of these features are utilized in this library. You can initialize DS3231 using DS3231_init() (with automatic reset, since DS3231 is battery backed and if MCU is reset or out of power, DS3231 continues to keep time so there’s no need to reset the DS3231 everytime our firmware starts).

## HOW TO USE
First, you need to tweak the low level file for your MCU (it consists of I2C configurations) which is all you have to do to start using DS3231. In the next step, use DS3231_init(uint8_t *data_array, uint8_t run_state, uint8_t reset_state) to start using DS3231. DS3231_init sets up the I2C connection, sets up the time using your input (data_array[7], in case of a reset), starts or stops the time progression (run_state = CLOCK_START or run_state = CLOCK_HALT) (PLEASE NOTE: RUNNING OR HALTING THE OSCILLATOR ONLY WORKS IN BATTERY BACKED SITUATIONS) and force resets (or not) the internal registers of DS3231 (reset_state = FORCE_RESET which resets DS3231 and sets up time from beginning using data_array[], or NO_FORCE_RESET which will automatically not reset DS3231, if it has been set up once and resets DS3231 if it is freshly powered up for the first time).

Now, you can read time by DS3231_read(TIME, time_array); time_array is an array of 7 bytes to read all the time registers inside DS3231. Instead of TIME, you can use these keywords to load your preferred registers from DS3231: SECOND (1 byte), MINUTE (1 byte), HOUR (1 byte), DAY_OF_WEEK, DATE (1 byte), MONTH (1 byte), YEAR (1 byte), CONTROL (1 byte), TIME (7 bytes).

After initializing, you can use DS3231_reset(ALL) to clear DS3231 to its initial zero values (time settings are lost) or DS3231_reset(SECOND) or any other register to reset them one by one. Next, you can set the time registers again, using DS3231_set(TIME, time_set) in which time_set is an array of 7 bytes.

Other usable function is DS3231_run(CLOCK_RUN *or* CLOCK_HALT) to run or halt the clock (PLEASE NOTE: RUNNING OR HALTING THE OSCILLATOR ONLY WORKS IN BATTERY BACKED SITUATIONS).

DS3231 reports time in BCD format. For the ease of use, the driver automatically handles all the conversions between BCD to HEX and HEX to BCD.

The AM/PM or 24 hours capability is set to 24 hours by default and cannot be changed. 

## HOW IT WORKS
Different functions in this library can be categorized into different levels of abstraction from low level functions dealing with I2C hardware, up to higher level functions reporting back time and etc.

### LEVEL 1:
void DS3231_I2C_init

void time_i2c_write_single

void time_i2c_write_multi

void time_i2c_read_single

void time_i2c_read_multi

### LEVEL 2:
void DS3231_reset

uint8_t DS3231_set

uint8_t DS3231_run

uint8_t DS3231_run_state

uint8_t DS3231_read

uint8_t DS3231_init_status_report

void DS3231_init_status_update

### LEVEL 3:
uint8_t DS3231_init

## TO BE ADDED
Features like alarms, temperature sensor and squarewave generator are yet to be added.
