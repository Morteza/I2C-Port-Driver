#
# make for TI TMS570 I2C Driver Library
#

I2CDriver: I2CPort.o
	cxx -o I2CDriver I2CPort.o -lm

I2CPort.o: i2clib.cpp
	cxx -c I2CPort.cpp

I2CPort.o: I2CPort.h

# end of makefile