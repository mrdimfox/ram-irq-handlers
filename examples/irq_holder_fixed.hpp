#pragma once

#include <cstdint>
#include <iostream>

#include <irq/irq.hpp>

#include <examples/config.hpp>

namespace examples {

class IrqHolderFixed : irq::IrqHandlerFixed<IrqHolderFixed, irq::Irq::USB>
{
    using IrqHandler = irq::IrqHandlerFixed<IrqHolderFixed, irq::Irq::USB>;
    friend IrqHandler;

 public:
    IrqHolderFixed() : IrqHandler(this) {}

 private:
    void call_irq_handler()
    {
        if constexpr (config::examples::IS_PRINT_ENABLED) {
            std::cout << "USB interrupt!"
                      << "\n";
        }

        _is_usb = true;
    }

    bool _is_usb = false;
};

}  // namespace examples