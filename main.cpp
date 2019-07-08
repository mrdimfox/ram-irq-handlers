#include <irq/vectors.h>

#include <examples/irq_holder.hpp>
#include <examples/multi_irq_holder.hpp>

#include <examples/irq_holder_fixed.hpp>
#include <examples/multi_irq_holder_fixed.hpp>

#include <examples/irq_singleton_holder.hpp>

int main()
{
    examples::MultiIrqHolder multi_irq_holder;
    examples::IrqHolderFixed irq_holder_fixed;
    examples::IrqHolderFixedWithMultiIrq irq_holder_fixed_with_multi_irq;
    auto& irq_holder_singleton = examples::IrqHolderSingleton::instance();

    // Simulate interrupt
    global_irq_vectors.usart1_irq();
    global_irq_vectors.dma_irq();
    global_irq_vectors.usart2_irq();
    global_irq_vectors.usb_irq();
    global_irq_vectors.adc1_irq();
    global_irq_vectors.adc2_irq();

    return 0;
}
