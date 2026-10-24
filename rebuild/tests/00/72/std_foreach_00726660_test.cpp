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
class CThing { public: int m_rent; };
class CApplyAdditionalImpulseIfInCombatAndTooClose {
public:
    int* m_pTotal;
    CApplyAdditionalImpulseIfInCombatAndTooClose(int* total) : m_pTotal(total) {}
    void operator()(CThing* thing) { *m_pTotal += thing->m_rent; }
};
int main()
{
    int total = 0;
    CThing t[3]; t[0].m_rent = 5; t[1].m_rent = 7; t[2].m_rent = 100;
    CThing* v[3] = { &t[0], &t[1], &t[2] };
    CApplyAdditionalImpulseIfInCombatAndTooClose f = std::for_each(v, v + 2, CApplyAdditionalImpulseIfInCombatAndTooClose(&total));
    if (total != 12 || f.m_pTotal != &total) { std::printf("00726660_TEST FAIL\n"); return 1; }
    std::for_each(v + 2, v + 2, f);
    if (total != 12) { std::printf("00726660_TEST FAIL\n"); return 1; }
    std::printf("00726660_TEST PASS\n");
    return 0;
}