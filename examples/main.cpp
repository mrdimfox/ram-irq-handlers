#include "vectors/vectors.h"

#include "irq_holder.hpp"
#include "multi_irq_holder.hpp"

#include "irq_holder_fixed.hpp"
#include "multi_irq_holder_fixed.hpp"

#include "irq_singleton_holder.hpp"

int main()
{
    // IrqHandler examples
    examples::IrqHolder irq_holder;
    examples::MultiIrqHolder multi_irq_holder;

    // IrqHandlerFixed examples
    examples::IrqHolderFixed irq_holder_fixed;
    examples::IrqHolderFixedWithMultiIrq irq_holder_fixed_with_multi_irq;

    // Free function registration example
    auto& irq_holder_singleton = examples::IrqHolderSingleton::instance();

    // Simulate interrupt
    global_irq_vectors.usart1_irq();
    global_irq_vectors.dma_irq();
    global_irq_vectors.usart2_irq();
    global_irq_vectors.usb_irq();
    global_irq_vectors.adc1_irq();
    global_irq_vectors.adc2_irq();
    global_irq_vectors.spi1_irq();

    return 0;
}
