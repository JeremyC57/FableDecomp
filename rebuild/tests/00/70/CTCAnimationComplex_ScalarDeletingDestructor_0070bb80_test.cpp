#include <cstdio>
// Model of CTCAnimationComplex's deleting destructor @ 0x0070BB80: delete the owned animate-thing when set, ~CTCBase.
static int g_thingDtor, g_base;
class CAnimateThing { public: ~CAnimateThing() { ++g_thingDtor; } };
class CTCBase { public: virtual ~CTCBase() { ++g_base; } };
class CTCAnimationComplex : public CTCBase { public: CAnimateThing* m_pAnimateThing; virtual ~CTCAnimationComplex() { if (m_pAnimateThing != 0) delete m_pAnimateThing; } };
int main()
{
    { CTCAnimationComplex c; c.m_pAnimateThing = new CAnimateThing; }
    if (g_thingDtor != 1 || g_base != 1) { std::printf("0070bb80_TEST FAIL\n"); return 1; }
    CTCAnimationComplex* p = new CTCAnimationComplex; p->m_pAnimateThing = 0; CTCBase* b = p; delete b;
    if (g_thingDtor != 1 || g_base != 2) { std::printf("0070bb80_TEST FAIL\n"); return 1; }
    std::printf("0070bb80_TEST PASS\n"); return 0;
}