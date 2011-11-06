#ifndef I2CPORT_H
#define I2CPORT_H


// Ports definitions and settings
#define SGPIODT2	(* (volatile int *) 0x2fc028) //  SCL-SDA data reg
#define SGPIOCR		(* (volatile int *) 0x2fc02c) //  ACL-SDA dir reg

#define SCLINPUT	SGPIOCR &= 0xFEFFFFFF
#define SCLOUTPUT	SGPIOCR |= 0x01000000

#define SDAINPUT	SGPIOCR &= 0xFDFFFFFF
#define SDAOUTPUT	SGPIOCR |= 0x02000000

#define SCLCLEAR	SGPIODT2 &= 0xFEFFFFFF
#define SCLSET		SGPIODT2 |= 0x01000000
#define SCLSTAT		SGPIODT2 &  0x01000000

#define SDACLEAR	SGPIODT2 &= 0xFDFFFFFF
#define SDASET		SGPIODT2 |= 0x02000000
#define SDASTAT		SGPIODT2 &  0x02000000


// Standard datatypes
#define  BYTE  unsigned char
#define  WORD  unsigned int
#define  DWORD unsigned long
#define  BCD8  unsigned char
#define  BCD16 unsigned int
#define  NULL  0


// I2C port constants
#define  I2C_SUCCESSFUL       0x00
#define  I2C_NO_ACKNOWLEDGE   0xFE
#define  I2C_NOT_IMPLEMENTED  0xFF


class I2CPort
{
public:
    I2CPort();
    void init();
    BYTE write(BYTE *source, WORD destinationAddress, BYTE deviceID, WORD size);
    BYTE read(BYTE *destination, WORD sourceAddress, BYTE deviceID, WORD size);

    void start();
    void stop();
    void sendAck();
    BYTE receiveAck();
    void sendBit(BYTE state);
    BYTE receiveBit();
    void writeByte(BYTE data);
    BYTE readByte();

    void delay();

    void putSCL(bool state);
    BYTE getSCL();

    void putSDA(bool state);
    BYTE getSDA();

private:
        int _i2cDelay;
        int getDelayValue();
        void setDelayValue(int delay);
};

#endif // I2CPORT_H
