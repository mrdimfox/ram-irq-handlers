#pragma once

struct Vectors
{
    void (*dma_irq)();
    void (*usart1_irq)();
    void (*usart2_irq)();
    void (*usb_irq)();
    void (*adc1_irq)();
    void (*adc2_irq)();
};

extern struct Vectors global_irq_vectors;