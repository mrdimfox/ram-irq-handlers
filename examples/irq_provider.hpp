#pragma once

#include <irq/irq.hpp>

#include <examples/vectors/vectors.h>

namespace examples {

/**
 * @brief Dummy addr of the vector table in RAM
 */
constexpr const uint32_t ISR_VECTOR_START = 0x0;

/**
 * @brief Dummy irq handler setter
 *
 * Emulate setting interrupts handlers by
 * address provided by IrqProvider.
 */
struct IrqHandlerSetter
{
    static void
    set(irq::VectorFreeFunc*, irq::VectorFreeFunc func, uint8_t func_shift)
    {
        auto* vector_emulation_start =
          reinterpret_cast<irq::VectorFreeFunc*>(&global_irq_vectors);

        *(vector_emulation_start + func_shift) = func;
    }
};

/**
 * @brief IrqProvider specialization for current addr and structure
 */
using IrqProvider = irq::IrqProvider<ISR_VECTOR_START, Irq, IrqHandlerSetter>;

}  // namespace examples