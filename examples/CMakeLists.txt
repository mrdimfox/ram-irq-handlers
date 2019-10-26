# Examples of use `ramisr` library
#
# Description:
#  These are examples for each public class from `ramisr` library.
#  Entry point is in "main.cpp" file.
#
#  "vector" folder contains imitator of a real vectors table. It
#  is used by `IrqHandlerSetter` for specialization of `ServiceProvider`
#  (see "irq_provider.hpp" file). See more about `ServiceProvider` in
#  library documentation.
#  
#  In "irq_*.hpp" files contains example of user-classes.
#  All IRQ handlers inside the classes are just a stubs, but if
#  `config::examples::IS_PRINT_ENABLED` is `true` some output will
#  be printed. It is enabled by default.
#

add_executable(examples
    main.cpp
    vectors/vectors.c
)

target_link_libraries(examples
    PRIVATE
        ramisr
)