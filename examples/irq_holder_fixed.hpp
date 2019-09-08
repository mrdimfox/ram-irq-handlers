#pragma once

#include <cstdint>
#include <iostream>

#include <examples/config.hpp>
#include <examples/irq_provider.hpp>

namespace examples {

class IrqHolderFixed
  : IrqProvider::IrqHandlerFixed<IrqHolderFixed, IrqProvider::Irq::USB>
{
    using IrqHandler =
      IrqProvider::IrqHandlerFixed<IrqHolderFixed, IrqProvider::Irq::USB>;

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