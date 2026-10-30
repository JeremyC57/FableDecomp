// NScript::CQ_HeroSoulsNostroScript::CSUMMONED_CREATURE::OnPredicateFail() -- a summoned undead died: one fewer alive for the Nostro script, and if Nostro's vulnerability timer is below the def threshold it gets the def bonus time added
// Leaf: ?OnPredicateFail@CSUMMONED_CREATURE@CQ_HeroSoulsNostroScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D89160, length 107 bytes (+5 int3 padding).
// Which CSUMMONED_CREATURE: the PDB has two (CQ_ArenaScript's and CQ_HeroSoulsNostroScript's); the counter at
// ParentClass+0x50 and the CTimer at +0x54 are CQ_HeroSoulsNostroScript::UndeadAlive / NostroVulnerableTimer
// (PDB 100 / 104, retail 0x14 lower like the other Hero Souls scripts); the arena script has CCharString tags there.
// Callees: the global CGameScriptInterface @ 0x0143E8F8, slots 90 GetTimer(long) (+0x168, twice) and 89
// SetTimer(long, long) (+0x164). Data: the script def @ 0x0143E90C, threshold +0xC88 and bonus +0xC8C.
// Shape: CTimer's inline GetTime/SetTime accessors forward the TimerIndex to the global interface; the def global
// is read through an inline accessor GetScriptDef(), so VC evaluates it (edi) BEFORE each GetTimer call and reads
// the field after it (a plain `pGlobalScriptDef->Field` loads the pointer after the call); `this->ParentClass` is re-read
// for every access.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8

class CScriptDef { public:
    unsigned char _pad[0xC88];
    long          NostroVulnerableTimerThreshold;      // +0xC88
    long          NostroVulnerableTimerBonus;          // +0xC8C
};
extern CScriptDef* pGlobalScriptDef;                            // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
inline CScriptDef* GetScriptDef() { return pGlobalScriptDef; }

// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }

class CScriptThing { public:                                  // 12 bytes (vtable 0x01238C8C)
    void* __vftable;
    long  m_data[2];
};

namespace NScript {

class CQ_SunnyvaleMasterData;

class CQ_HeroSoulsNostroScript { public:
    unsigned char _pad[0x50];
    long          UndeadAlive;                         // +0x50
    CTimer        NostroVulnerableTimer;               // +0x54

    class CSUMMONED_CREATURE {
    public:
        void*                     __vftable;           // +0x00
        CGameScriptInterface*     Game;                // +0x04
        CScriptThing              Me;                  // +0x08
        CQ_HeroSoulsNostroScript* ParentClass;         // +0x14
        CQ_SunnyvaleMasterData*   PMasterData;         // +0x18
        void OnPredicateFail();
    };
};

void CQ_HeroSoulsNostroScript::CSUMMONED_CREATURE::OnPredicateFail()
{
    --ParentClass->UndeadAlive;
    if (ParentClass->NostroVulnerableTimer.GetTime() < GetScriptDef()->NostroVulnerableTimerThreshold)
        ParentClass->NostroVulnerableTimer = ParentClass->NostroVulnerableTimer.GetTime() + GetScriptDef()->NostroVulnerableTimerBonus;
}

} // namespace NScript