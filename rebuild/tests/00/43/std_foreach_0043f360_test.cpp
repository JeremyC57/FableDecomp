#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
namespace std {
template <class Result, class T> class mem_fun_ref_t {
    Result (T::*m_pf)();
public:
    explicit mem_fun_ref_t(Result (T::*pf)()) : m_pf(pf) {}
    Result operator()(T& r) const { return (r.*m_pf)(); }
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
class CEnginePrimitiveHandle {
public:
    void* m_pVTableOrOwner;
    void* m_pData;
    void Clear() { m_pData = 0; }
};
int main()
{
    int x = 1;
    CEnginePrimitiveHandle h[3];
    for (int i = 0; i < 3; ++i) { h[i].m_pVTableOrOwner = 0; h[i].m_pData = &x; }
    std::for_each(h, h + 2, std::mem_fun_ref_t<void, CEnginePrimitiveHandle>(&CEnginePrimitiveHandle::Clear));
    if (h[0].m_pData != 0 || h[1].m_pData != 0 || h[2].m_pData != &x) { std::printf("0043f360_TEST FAIL\n"); return 1; }
    std::printf("0043f360_TEST PASS\n");
    return 0;
}