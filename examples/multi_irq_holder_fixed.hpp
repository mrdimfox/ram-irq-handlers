#pragma once

#include <cstdint>
#include <iostream>

#include <irq/irq.hpp>

#include <examples/config.hpp>

namespace examples {

class IrqHolderFixedWithMultiIrq : irq::MultiIrqHandlerFixed<
                                     IrqHolderFixedWithMultiIrq,
                                     irq::Irq::ADC1,
                                     irq::Irq::ADC2>
{
    using MultiIrqHandler = irq::MultiIrqHandlerFixed<
      IrqHolderFixedWithMultiIrq,
      irq::Irq::ADC1,
      irq::Irq::ADC2>;

    //<! Needed because call_irq_handler is private
    template<class, irq::Irq, bool>
    friend class irq::IrqHandlerFixed;

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
    template<irq::Irq>
    void call_irq_handler();

    bool _is_adc1 = false;
    bool _is_adc2 = false;
};

/**
 * @brief ADC1 IRQ handler
 */
template<>
void IrqHolderFixedWithMultiIrq::call_irq_handler<irq::Irq::ADC1>()
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
void IrqHolderFixedWithMultiIrq::call_irq_handler<irq::Irq::ADC2>()
{
    if constexpr (config::examples::IS_PRINT_ENABLED) {
        std::cout << "ADC2 interrupt!"
                  << "\n";
    }

    _is_adc2 = true;
}

}  // namespace examples