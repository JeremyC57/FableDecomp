#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
namespace std {
template <class Result, class T> class mem_fun_t {
    Result (T::*m_pf)();
public:
    explicit mem_fun_t(Result (T::*pf)()) : m_pf(pf) {}
    Result operator()(T* p) const { return (p->*m_pf)(); }
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
class CTCPerceiveThing {
public:
    int m_calls;
    CTCPerceiveThing() : m_calls(0) {}
    void SetAsIndirectlyAwareOfPerceivedThing() { ++m_calls; }
};
int main()
{
    CTCPerceiveThing a, b, c;
    CTCPerceiveThing* v[3] = { &a, &b, &a };
    std::mem_fun_t<void, CTCPerceiveThing> f = std::for_each(v, v + 3, std::mem_fun_t<void, CTCPerceiveThing>(&CTCPerceiveThing::SetAsIndirectlyAwareOfPerceivedThing));
    if (a.m_calls != 2 || b.m_calls != 1 || c.m_calls != 0) { std::printf("0084ad60_TEST FAIL\n"); return 1; }
    std::mem_fun_t<void, CTCPerceiveThing> g = std::for_each(v, v, f);       // empty range: functor copied back unchanged
    g(&c);
    if (c.m_calls != 1) { std::printf("0084ad60_TEST FAIL\n"); return 1; }
    std::printf("0084ad60_TEST PASS\n");
    return 0;
}