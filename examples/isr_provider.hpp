#pragma once

#include <ramisr/isr.hpp>

#include "vectors/vectors.h"

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
    static void set(
      ramisr::VectorFreeFunc*,
      ramisr::VectorFreeFunc func,
      uint8_t func_shift)
    {
        auto* vector_emulation_start =
          reinterpret_cast<ramisr::VectorFreeFunc*>(&global_irq_vectors);

        *(vector_emulation_start + func_shift) = func;
    }
};

/**
 * @brief ramisr::ServiceProvider specialization for current addr and structure
 */
using IsrProvider =
  ramisr::ServiceProvider<ISR_VECTOR_START, Irq, IrqHandlerSetter>;

}  // namespace examples