#ifndef CONFIG_H
#define CONFIG_H

// ===== РЕЖИМ РАБОТЫ ПЛАТЫ =====
// Если (0) — прошиваем Приемник (База + UART)
// Если (1) — прошиваем Передатчик (Датчик)
#define IS_TRANSMITTER 1

// ID модуля
#define NODE_ID 2

// Тип датчика если IS_TRANSMITTER == 1
#define NODE_TYPE_MOTION 1  // PIR, движение
#define NODE_TYPE_OC     2  // геркон, открыто/закрыто

#define NODE_TYPE NODE_TYPE_OC

#endif
