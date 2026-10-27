// 005df6b0 behaviour test for CAnimComponentCombatComboChain::CAnimComponentCombatComboChain()
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#ifndef __fastcall
#define __fastcall
#endif
#ifndef __cdecl
#define __cdecl
#endif
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed at line %d\n", __LINE__); g_fail = 1; } } while (0)
class CIVCountedPointeeBase {
public:
    unsigned long IVRefCount;
    CIVCountedPointeeBase() : IVRefCount(1) { }
    virtual ~CIVCountedPointeeBase() { }
};
class CAnimComponentBase : public CIVCountedPointeeBase { public: virtual ~CAnimComponentBase() { } };
class CAnimComponentCombatComboChain : public CAnimComponentBase {
public:
    long ComboStage;
    long ComboID;
    CAnimComponentCombatComboChain();
    virtual ~CAnimComponentCombatComboChain() { }
};
CAnimComponentCombatComboChain::CAnimComponentCombatComboChain()
{
}
static void run()
{
    CAnimComponentCombatComboChain* c = new CAnimComponentCombatComboChain;
    CHECK(c->IVRefCount == 1);
    delete c;
}

int main()
{
    run();
    if (g_fail) {
        std::printf("005df6b0_TEST FAIL\n");
        return 1;
    }
    std::printf("005df6b0_TEST PASS\n");
    return 0;
}