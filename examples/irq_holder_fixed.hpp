#pragma once

#include <cstdint>
#include <iostream>

#include "config.hpp"
#include "irq_provider.hpp"

namespace examples {

class IrqHolderFixed
  : IrqProvider::IrqHandlerFixed<IrqHolderFixed, IrqProvider::Irq::USB>
{
    //<! Needed because call_irq_handler is private
    friend IrqProvider::PrivateAccessor;

 public:
    IrqHolderFixed() : IrqHandlerFixed(this) {}

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