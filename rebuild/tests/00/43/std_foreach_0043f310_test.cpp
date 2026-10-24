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
class CGuiControl {
public:
    int m_calls;
    CGuiControl() : m_calls(0) {}
    virtual void Clear() { ++m_calls; }
};
int main()
{
    CGuiControl a, b, c;
    CGuiControl* v[3] = { &a, &b, &a };
    std::mem_fun_t<void, CGuiControl> f = std::for_each(v, v + 3, std::mem_fun_t<void, CGuiControl>(&CGuiControl::Clear));
    if (a.m_calls != 2 || b.m_calls != 1 || c.m_calls != 0) { std::printf("0043f310_TEST FAIL\n"); return 1; }
    std::mem_fun_t<void, CGuiControl> g = std::for_each(v, v, f);       // empty range: functor copied back unchanged
    g(&c);
    if (c.m_calls != 1) { std::printf("0043f310_TEST FAIL\n"); return 1; }
    std::printf("0043f310_TEST PASS\n");
    return 0;
}