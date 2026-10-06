#ifndef NRF24L01_H
#define NRF24L01_H

#include "stm32f4xx_hal.h"

extern SPI_HandleTypeDef hspi1;

// NRF24L01 Control command registers

#define NRF24_CSN_PORT GPIOA
#define NRF24_CSN_PIN GPIO_PIN_4

#define NRF24_CE_PORT GPIOB
#define NRF24_CE_PIN GPIO_PIN_0

#define PIR_SENSOR_PORT GPIOA
#define PIR_SENSOR_PIN GPIO_PIN_1

#define NRF_CMD_R_REGISTER 0x00
#define NRF_CMD_W_REGISTER 0x20
#define NRF_CMD_R_RX_PAYLOAD 0x61
#define NRF_CMD_W_TX_PAYLOAD 0xA0
#define NRF_CMD_FLUSH_TX 0xE1
#define NRF_CMD_FLUSH_RX 0xE2

#define REG_CONFIG 0x00
#define REG_EN_AA 0x01
#define REG_EN_RXADDR 0x02
#define REG_SETUP_AW 0x03
#define REG_SETUP_RETR 0x04
#define REG_RF_CH 0x05
#define REG_RF_SETUP 0x06
#define REG_STATUS 0x07
#define REG_RX_PW_P0 0x11
#define REG_TX_ADDR 0x10
#define REG_RX_ADDR_P0 0x0A

typedef struct {
    uint8_t node_id;
    uint8_t node_type;  // NODE_TYPE_MOTION / NODE_TYPE_OC
    uint8_t data;
    uint8_t timestamp;
} AlarmPacket;

extern uint8_t TX_ADDRESS[5];

void NRF24_CSN(uint8_t state);
void NRF24_CE(uint8_t state);
void NRF24_WriteReg(uint8_t reg, uint8_t value);
uint8_t NRF24_ReadReg(uint8_t reg);
void NRF24_WriteBuf(uint8_t cmd, uint8_t *pBuf, uint8_t len);
void NRF24_ReadBuf(uint8_t cmd, uint8_t *pBuf, uint8_t len);
void NRF24_Init(uint8_t type);

#endif