#pragma once

#include <algorithm>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/vector.h>
#include <libopencmsis/core_cm3.h>

namespace irq {

namespace port {

inline static vector_table_t* move_vector_table_to_ram(uint32_t table_rom_addr,
                                                       uint32_t table_ram_addr)
{
    const uint32_t TABLE_ROM_ADDR = table_rom_addr;
    const uint32_t TABLE_RAM_ADDR = table_ram_addr;

    uint8_t* new_vec_table = reinterpret_cast<uint8_t*>(TABLE_RAM_ADDR);
    uint8_t* old_vec_table = reinterpret_cast<uint8_t*>(TABLE_ROM_ADDR);
    std::copy_n(old_vec_table, sizeof(vector_table), new_vec_table);

    SCB->VTOR = TABLE_RAM_ADDR;

    return reinterpret_cast<vector_table_t*>(TABLE_RAM_ADDR);
}

}  // namespace port

}  // namespace irq