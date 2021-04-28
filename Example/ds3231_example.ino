/*ds3231 rtc showcase, arduino example main code*/
#include "ds3231.h"

uint8_t time_set[7] = {0, 10, 13, 5, 25, 1, 21};        //array of 7 initial values for seconds (time_set[0]), minutes, hours, day of the week, date, month and year (time_set[6])
uint8_t time_current[7];        //array of 7 bytes to refresh time values from ds3231

void setup() {
  Serial.begin(115200);
  ds3231_init(&time_set[0], CLOCK_HALT, FORCE_RESET);       //&time_set[0] is the starting address of initial values to set the clock for the first time, CLOCK_RUN tells the ds3231 to run after initialization, NO_FORCE_RESET ensures that the mcu will only reset the ds3231 once
}

void loop() {
  ds3231_read(TIME, time_current);        //refreshing the current time, read from ds3231 into time_current array
  
  Serial.print("Current time: ");
  timeToSerial(time_current, 7);
  
  delay(1000);
}

void timeToSerial(uint8_t *data_array, uint8_t array_length)
{
   for (int8_t index = array_length - 1; index >= 0; index--)
  {
    Serial.print(data_array[index]);
    if (index != 0)
      Serial.print(":");
  }
  Serial.println("");
}
