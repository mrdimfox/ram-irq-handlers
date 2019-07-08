#pragma once

#include <cstdint>
#include <iostream>

#include <irq/irq.hpp>

#include <examples/config.hpp>

namespace examples {

class IrqHolder : irq::IrqHandler<IrqHolder, irq::Irq::SPI1>
{
 public:
    IrqHolder() :
      irq::IrqHandler<IrqHolder, irq::Irq::SPI1>(
        this,
        &IrqHolder::_spi1_irq_handler)
    {
    }

 private:
    void _spi1_irq_handler()
    {
        if constexpr (config::examples::IS_PRINT_ENABLED) {
            std::cout << "SPI1 interrupt!"
                      << "\n";
        }

        _is_spi = true;
    }

    bool _is_spi = false;
};

}  // namespace examples