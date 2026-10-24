#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
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
}
namespace std {
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
    bool m_inLimbo;
    void SetInLimbo(bool inLimbo) { m_inLimbo = inLimbo; }
};
int main()
{
    CThing a, b, c; a.m_inLimbo = b.m_inLimbo = c.m_inLimbo = false;
    CThing* v[2] = { &a, &b };
    std::for_each(v, v + 2, std::binder2nd<std::mem_fun1_t<void, CThing, bool> >(std::mem_fun1_t<void, CThing, bool>(&CThing::SetInLimbo), true));
    if (!a.m_inLimbo || !b.m_inLimbo || c.m_inLimbo) { std::printf("00528720_TEST FAIL\n"); return 1; }
    std::printf("00528720_TEST PASS\n");
    return 0;
}