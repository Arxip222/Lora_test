#include "main.h"
uint16_t received_data;

int main(void) {
    PORTS_INIT();

    while(1) 
		{
			
    }
}
void PORTS_INIT(void) {
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;

    // Настройка пина SPI1 SCK (PA5)
    GPIOA->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5); // Очистить биты MODE и CNF
    GPIOA->CRL |= GPIO_CRL_MODE5_1 | GPIO_CRL_MODE5_0 | GPIO_CRL_CNF5_1; // Режим "Выход, альтернативная функция push-pull"

    // Настройка пина SPI1 MISO (PA6)
    GPIOA->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6); // Очистить биты MODE и CNF
    GPIOA->CRL |= GPIO_CRL_CNF6_1; // Режим "Вход, pull-up"

    // Настройка пина SPI1 MOSI (PA7)
    GPIOA->CRL &= ~(GPIO_CRL_MODE7 | GPIO_CRL_CNF7); // Очистить биты MODE и CNF
    GPIOA->CRL |= GPIO_CRL_MODE7_1 | GPIO_CRL_MODE7_0 | GPIO_CRL_CNF7_1; // Режим "Выход, альтернативная функция push-pull"

    // Настройка пина NSS (PA4) для управления модулем LoRa
    GPIOA->CRL &= ~(GPIO_CRL_MODE4 | GPIO_CRL_CNF4); // Очистить биты MODE и CNF
    GPIOA->CRL |= GPIO_CRL_MODE4_1 | GPIO_CRL_MODE4_0; // Режим "Выход, альтернативная функция push-pull"
    GPIOA->BSRR = GPIO_BSRR_BS4; // Установить NSS в высокий уровень
	
		GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1); // Очистить биты MODE и CNF
		GPIOA->CRL |= GPIO_CRL_CNF1_0; // Режим "Вход, floating input"

		// Настройка пина RST (пример: PA2)
		GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2); // Очистить биты MODE и CNF
		GPIOA->CRL |= GPIO_CRL_MODE2_1; // Режим "Выход, альтернативная функция push-pull"
		GPIOA->BSRR = GPIO_BSRR_BS2; // Установить RST в высокий уровень (сброс неактивен)
		/*
		RST
		0 - сброс
		1 - работа
		
		DIO0 - READ
		*/
		
    SPI1->CR1 = 0;
		SPI1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA); //Настройка тактов и фазы (из даташита лоры)
		SPI1->CR1 |= SPI_CR1_MSTR; //Master Mode
		SPI1->CR1 |= SPI_CR1_BR_0; //Baud rate control Fpclk/2
		SPI1->CR1 &= ~SPI_CR1_LSBFIRST; //MSB first
		SPI1->CR1 &= ~SPI_CR1_DFF; //8-bit fprmat transmitter
		SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI; // SSM = 0 - automatic contol NSS, SSI = 0 - deactivate software NSS control
		SPI1->CR1 &= ~SPI_CR1_CRCNEXT; //CRC transfer next disable
		SPI1->CR1 &= ~SPI_CR1_CRCEN; //Hardware CRC calculation disable
		SPI1->CR1 &= ~SPI_CR1_RXONLY; //Receive only
		SPI1->CR1 &= ~SPI_CR1_BIDIMODE; //1-line data mode
		SPI1->CR1 |= SPI_CR1_SPE; //Включение SPI
		
		SPI1->CR2 &= ~SPI_CR2_RXDMAEN; //Rx DMA disable
		SPI1->CR2 &= ~SPI_CR2_TXDMAEN; //Tx DMA disable
		SPI1->CR2 &= ~SPI_CR2_SSOE; //SS output disable
		SPI1->CR2 &= ~SPI_CR2_ERRIE; //Err interrupt disable
		SPI1->CR2 |= SPI_CR2_RXNEIE; //Rx not empty interrupt enable
		SPI1->CR2 &= ~SPI_CR2_TXEIE; //Tx interrupt disable
}

void SPI1_IRQHandler(void) {
    if (SPI1->SR & SPI_SR_RXNE) {
        received_data = SPI1->DR;
    }
}
