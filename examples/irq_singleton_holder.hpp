#pragma once

#include <cstdint>
#include <iostream>

#include <examples/config.hpp>
#include <examples/irq_provider.hpp>
#include <singleton/singleton.hpp>

namespace examples {

class IrqHolderSingleton : public patterns::Singleton<IrqHolderSingleton>
{
    friend patterns::Singleton<IrqHolderSingleton>;

 private:
    IrqHolderSingleton()
    {
        IrqProvider::register_irq_handler(
          IrqProvider::Irq::USART2, &IrqHolderSingleton::irq_uart2_handler);
    };

    static void irq_uart2_handler()
    {
        if constexpr (config::examples::IS_PRINT_ENABLED) {
            std::cout << "UART2 interrupt!"
                      << "\n";
        }

        instance_ptr->_uart2 = true;
    }

    bool _uart2 = false;
};

}  // namespace examples