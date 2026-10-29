#include "engine/CReactionFrequencyTraits_PeriodicFrequency.h"
#include <cstdio>

class CThingAICreature;



void __fastcall LogReaction(CReactionFrequencyTraits_PeriodicFrequency* traits,
                            int edx_pad,
                            const CThingAICreature& creature, bool flag);

static int g_called = 0;
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
long __fastcall CWorld::GetFrame() {
    g_called++;
    return 0x1234;
}

int main()
{
    CReactionFrequencyTraits_PeriodicFrequency obj;
    obj.LastHappenedFrame = -1;
    const CThingAICreature* cp = 0;

    LogReaction(&obj, 0, *cp, false);
    if (g_called != 0) { printf("FAIL called on false\n"); return 1; }
    if (obj.LastHappenedFrame != -1) { printf("FAIL result changed on false\n"); return 1; }

    LogReaction(&obj, 0, *cp, true);
    if (g_called != 1) { printf("FAIL not called on true\n"); return 1; }
    if (obj.LastHappenedFrame != 0x1234) { printf("FAIL result not stored\n"); return 1; }

    printf("OK_006fedb0\n");
    return 0;
}