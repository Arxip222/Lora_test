#ifndef MAIN_H
#define MAIN_H

#include "stm32f10x.h"
void SPI_Init(void);
void SPI_SendByte(uint8_t data);
void LoRa_SendData(uint8_t *data, uint8_t len);

#endif