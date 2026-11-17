// NF52 (round 7): the guard at 0x013D2838 is the running script process, the established
// ?g_pScriptRunner@@3PAVCSuspendableProcessMicrothread@@A (descriptive; the spelling of the landed 0x00A44660 / 0x00A44880 that store it)
// over engine/CSuspendableProcessMicrothread.h: StopRunning +0x05 and the slot-2 virtual SuspendSuspendableProcess (the frame yield).
// Were the made-up Guard / GuardVT stand-ins and extern g_guard.
#include "engine/CSuspendableProcessMicrothread.h"
extern CSuspendableProcessMicrothread* g_pScriptRunner;   // 0x013D2838

struct Inner {
    char pad0[0x18];
    int f18;     // +0x18
    int f1c;     // +0x1c
};
struct Chain {
    char pad0[0x2c];
    Inner* p2c;  // +0x2c
};
struct Mid2 {
    char pad0[0x8];
    Chain* p8;   // +0x8
};
struct Big {
    char pad0[0x88];
    Mid2* p88;   // +0x88
};

#include "engine/CTBaseSingletonNoDefaultAlloc.h"
class CQuestManager;
// 0x013B89FC = CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (private static; reached through the public inline Get())

class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};   // 0x0049D870

void CGameScriptInterface_NewScriptFrame()
{
    if (g_pScriptRunner->StopRunning) return;
    g_pScriptRunner->SuspendSuspendableProcess();

    if (g_pScriptRunner->StopRunning) return;

    Inner* esi = reinterpret_cast<Big*>(&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get())->p88->p8->p2c;
    int eax = CWorld::GetFrame();
    esi->f18 = esi->f1c;
    esi->f1c = eax;
}