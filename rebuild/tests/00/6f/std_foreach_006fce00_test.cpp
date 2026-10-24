#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
namespace std {
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
namespace NAnimationEvents {
class CEvent {
public:
    unsigned long m_idAndFlags;
    float m_time;
};
}
class CCollectEventNames {
public:
    void* m_pNames;
    void* m_pManager;
    CCollectEventNames(void* names, void* manager) : m_pNames(names), m_pManager(manager) {}
    void operator()(const NAnimationEvents::CEvent& ev) { *(unsigned long*)m_pNames += ev.m_idAndFlags & 0x3fffffff; }
};
int main()
{
    unsigned long sum = 0;
    NAnimationEvents::CEvent e[2]; e[0].m_idAndFlags = 0xC0000003; e[0].m_time = 0; e[1].m_idAndFlags = 4; e[1].m_time = 1;
    CCollectEventNames f = std::for_each(e, e + 2, CCollectEventNames(&sum, 0));
    if (sum != 7 || f.m_pNames != &sum) { std::printf("006fce00_TEST FAIL\n"); return 1; }
    std::printf("006fce00_TEST PASS\n");
    return 0;
}