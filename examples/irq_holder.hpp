#pragma once

#include <cstdint>
#include <iostream>

#include <irq/irq.hpp>

#include <examples/config.hpp>

namespace examples {

class IrqHolder
  : public irq::MultiIrqHandler<IrqHolder, irq::Irq::USART1, irq::Irq::DMA>
{
 public:
    IrqHolder() :
      MultiIrqHandler<IrqHolder, irq::Irq::USART1, irq::Irq::DMA>(
        this,
        &IrqHolder::_uart1_irq_handler,
        &IrqHolder::_dma_irq_handler)
    {
    }

 private:
    void _uart1_irq_handler()
    {
        if constexpr (config::examples::IS_PRINT_ENABLED) {
            std::cout << "UART2 interrupt!"
                      << "\n";
        }

        _is_usart = true;
    }

    void _dma_irq_handler()
    {
        if constexpr (config::examples::IS_PRINT_ENABLED) {
            std::cout << "DMA interrupt!"
                      << "\n";
        }

        _is_dma = true;
    }

    bool _is_dma = false;
    bool _is_usart = false;
};

}  // namespace examples