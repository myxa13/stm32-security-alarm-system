#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "nrf24l01.h"
#include "config.h"

void magnit_hub () {
    printf("=== Плата ПЕРЕДАТЧИКА запущена ===\r\n");
    uint8_t last_state = 0;

    while(1) {
     // Считываем состояние датчика SR505 (подключен к PA1)
        uint8_t current_state = HAL_GPIO_ReadPin(GPIOA, PIR_SENSOR_PIN);

        // Отправляем пакет при сработке движения
        if (current_state == GPIO_PIN_SET && last_state == 0) {
            AlarmPacket packet;
            packet.node_id = NODE_ID;
            packet.node_type = NODE_TYPE;
            packet.data = 1;
            packet.timestamp = HAL_GetTick();

            // 1. Очищаем FIFO и заталкиваем пакет
            uint8_t cmd_flush = NRF_CMD_FLUSH_TX;
            NRF24_CSN(0);
            HAL_SPI_Transmit(&hspi1, &cmd_flush, 1, HAL_MAX_DELAY);
            NRF24_CSN(1);

            NRF24_WriteBuf(NRF_CMD_W_TX_PAYLOAD, (uint8_t*)&packet, sizeof(AlarmPacket));

            // 2. Импульс CE для старта передачи по воздуху
            NRF24_CE(1);
            HAL_Delay(1);
            NRF24_CE(0);

            // 3. Сбрасываем флаги в STATUS
            NRF24_WriteReg(REG_STATUS, 0x70);

            printf("[%lu ms] Магнит обнаружено! Пакет отправлен в эфир.\r\n", (unsigned long)packet.timestamp);
            HAL_Delay(2000); // Задержка от дребезга/повторных сработок датчика
        }
        last_state = current_state;
    }
}