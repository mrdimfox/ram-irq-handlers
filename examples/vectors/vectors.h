#pragma once

#include <stdint.h>

struct Vectors
{
    void (*dma_irq)();
    void (*usart1_irq)();
    void (*usart2_irq)();
    void (*usb_irq)();
    void (*adc1_irq)();
    void (*adc2_irq)();
    void (*spi1_irq)();
};

extern struct Vectors global_irq_vectors;

enum Irq
{
    DMA = (uint8_t)(0),
    USART1,
    USART2,
    USB,
    ADC1,
    ADC2,
    SPI1
    // others
};