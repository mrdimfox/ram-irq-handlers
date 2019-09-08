#pragma once

#include <cstdint>
#include <iostream>

#include <examples/config.hpp>
#include <examples/irq_provider.hpp>

namespace examples {

class IrqHolderFixedWithMultiIrq : IrqProvider::MultiIrqHandlerFixed<
                                     IrqHolderFixedWithMultiIrq,
                                     IrqProvider::Irq::ADC1,
                                     IrqProvider::Irq::ADC2>
{
    using MultiIrqHandler = IrqProvider::MultiIrqHandlerFixed<
      IrqHolderFixedWithMultiIrq,
      IrqProvider::Irq::ADC1,
      IrqProvider::Irq::ADC2>;

    //<! Needed because call_irq_handler is private
    FRIEND_IRQ_HANDLER_FIXED;

 public:
    IrqHolderFixedWithMultiIrq() : MultiIrqHandler(this) {}

 private:
    /**
     * @brief Tempalate handler for all IRQs
     *
     * This method requires 2 specializations for
     * ADC1 and ADC2 IRQs.
     *
     * @tparam irq::Irq is a param for specialization
     */
    template<IrqProvider::Irq>
    void call_irq_handler();

    bool _is_adc1 = false;
    bool _is_adc2 = false;
};

/**
 * @brief ADC1 IRQ handler
 */
template<>
void IrqHolderFixedWithMultiIrq::call_irq_handler<IrqProvider::Irq::ADC1>()
{
    if constexpr (config::examples::IS_PRINT_ENABLED) {
        std::cout << "ADC1 interrupt!"
                  << "\n";
    }

    _is_adc1 = true;
}

/**
 * @brief ADC2 IRQ handler
 */
template<>
void IrqHolderFixedWithMultiIrq::call_irq_handler<IrqProvider::Irq::ADC2>()
{
    if constexpr (config::examples::IS_PRINT_ENABLED) {
        std::cout << "ADC2 interrupt!"
                  << "\n";
    }

    _is_adc2 = true;
}

}  // namespace examples