// std::for_each(CThing** first, CThing** last, std::binder2nd<std::mem_fun1_t<void, CThing, bool> > f) -- calls CThing::SetInLimbo(flag) on every carried thing of a vector range and returns the binder
// Leaf: for_each
// Retail: Fable.exe (TLC, VC7.1) @ 0x00528720, 61 bytes, `ret 0Ch`. Caller: CTCCarrying::OnSetInLimbo 0x006A23B8 (landed,
// `for_each(v.begin(), v.end(), bind2nd(mem_fun(&CThing::SetInLimbo), inLimbo))`); the bound member pointer is 0x004C8CF0.
// SGI/STLport `for_each` (`for (; first != last; ++first) f(*first); return f;`), compiled /Gr in retail so it is `__fastcall` here: the functor is returned through the hidden pointer (ecx), a pointer `first` rides in edx, the rest on the stack.
// Shape: all four callee-saved registers hold first/last/member pointer/bound flag, so the hidden result pointer is spilled
// to a `push ecx` slot; the empty and non-empty paths share one epilogue.
namespace std {
template <class Arg1, class Arg2, class Result> struct binary_function {
    typedef Arg1 first_argument_type; typedef Arg2 second_argument_type; typedef Result result_type;
};
template <class R, class T, class A> class mem_fun1_t : public binary_function<T*, A, R> {
    R (T::*m_pf)(A);
public:
    explicit mem_fun1_t(R (T::*pf)(A)) : m_pf(pf) {}
    R operator()(T* p, A a) const { return (p->*m_pf)(a); }
};
template <class Op> class binder2nd {
protected:
    Op op;
    typename Op::second_argument_type value;
public:
    binder2nd(const Op& x, const typename Op::second_argument_type& y) : op(x), value(y) {}
    typename Op::result_type operator()(const typename Op::first_argument_type& x) const { return op(x, value); }
};
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
class CThing {
public:
    void SetInLimbo(bool inLimbo);         // 0x004C8CF0 (the bound member pointer)
};
template std::binder2nd<std::mem_fun1_t<void, CThing, bool> > __fastcall std::for_each(CThing**, CThing**, std::binder2nd<std::mem_fun1_t<void, CThing, bool> >);