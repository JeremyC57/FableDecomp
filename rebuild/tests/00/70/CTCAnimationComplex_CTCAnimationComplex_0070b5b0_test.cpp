// Test for 0x0070B5B0 CTCAnimationComplex::~CTCAnimationComplex: deletes the owned animate-thing (if any), then ~CTCBase.
#include <cstdio>
static int g_animDtors = 0, g_baseDtors = 0, g_order = 0, g_animAt = 0, g_baseAt = 0;
class CAnimateThing { public: ~CAnimateThing() { ++g_animDtors; g_animAt = ++g_order; } };
class CTCBase { public:
    virtual ~CTCBase() { ++g_baseDtors; g_baseAt = ++g_order; }
    void* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
};
class CTCAnimationComplex : public CTCBase { public:
    CAnimateThing* PAnimation;
    virtual ~CTCAnimationComplex();
};
CTCAnimationComplex::~CTCAnimationComplex() { if (PAnimation) delete PAnimation; }
int main()
{
    CTCAnimationComplex* a = new CTCAnimationComplex; a->PAnimation = new CAnimateThing;
    CTCBase* b = a; delete b;
    if (g_animDtors != 1 || g_baseDtors != 1 || !(g_animAt < g_baseAt)) { std::printf("0070b5b0_TEST FAIL\n"); return 1; }
    CTCAnimationComplex* c = new CTCAnimationComplex; c->PAnimation = 0; delete c;
    if (g_animDtors != 1 || g_baseDtors != 2) { std::printf("0070b5b0_TEST FAIL\n"); return 1; }
    std::printf("0070b5b0_TEST PASS\n");
    return 0;
}