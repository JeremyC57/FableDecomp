#include <stdio.h>
class CThing { public: int x; };
class CThingCreatureBase;
static int g_calls; static CThingCreatureBase* g_creature; static bool g_hit; static const void* g_this;
class CTCBattleCharge {
public:
    void*   __vftable;
    CThing* Thing;
    void FinishMove(CThingCreatureBase* creature, bool hitSomething) { ++g_calls; g_creature = creature; g_hit = hitSomething; g_this = this; }
    void CancelCharge();
};

void CTCBattleCharge::CancelCharge()
{
    FinishMove((CThingCreatureBase*)Thing, false);
}

int main()
{
    CThing t; CTCBattleCharge c; c.Thing = &t; g_hit = true;
    c.CancelCharge();
    bool ok = g_calls == 1 && g_creature == (CThingCreatureBase*)&t && !g_hit && g_this == &c;
    printf(ok ? "007a7d10_TEST PASS\n" : "007a7d10_TEST FAIL\n");
    return ok ? 0 : 1;
}