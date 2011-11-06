#include "I2CPort.h"

I2CPort::I2CPort() : _i2cDelay(30)
{
    init();
}

void I2CPort::init()
{

    //TODO: how to set USIU
    //*USIU.SIUMCR.B.GPC = 0x1;


    //! re-enable SDA-line

    this->delay();
    this->putSCL(false);                       //! preset SCL=0 to avoid accidential

    //! transmission of START condition
    //! by following SDA-transition

    delay();
    putSDA(false);                       //! drive SDA to inactive state
    delay();
    putSCL(true);                       //! set SCL for the first time
    delay();
    putSCL(false);                       //! release SCL
    delay();
    putSCL(true);                       //! set SCL for the second time
    delay();
    putSCL(false);                       //! release SCL
    delay();
    putSCL(true);                       //! set SCL for the third time

    //! I2C-bus is now going in the inactive state by the following commands

    delay();
    putSDA(true);                       //! this is also the state after RESET
    delay();
    putSDA(true);                       //! setup SCL for START condition

}

BYTE I2CPort::write(unsigned char *source, unsigned int destinationAddress, unsigned char deviceID, unsigned int size)
{
    BYTE result;

    start();
    writeByte((BYTE)deviceID & 0xFE);

    if((result = receiveAck()) != I2C_SUCCESSFUL)
    {
       stop();
       return result;
    }


    if(destinationAddress & 0x80000000)
     {

         writeByte((BYTE) (destinationAddress>>8));

         if((result = receiveAck()) != I2C_SUCCESSFUL)
         {
            stop();
            return result;
         }
     }

    writeByte((BYTE) destinationAddress);

    if((result = receiveAck()) != I2C_SUCCESSFUL)
    {
       stop();
       return result;
    }

    unsigned int _size = size;
    unsigned char *_source = source;
    while(_size--)
    {
       writeByte(*(_source++));

       if((result = receiveAck()) != I2C_SUCCESSFUL)
       {
          stop();
          return(result);
       }
    }

    stop();
    return I2C_SUCCESSFUL;
}

BYTE I2CPort::read(unsigned char *destination, unsigned int sourceAddress, unsigned char deviceID, unsigned int size)
{
    BYTE result;

    start();
    writeByte(deviceID & 0xFE);

    if((result = receiveAck()) != I2C_SUCCESSFUL) {
       stop();
       return result;
    }

     if(sourceAddress & 0x80000000)
     {
         writeByte((BYTE) (sourceAddress>>8));

         if((result = receiveAck()) != I2C_SUCCESSFUL)
         {
            stop();
            return result;
         }
     }

    writeByte((BYTE) sourceAddress);

    if((result = receiveAck()) != I2C_SUCCESSFUL)
    {
       stop();
       return result;
    }

    start();
    writeByte(deviceID | 0x01);

    if((result = receiveAck()) != I2C_SUCCESSFUL) {
       stop();
       return result;
    }

    unsigned int _size = size;
    while(_size--)
    {
       *(destination++) = readByte();
       if(_size)
          sendAck();
    }

    stop();
    return I2C_SUCCESSFUL;
}

void I2CPort::start()
{
    delay();
    putSDA(true);
    delay();
    putSCL(true);
    delay();
    putSDA(false);
    delay();
    putSCL(false);
}

void I2CPort::stop()
{
    delay();
    putSCL(false);
    delay();
    putSDA(false);
    delay();
    putSCL(true);
    delay();
    putSDA(true);
}


void I2CPort::sendAck()
{
    sendBit(0);
}

BYTE I2CPort::receiveAck()
{
    if (receiveBit() == 0)
       return I2C_SUCCESSFUL;
    else
       return I2C_NO_ACKNOWLEDGE;
}

void I2CPort::sendBit(BYTE state)
{
    bool _state = (state?1:0);

    delay();
    putSCL(false);
    delay();
    putSDA(_state);
    delay();
    putSCL(true);

    delay();

    while(!getSCL());

    delay();
    putSCL(false);

    delay();
    putSDA(true);

}

BYTE I2CPort::receiveBit()
{
    bool state;

    delay();
    putSCL(false);
    delay();
    putSDA(true);
    delay();
    putSCL(true);

    delay();

    while(!getSCL());

    delay();
    state = getSDA();
    putSCL(false);
    delay();
    return (state?1:0);
}


void I2CPort::writeByte(BYTE data)
{
    BYTE _tempData = data;

    for(int i=0; i <= 7; i++)
    {
       sendBit((_tempData & 0x80) == 0x80);
       _tempData = (_tempData << 1);
    }
}

BYTE I2CPort::readByte()
{
    BYTE value = 0;

    for(int i = 0; i <= 7; i++)
    {
       value = (value << 1);
       value = value | receiveBit();
    }

    return value;

}

void I2CPort::delay()
{
    for(int count = 0; count < this->_i2cDelay; count++);
}

void I2CPort::putSCL(bool state)
{
    if(!state)
    {
        SCLCLEAR;
        SCLOUTPUT;
        SCLCLEAR;
    }
    else
        SCLINPUT;

}

BYTE I2CPort::getSCL()
{
    SCLINPUT;

    //! read SCL-line status
    if(SCLSTAT)
        return true;
    else
        return false;

}

void I2CPort::putSDA(bool state)
{
    if(!state)
    {
         SDACLEAR;
         SDAOUTPUT;
         SDACLEAR;
    }
    else
         SDAINPUT;
}

BYTE I2CPort::getSDA()
{
    SDAINPUT;

    //! read SDA-line status
    if(SDASTAT)
        return true;
    else
        return false;

}

void I2CPort::setDelayValue(int delay)
{
    this->_i2cDelay = delay;
}
