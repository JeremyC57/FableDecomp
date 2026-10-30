// NScript::CV_MurderTwistFinalScript::WaitForEnflameReactionIfNecessary(const CScriptThing& thing) -- if the hero has set the thing alight, disables the hero's will for a three-second script timer (frame by frame) and re-enables it unless the thread is terminating
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB0700, 195 bytes (padded to 208), `ret 4`.
//   PDB name ?WaitForEnflameReactionIfNecessary@CV_MurderTwistFinalScript@NScript@@QAEXABVCScriptThing@@@Z.
// Callees: HasHeroEnflamedEntity(const CScriptThing&) 0x00EB07D0; CScriptBase::IsActiveThreadTerminating 0x00CB7940;
//   through the quest's interface (+0x40): slot 131 SetHeroWillAsUsable(bool) (+0x20c), slot 7 NewScriptFrame (+0x1c);
//   through the global interface 0x0143E8F8 (the CTimer local): slot 87 RegisterTimer (+0x15c), slot 89 SetTimer
//   (+0x164), slot 90 GetTimer (+0x168), slot 88 DeregisterTimer (+0x160).
// Shape: a CTimer local (index in esi) whose inline destructor deregisters it on both exits (the terminating exit
//   inside the loop gets its own copy); the `while (GetTime() > 0)` test is rotated by VC (test before the loop and
//   at the bottom); the post-loop `if (IsActiveThreadTerminating()) return;` before the re-enable (not an `if (!..)` guard) is
//   what keeps the in-loop exit as its own deregister tail.
class CScriptThing;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
class CTimer { public:                                         // a script timer: the interface's timer index
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
    ~CTimer() { g_pGameScriptInterface->DeregisterTimer(Index); }
    void SetTime(long seconds) { g_pGameScriptInterface->SetTimer(Index, seconds); }
    long GetTime() { return g_pGameScriptInterface->GetTimer(Index); }
};
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CV_MurderTwistFinalScript : public CScriptBase { public:
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
    bool HasHeroEnflamedEntity(const CScriptThing& thing);   // 0x00EB07D0
    void WaitForEnflameReactionIfNecessary(const CScriptThing& thing);
};
void CV_MurderTwistFinalScript::WaitForEnflameReactionIfNecessary(const CScriptThing& thing)
{
    if (!HasHeroEnflamedEntity(thing))
        return;
    if (IsActiveThreadTerminating())
        return;
    Game->SetHeroWillAsUsable(false);
    CTimer timer;
    timer.SetTime(3);
    while (timer.GetTime() > 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetHeroWillAsUsable(true);
}
}