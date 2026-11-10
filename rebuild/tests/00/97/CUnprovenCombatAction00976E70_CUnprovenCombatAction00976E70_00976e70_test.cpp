// Behaviour test for the jump destructor 0x00976E70: destroying the action by name runs only the CCombatActionBase destructor
// (the derived class has no members and no destructor work of its own).
#include <cstdio>
static int g_baseDtors = 0;
class CCombatActionBase { public: virtual ~CCombatActionBase() { ++g_baseDtors; } };
struct CUnprovenCombatAction : CCombatActionBase { };
int main()
{
    {
        CUnprovenCombatAction a;
    }
    bool ok = g_baseDtors == 1;
    CCombatActionBase* p = new CUnprovenCombatAction;
    delete p;
    ok = ok && g_baseDtors == 2;
    if (!ok) { std::printf("00976e70_TEST FAIL\n"); return 1; }
    std::printf("00976e70_TEST PASS\n");
    return 0;
}