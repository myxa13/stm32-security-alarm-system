#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "nrf24l01.h"
#include "config.h"

void main_hub () {
    
    printf("=== Плата ПРИЕМНИКА запущена (Ожидание сигналов...) ===\r\n");

    while(1) {
        // Проверяем бит RX_DR (Data Ready) в регистре STATUS
        uint8_t status = NRF24_ReadReg(REG_STATUS);

        if (status & (1 << 6)) { // Есть принятые данные!
            AlarmPacket rx_packet;

            // Вычитываем данные из FIFO
            NRF24_ReadBuf(NRF_CMD_R_RX_PAYLOAD, (uint8_t*)&rx_packet, sizeof(AlarmPacket));

            // Сбрасываем флаг приема
            NRF24_WriteReg(REG_STATUS, (1 << 6));

            // Выводим принятые данные в ПК через UART
            printf("\r\n>>> [ТРЕВОГА!] Приняты данные по радио <<<\r\n");
            printf("ID Датчика: %d\r\n", rx_packet.node_id);
            printf("Тип: %d\r\n", rx_packet.node_type);
            if (rx_packet.node_type == NODE_TYPE_OC) {
                printf("Статус: %s\r\n", rx_packet.data ? "ОТКРЫТО" : "ЗАКРЫТО");
            } else {
                printf("Статус: %s\r\n", rx_packet.data ? "ДВИЖЕНИЕ ОБНАРУЖЕНО!" : "Тишина");
            }
            printf("Время отправки: %d ms\r\n", rx_packet.timestamp);
            printf("--------------------------------------\r\n");
        }
    }
}