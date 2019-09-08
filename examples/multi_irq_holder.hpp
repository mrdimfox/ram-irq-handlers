#pragma once

#include <cstdint>
#include <iostream>

#include <examples/config.hpp>
#include <examples/irq_provider.hpp>

namespace examples {

class MultiIrqHolder : public IrqProvider::MultiIrqHandler<
                         MultiIrqHolder,
                         IrqProvider::Irq::USART1,
                         IrqProvider::Irq::DMA>
{
 public:
    MultiIrqHolder() :
      MultiIrqHandler<
        MultiIrqHolder,
        IrqProvider::Irq::USART1,
        IrqProvider::Irq::DMA>(
        this,
        &MultiIrqHolder::_uart1_irq_handler,
        &MultiIrqHolder::_dma_irq_handler)
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