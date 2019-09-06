#pragma once

#include <cstdint>
#include <type_traits>

#include <irq/vectors.h>

namespace irq {

enum class Irq : uint8_t
{
    DMA,
    USART1,
    USART2,
    USB,
    ADC1,
    ADC2,
    SPI1
    // others
};

using VectorFreeFunc = void (*)(void);

/**
 * @brief IRQ handlers registation
 *
 * Method for registration free free functions
 * and static methods as interrupt.
 *
 * @note Fastest variant of using interrupt handlers
 */
inline void register_irq_handler(Irq irq, VectorFreeFunc func)
{
    auto vectors_table_start =
      reinterpret_cast<VectorFreeFunc*>(&global_irq_vectors);

    *(vectors_table_start + uint8_t(irq)) = func;
}

/**
 * @brief Class for registering one IRQ handler
 *
 * Register one class method with fixed name `call_irq_handler`.
 * Required to register this class as friend in class-inheritor
 * (IrqHandlerHolder).
 *
 * @tparam IrqHandlerHolder is a class of IRQ holder (inheritor)
 * @tparam IRQ is a selected interrupt for registering
 * @tparam TEMPLATE_IRQ_TYPE if true than call_irq_handler<Irq>()
 *         template method will be called instead of
 *         call_irq_handler() simple method.
 *
 * @note Slower than register_irq_handler but fast enough.
 */
template<class IrqHandlerHolder, Irq IRQ, bool TEMPLATE_IRQ_TYPE = false>
class IrqHandlerFixed
{
 public:
    constexpr IrqHandlerFixed(IrqHandlerHolder* holder)
    {
        _holder = holder;

        register_irq_handler(
          IRQ,
          IrqHandlerFixed<IrqHandlerHolder, IRQ, TEMPLATE_IRQ_TYPE>::call_irq);
    }

    IrqHandlerFixed(const IrqHandlerFixed&) = delete;
    IrqHandlerFixed& operator=(const IrqHandlerFixed&) = delete;
    IrqHandlerFixed(IrqHandlerFixed&&) = delete;
    IrqHandlerFixed& operator=(IrqHandlerFixed&&) = delete;

 private:
    static void call_irq()
    {
        if constexpr (TEMPLATE_IRQ_TYPE) {
            _holder->template call_irq_handler<IRQ>();
        }
        else {
            _holder->call_irq_handler();
        }
    }

    static IrqHandlerHolder* _holder;
};

template<class IrqHandlerHolder, Irq IRQ, bool TEMPLATE_IRQ_TYPE>
IrqHandlerHolder*
  IrqHandlerFixed<IrqHandlerHolder, IRQ, TEMPLATE_IRQ_TYPE>::_holder = nullptr;

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
struct MultiIrqHandlerFixed : IrqHandlerFixed<IrqHolder, IRQS, true>...
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

        register_irq_handler(IRQ, IrqHandler<IrqHandlerHolder, IRQ>::call_irq);
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

template<class IrqHandlerHolder, Irq IRQ>
IrqHandlerHolder* IrqHandler<IrqHandlerHolder, IRQ>::_holder = nullptr;

template<class IrqHandlerHolder, Irq IRQ>
typename IrqHandler<IrqHandlerHolder, IRQ>::CallableHandler
  IrqHandler<IrqHandlerHolder, IRQ>::_callable_handler = nullptr;

/**
 * @brief Register any count of class methods, one per interrupt
 *
 * Just a wrapper over IrqHandler class for a comfortable
 * works with multiple interrupts connected with multiple
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

}  // namespace irq