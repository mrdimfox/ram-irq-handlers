#pragma once

#include <cstdint>
#include <type_traits>

namespace ramisr {

using FreeFunc = void (*)(void);

namespace detail {

struct DeafultRamIrqHandlerSetter
{
    static void set(FreeFunc* vector_start, FreeFunc func, uint8_t func_shift)
    {
        *(vector_start + func_shift) = func;
    }
};

}  // namespace detail

/**
 * @brief Aggregator of all classes for a binding interrupts
 *
 * This class provide info like the vector table start address and enum which
 * describes a vector table structure, to inner classes
 *
 * Example of usage:
 *
 * @code{.cpp}
 *
 * enum class Irq : uint8_t {
 *    SYS_TICK = 16,
 *    // .. other interrupts
 * };
 *
 * constexpr const uint32_t VEC_TABLE_ADDR = 0x2001FC00;
 * using ServiceProvider = irq::ServiceProvider<VEC_TABLE_ADDR, Irq>;
 *
 * // Now we can register an interrupt handler
 * // using `register_irq_handler` or any class below.
 * ServiceProvider::register_irq_handler(Irq::SYS_TICK, my_handler);
 *
 * @endcode
 *
 * Before using of ServiceProvider subclasses you should call
 * `move_vector_table_to_ram` from one of the ports files
 * to move your vector table to RAM.
 *
 * @tparam VECTOR_TABLE_ADDRESS - start address of vector table in RAM
 * @tparam VectorTableEnum - enum describing a vector table structure
 * @tparam IrqHandlerSetter - special behaviour of setting Irq handler
 *         (by default it is just an assign, see DeafultRamIrqHandlerSetter)
 */
template<
  uint32_t VECTOR_TABLE_ADDRESS,
  typename VectorTableEnum,
  class IrqHandlerSetter = detail::DeafultRamIrqHandlerSetter>
struct ServiceProvider
{
    ServiceProvider() = delete;
    ServiceProvider(const ServiceProvider&) = delete;
    ServiceProvider& operator=(const ServiceProvider&) = delete;
    ServiceProvider(ServiceProvider&&) = delete;
    ServiceProvider& operator=(ServiceProvider&&) = delete;

    static constexpr const uint32_t VECTOR_TABLE_START_ADDR =
      VECTOR_TABLE_ADDRESS;

    using Irq = VectorTableEnum;

    static inline void register_irq_handler(Irq irq, FreeFunc func)
    {
        auto vectors_table_start =
          reinterpret_cast<FreeFunc*>(VECTOR_TABLE_START_ADDR);

        IrqHandlerSetter::set(vectors_table_start, func, uint8_t(irq));
    }

    struct PrivateAccessor
    {
        template<
          class IrqHandlerHolder,
          Irq IRQ,
          bool CALL_HANDLER_WITH_TEMPLATE_IRQ_TYPE>
        static inline void __attribute__((always_inline))
        call(IrqHandlerHolder* holder)
        {
            if constexpr (CALL_HANDLER_WITH_TEMPLATE_IRQ_TYPE) {
                holder->template call_irq_handler<IRQ>();
            }
            else {
                holder->call_irq_handler();
            }
        }

        PrivateAccessor() = delete;
        PrivateAccessor(const PrivateAccessor&) = delete;
        PrivateAccessor& operator=(const PrivateAccessor&) = delete;
        PrivateAccessor(PrivateAccessor&&) = delete;
        PrivateAccessor& operator=(PrivateAccessor&&) = delete;
    };

