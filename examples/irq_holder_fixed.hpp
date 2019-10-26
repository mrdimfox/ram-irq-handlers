#pragma once

#include <cstdint>
#include <iostream>

#include "config.hpp"
#include "isr_provider.hpp"

namespace examples {

class IrqHolderFixed
  : IsrProvider::IrqHandlerFixed<IrqHolderFixed, IsrProvider::Irq::USB>
{
    //<! Needed because call_irq_handler is private
    friend IsrProvider::PrivateAccessor;

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