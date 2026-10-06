#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "nrf24l01.h"

uint8_t TX_ADDRESS[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};

void NRF24_CSN(uint8_t state) {
    HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void NRF24_CE(uint8_t state) {
    HAL_GPIO_WritePin(NRF24_CE_PORT, NRF24_CE_PIN, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void NRF24_WriteReg(uint8_t reg, uint8_t value) {
    uint8_t buf[2] = { NRF_CMD_W_REGISTER | (reg & 0x1F), value };
    NRF24_CSN(0);
    HAL_SPI_Transmit(&hspi1, buf, 2, HAL_MAX_DELAY);
    NRF24_CSN(1);
}

uint8_t NRF24_ReadReg(uint8_t reg) {
    uint8_t tx[2] = { reg & 0x1F, 0xFF };
    uint8_t rx[2] = { 0 };
    NRF24_CSN(0);
    HAL_SPI_TransmitReceive(&hspi1, tx, rx, 2, HAL_MAX_DELAY);
    NRF24_CSN(1);
    return rx[1];
}

void NRF24_WriteBuf(uint8_t cmd, uint8_t *pBuf, uint8_t len) {
    NRF24_CSN(0);
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(&hspi1, pBuf, len, HAL_MAX_DELAY);
    NRF24_CSN(1);
}

void NRF24_ReadBuf(uint8_t cmd, uint8_t *pBuf, uint8_t len) {
    NRF24_CSN(0);
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, pBuf, len, HAL_MAX_DELAY);
    NRF24_CSN(1);
}

void NRF24_Init(uint8_t type) {
    NRF24_CE(0); // Выключаем радио на время настройки
    HAL_Delay(10);

    NRF24_WriteReg(REG_RF_CH, 76); // Канал 76 (2476 МГц — вне зоны Wi-Fi)
    NRF24_WriteReg(REG_RF_SETUP, 0x06); // Скорость 1 Мбит/с, мощность 0 dBm (максимум)
    NRF24_WriteReg(REG_RX_PW_P0, sizeof(AlarmPacket)); // Фиксированный размер пакета

    // Записываем адрес трубы 0
    NRF24_WriteBuf(NRF_CMD_W_REGISTER | REG_TX_ADDR, TX_ADDRESS, 5);
    NRF24_WriteBuf(NRF_CMD_W_REGISTER | REG_RX_ADDR_P0, TX_ADDRESS, 5);

    if (type) {
        // Настройка Передатчика: Включаем питание (PWR_UP=1), режим TX (PRIM_RX=0), CRC 2 байта
        NRF24_WriteReg(REG_CONFIG, 0x0E); 
    } else if (type == 0) {
        // Настройка Приемника: Включаем питание (PWR_UP=1), режим RX (PRIM_RX=1), CRC 2 байта
        NRF24_WriteReg(REG_CONFIG, 0x0F); 
        NRF24_CE(1); // Приемник должен непрерывно слушать эфир
    } else {
        printf("[ERROR] Ошибка инициации модуля");
    }

    HAL_Delay(2); // Время на запуск кварца радиочипа
}