    /**
     * @brief Class for registering one IRQ handler
     *
     * Register one class method with fixed name `call_irq_handler`.
     * Required to register this class as friend in class-inheritor
     * (IrqHandlerHolder).
     *
     * @tparam IrqHandlerHolder is a class of IRQ holder (inheritor)
     * @tparam IRQ is a selected interrupt for registering
     * @tparam CALL_HANDLER_WITH_TEMPLATE_IRQ_TYPE if true than
     *         call_irq_handler<Irq>() template method will be
     *         called instead of call_irq_handler() simple method.
     *
     * @note Slower than register_irq_handler but fast enough.
     */
    template<
      class IrqHandlerHolder,
      Irq IRQ,
      bool CALL_HANDLER_WITH_TEMPLATE_IRQ_TYPE = false>
    class IrqHandlerFixed
    {
        static constexpr auto IS_HANDLER_TEMPLATE =
          CALL_HANDLER_WITH_TEMPLATE_IRQ_TYPE;

     public:
        constexpr IrqHandlerFixed(IrqHandlerHolder* holder)
        {
            _holder = holder;

            register_irq_handler(
              IRQ, IrqHandlerFixed<
                     IrqHandlerHolder, IRQ, IS_HANDLER_TEMPLATE>::call_irq);
        }

        IrqHandlerFixed(const IrqHandlerFixed&) = delete;
        IrqHandlerFixed& operator=(const IrqHandlerFixed&) = delete;
        IrqHandlerFixed(IrqHandlerFixed&&) = delete;
        IrqHandlerFixed& operator=(IrqHandlerFixed&&) = delete;

     private:
        static void call_irq()
        {
            PrivateAccessor::template call<
              IrqHandlerHolder, IRQ, IS_HANDLER_TEMPLATE>(_holder);
        }

        static IrqHandlerHolder* _holder;
    };

    /**
     * @brief Register any count of interrupts
     *
     * This is wrapper over IrqHandlerFixed. It requires
     * template method call_irq_handler<Irq> and its
     * specialisations per each interrupt from IRQS
     * template arg.
     *
     * Also it requires make the IrqHandlerFixed tempatate
     * a friend. Put it inside the interrupt holder class.
     *
     * @code{.cpp}
     *
     * template<class, irq::Irq, bool>
     * friend class irq::IrqHandlerFixed;
     *
     * @endcode
     *
     * @tparam IrqHolder is a class of IRQs holder (inheritor)
     * @tparam IRQS is a selected interrupts for registering
     *
     * @note Has same speed as IrqHandlerFixed
     */
    template<class IrqHolder, Irq... IRQS>
    struct MultiIrqHandlerFixed
      : public IrqHandlerFixed<IrqHolder, IRQS, true>...
    {
        MultiIrqHandlerFixed(IrqHolder* holder) :
          IrqHandlerFixed<IrqHolder, IRQS, true>(holder)...
        {
        }

        MultiIrqHandlerFixed(const MultiIrqHandlerFixed&) = delete;
        MultiIrqHandlerFixed& operator=(const MultiIrqHandlerFixed&) = delete;
        MultiIrqHandlerFixed(MultiIrqHandlerFixed&&) = delete;
        MultiIrqHandlerFixed& operator=(MultiIrqHandlerFixed&&) = delete;
    };

    /**
     * @brief Class for registering one IRQ handler with any name
     *
     * Register one class method with any name as an IRQ handler.
     *
     * @tparam IrqHandlerHolder is a class of IRQ holder (inheritor)
     * @tparam IRQ is a selected interrupt for registering
     *
     * @note Slowest method because of the pointer to a class method
     *       but good enough for not very fast tasks.
     */
    template<class IrqHandlerHolder, Irq IRQ>
    class IrqHandler
    {
        using CallableHandler = void (IrqHandlerHolder::*)(void);

     public:
        constexpr IrqHandler(
          IrqHandlerHolder* holder,
          CallableHandler callable = &IrqHandlerHolder::call_irq_handler)
        {
            _callable_handler = callable;
            _holder = holder;

            register_irq_handler(
              IRQ, IrqHandler<IrqHandlerHolder, IRQ>::call_irq);
        }

