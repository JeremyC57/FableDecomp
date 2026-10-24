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
struct SCombatantRing { long m_index; float m_radius; };
class CSetupHelperRings {
public:
    long m_a, m_b, m_c;
    CSetupHelperRings() : m_a(0), m_b(0), m_c(0) {}
    void operator()(const SCombatantRing& r) { m_a += r.m_index; ++m_b; m_c = r.m_index; }
};
int main()
{
    SCombatantRing r[3] = { { 1, 1.0f }, { 2, 2.0f }, { 4, 3.0f } };
    CSetupHelperRings f = std::for_each(r, r + 3, CSetupHelperRings());
    if (f.m_a != 7 || f.m_b != 3 || f.m_c != 4) { std::printf("0085b840_TEST FAIL\n"); return 1; }
    std::printf("0085b840_TEST PASS\n");
    return 0;
}