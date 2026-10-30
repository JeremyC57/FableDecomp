// CFadeinSummoner::GetAlphaChange() -- the per-frame alpha step of the summoner fade: while the camp's chant timer still runs, its remaining time scaled by AlphaGradient plus 0.12, else a flat 0.02; 0.0 once the script is done
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D9FE40, length 112 bytes.
// Callees: the script timer service (global 0x0143E8F8) virtual slot 90 (+0x168, GetTimer, twice), IsDone @ 0x00F35B30 (on `this`;
// manifest label "CCreatureAction_TrollWhackGroundBase::Initialise" is propagated). Constants: 0x0122DEDC = 0.0f, 0x012C26F8 = 0.02f,
// 0x012C26FC = 0.12f.
// Layout: Ego_r.pdb CFadeinSummoner (40 bytes): ParentClass +0x14 (CQ_MinionCampScript*), MyAlpha +0x1c, AlphaGradient +0x20, AlphaMax +0x24
// (as the landed Init 0x00D9F660). The parent's timer at retail +0x60 is ChantTimer (PDB +0x74: the retail quest-script base is 0x14 smaller).
// Shape: every script query is guarded by IsDone (the script macro returns the default 0.0f); written as `if (time <= 0) {...} else
// {...}` with each arm `if (IsActiveThreadTerminating()) return 0.0f; else return value;` -- the `> 0` test then branches forward to the timer arm
// and the shared `return 0.0f` exit stays in the fall-through arm (an if/return without `else` moves it to the later arm);
// `fild t ; fmul AlphaGradient ; fadd 0.12`.
namespace NScript { class CPredicateBase; }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8

class CTimer { public:
    long TimerIndex;                          // +0x00
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
};

struct CQ_MinionCampScript {
    unsigned char _pad[0x60];
    CTimer        ChantTimer;                 // +0x60 (retail)
};

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CFadeinSummoner : public CEntityScriptBase {
public:
    void*                __vftable;           // +0x00
    void*                Game;                // +0x04
    unsigned char        Me[0x0c];            // +0x08
    CQ_MinionCampScript* ParentClass;         // +0x14
    void*                PMasterData;         // +0x18
    float                MyAlpha;             // +0x1c
    float                AlphaGradient;       // +0x20
    bool                 AlphaMax;            // +0x24
    float GetAlphaChange();
};

float CFadeinSummoner::GetAlphaChange()
{
    if (ParentClass->ChantTimer.GetTime() <= 0) {
        if (IsActiveThreadTerminating())
            return 0.0f;
        else
            return 0.02f;
    } else {
        if (IsActiveThreadTerminating())
            return 0.0f;
        else
            return AlphaGradient * ParentClass->ChantTimer.GetTime() + 0.12f;
    }
}