        IrqHandler(const IrqHandler&) = delete;
        IrqHandler& operator=(const IrqHandler&) = delete;
        IrqHandler(IrqHandler&&) = delete;
        IrqHandler& operator=(IrqHandler&&) = delete;

     private:
        static void call_irq() { (_holder->*_callable_handler)(); }

        static IrqHandlerHolder* _holder;
        static CallableHandler _callable_handler;
    };

    /**
     * @brief Register any count of class methods, one per interrupt
     *
     * Just a wrapper over IrqHandler class for a comfortable
     * work with multiple interrupts connected with multiple
     * methods of IrqHandlerHolder.
     *
     * Example:
     *
     * @code{.cpp}
     *
     * class IrqHolderExample
     *   : public irq::MultiIrqHandler<IrqHolder,
     *                                 irq::Irq::USART1,
     *                                 irq::Irq::USART2>
     * {
     *  public:
     *     IrqHolderExample() :
     *       MultiIrqHandler<IrqHolder, irq::Irq::USART1, irq::Irq::USART2>(
     *         this,
     *         &IrqHolder::_uart1_irq_handler,
     *         &IrqHolder::_uart2_irq_handler)
     *     {
     *     }
     *     void _uart1_irq_handler() { }
     *     void _uart2_irq_handler() { }
     * }
     *
     * @endcode
     *
     * @tparam IrqHandlerHolder is a class of IRQ holder (inheritor)
     * @tparam IRQs is a selected interrupts for registering
     *
     * @note Speed is same to IrqHandler.
     */
    template<class IrqHolder, Irq... IRQS>
    struct MultiIrqHandler : IrqHandler<IrqHolder, IRQS>...
    {
        template<class Holder, class... Callables>
        MultiIrqHandler(Holder* holder, Callables&&... callables) :
          IrqHandler<IrqHolder, IRQS>(holder, callables)...
        {
        }

        MultiIrqHandler(const MultiIrqHandler&) = delete;
        MultiIrqHandler& operator=(const MultiIrqHandler&) = delete;
        MultiIrqHandler(MultiIrqHandler&&) = delete;
        MultiIrqHandler& operator=(MultiIrqHandler&&) = delete;
    };
};

// clang-format off

/// Initialization of static variable _holder of class IrqHandlerFixed
template<uint32_t VT, typename Enum, class S>
template<class IrqHandlerHolder, Enum IRQ, bool TEMPLATE_IRQ_TYPE>
IrqHandlerHolder* 
ServiceProvider<VT, Enum, S>::
  IrqHandlerFixed<IrqHandlerHolder, IRQ, TEMPLATE_IRQ_TYPE>::
    _holder = nullptr;

/// Initialization of static variable _holder of class IrqHandlerHolder
template<uint32_t VT, typename Enum, class S>
template<class IrqHandlerHolder, Enum IRQ>
IrqHandlerHolder*
ServiceProvider<VT, Enum, S>::
  IrqHandler<IrqHandlerHolder, IRQ>::
    _holder = nullptr;

/// Initialization of static variable _callable_handler of class IrqHandlerHolder
template<uint32_t VT, typename Enum, class S>
template<class IrqHandlerHolder, Enum IRQ>
typename ServiceProvider<VT, Enum, S>::template IrqHandler<IrqHandlerHolder, IRQ>::CallableHandler
  ServiceProvider<VT, Enum, S>::IrqHandler<IrqHandlerHolder, IRQ>::
    _callable_handler = nullptr;

// clang-format on

}  // namespace ramisr

#define __COMMA__ ,

#define __MAKE_FRIEND__(class_name, template_line)                             \
    template<uint32_t N_, class T_, class U_>                                  \
    template_line friend class ramisr::ServiceProvider<N_, T_, U_>::class_name

#define FRIEND_IRQ_HANDLER_FIXED                                               \
    __MAKE_FRIEND__(                                                           \
      IrqHandlerFixed, template<class __COMMA__ class __COMMA__ bool>)
