# DS3231 REAL TIME CLOCK PLATFORM INDEPENDENT C DRIVER

- Reza G. Ebrahimi
- Version 2.0

### Linux example

In order to compile:
```c
gcc -I. -I./inc/ *.c ./src/*.c -o main.out -lm
```
Or:
```c
make
```
In order to run, set an env variable to select the I2C device path. It would be '/dev/i2c-1' if not provided (RPi).
```bash
I2C_DEV_PATH=/dev/i2c-2 ./main.out
```
