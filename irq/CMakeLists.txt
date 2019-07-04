add_library(irq INTERFACE)

target_sources(irq INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/vectors.c
)