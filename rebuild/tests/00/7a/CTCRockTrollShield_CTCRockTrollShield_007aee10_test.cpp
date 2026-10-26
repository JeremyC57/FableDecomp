#include <cstdio>
#include <stddef.h>
class CThing { public: int id; };
static CThing* g_baseThing = 0;
struct CTCBase_Calls {
    virtual void v00() { }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase_Calls(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { g_baseThing = thing; }
};
struct CTCRockTrollShield : CTCBase_Calls {
    long health; long rechargeCounter;
    virtual void v00() { }
    CTCRockTrollShield(CThing* thing) : CTCBase_Calls(thing), health(5), rechargeCounter(0) { }
};
int main()
{
    CThing thing = { 7 };
    CTCRockTrollShield shield(&thing);
    if (g_baseThing != &thing || shield.ParentThing != &thing || shield.health != 5 || shield.rechargeCounter != 0
        || offsetof(CTCRockTrollShield, health) != 0xc || offsetof(CTCRockTrollShield, rechargeCounter) != 0x10 || sizeof(shield) != 0x14) { std::printf("007aee10_TEST FAIL\n"); return 1; }
    std::printf("007aee10_TEST PASS\n"); return 0;
}