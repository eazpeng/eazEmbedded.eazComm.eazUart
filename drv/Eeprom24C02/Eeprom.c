/*************************************************************************
* Copyright:    Midea Kitchenware(Oven).
* File:         Eeprom.c
* Layer:        Driver.ex
* Author:       eaz
* Version:      0.0.171122
* Brief:        this driver only for 24C02/01
*************************************************************************/
#include "Eeprom.h"
#include "I2C.h"
#include "A31G12x_series.h"
#include "A31G12x_pcu.h"

static void SCL_SET(void);
static void SCL_CLR(void);
static void SDA_SET(void);
static void SDA_CLR(void);
static bool SDA_VALUE(void);

#define P_SDA_0     (GPIO_ClearValue(PC, (1 << 2)))
#define P_SDA_1     (GPIO_SetValue(PC, (1 << 2)))
#define P_SDA       (GPIO_ReadValue(PC) & (1 << 2))
#define P_SCL_0     (GPIO_ClearValue(PC, (1 << 1)))
#define P_SCL_1     (GPIO_SetValue(PC, (1 << 1)))
#define DEVICE_ADDR (0xA0)

static SimuI2C s_xsSimuI2C =
{
    DEVICE_ADDR,

    SCL_SET,
    SCL_CLR,
    SDA_SET,
    SDA_CLR,
    SDA_VALUE
};

/**OK
 * E2WriteByte
 * EEPROM_STATE, EEPROM_ADDR, unsigned char
 * eaz.org.171122
**/
EEPROM_STATE E2WriteByte(EEPROM_ADDR eE2Addr, unsigned char ucData)
{
    I2CStart(&s_xsSimuI2C);
    I2CSendByte(&s_xsSimuI2C, s_xsSimuI2C.ucDeviceAddr & 0xFE); // device address and write bit(0xA0)
    if(I2CWaitACK(&s_xsSimuI2C) != I2C_ACK)
    {
		I2CStop(&s_xsSimuI2C);
        return E2STATE_WR_FAIL;
    }

    I2CSendByte(&s_xsSimuI2C, eE2Addr & 0xFF);
    if(I2CWaitACK(&s_xsSimuI2C) != I2C_ACK)
    {
		I2CStop(&s_xsSimuI2C);
        return E2STATE_WR_FAIL;
    }

    I2CSendByte(&s_xsSimuI2C, ucData);
    if(I2CWaitACK(&s_xsSimuI2C) != I2C_ACK)
    {
		I2CStop(&s_xsSimuI2C);
        return E2STATE_WR_FAIL;
    }

    I2CStop(&s_xsSimuI2C);

    return E2STATE_WR_OK;
}

/**OK
 * E2ReadByte
 * EEPROM_STATE, EEPROM_ADDR, , unsigned char *
 * eaz.org.171122
**/
EEPROM_STATE E2ReadByte(EEPROM_ADDR eE2Addr, unsigned char *pucData)
{
    I2CStart(&s_xsSimuI2C);
    I2CSendByte(&s_xsSimuI2C, s_xsSimuI2C.ucDeviceAddr & 0xFE); // device address and write bit(0xA0)
    if(I2CWaitACK(&s_xsSimuI2C) != I2C_ACK)
    {
		I2CStop(&s_xsSimuI2C);
        return E2STATE_RD_FAIL;
    }

    I2CSendByte(&s_xsSimuI2C, eE2Addr & 0xFF);
    if(I2CWaitACK(&s_xsSimuI2C) != I2C_ACK)
    {
		I2CStop(&s_xsSimuI2C);
        return E2STATE_RD_FAIL;
    }

    I2CStart(&s_xsSimuI2C);
    I2CSendByte(&s_xsSimuI2C, s_xsSimuI2C.ucDeviceAddr | 0x01); // device address and READ bit(0xA1)
    if(I2CWaitACK(&s_xsSimuI2C) != I2C_ACK)
    {
		I2CStop(&s_xsSimuI2C);
        return E2STATE_RD_FAIL;
    }

    I2CReceiveByte(&s_xsSimuI2C, pucData);
    I2CSendNACK(&s_xsSimuI2C);

    I2CStop(&s_xsSimuI2C);
    return E2STATE_RD_OK;
}

/**OK
 * E2ReadBytes
 * EEPROM_STATE, EEPROM_ADDR, unsigned char *, unsigned char
 * eaz.org.171122
**/
EEPROM_STATE E2ReadBytes(EEPROM_ADDR eE2Addr, unsigned char *pucData, unsigned char ucLen)
{
    unsigned char ucCounter;

    I2CStart(&s_xsSimuI2C);
    I2CSendByte(&s_xsSimuI2C, s_xsSimuI2C.ucDeviceAddr & 0xFE); // device address and write bit(0xA0)
    if(I2CWaitACK(&s_xsSimuI2C) != I2C_ACK)
    {
		I2CStop(&s_xsSimuI2C);
        return E2STATE_RD_FAIL;
    }

    I2CSendByte(&s_xsSimuI2C, eE2Addr & 0xFF);
    if(I2CWaitACK(&s_xsSimuI2C) != I2C_ACK)
    {
		I2CStop(&s_xsSimuI2C);
        return E2STATE_RD_FAIL;
    }

    I2CStart(&s_xsSimuI2C);
    I2CSendByte(&s_xsSimuI2C, s_xsSimuI2C.ucDeviceAddr | 0x01); // device address and READ bit(0xA1)
    if(I2CWaitACK(&s_xsSimuI2C) != I2C_ACK)
    {
		I2CStop(&s_xsSimuI2C);
        return E2STATE_RD_FAIL;
    }

    for(ucCounter = 0; ucCounter < ucLen - 1; ucCounter++)
    {
        I2CReceiveByte(&s_xsSimuI2C, pucData);
        I2CSendACK(&s_xsSimuI2C);
    }
    I2CReceiveByte(&s_xsSimuI2C, pucData);
    I2CSendNACK(&s_xsSimuI2C);

    I2CStop(&s_xsSimuI2C);
    return E2STATE_RD_OK;
}

/**OK
 * SDA_SET
 * void, void
 * eaz.org.171122
**/
static void SDA_SET(void)
{
    P_SDA_1;
}

/**OK
 * SDA_CLR
 * void, void
 * eaz.org.171122
**/
static void SDA_CLR(void)
{
    P_SDA_0;
}

/**OK
 * SCL_SET
 * void, void
 * eaz.org.171122
**/
static void SCL_SET(void)
{
    P_SCL_1;
}

/**OK
 * SCL_CLR
 * void, void
 * eaz.org.171122
**/
static void SCL_CLR(void)
{
    P_SCL_0;
}

/**OK
 * SDA_VALUE
 * bool, void
 * eaz.org.171122
**/
static bool SDA_VALUE(void)
{
    // SetDirect
    return P_SDA;
}
