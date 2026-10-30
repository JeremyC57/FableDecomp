// CHS_Screamer::OnPredicateFail() -- Hero Souls screamer died: one screamer fewer for the mother script and its bar grows by the script-def amount; a super screamer also clears the parent's super-screamer flag, restarts the parent's super-screamer timer, removes its effect thing and ends the super attack on itself
// Leaf: ?OnPredicateFail@CHS_Screamer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D83F20, length 114 bytes.
// Callees: CGameScriptInterface (global @ 0x0143E8F8, and the script's own Game at +4) slots 89 SetTimer(long, long),
// 108 RemoveThing(const CScriptThing&, bool, bool) with (false, true), 543 EntityEndScreamerSuperAttackThing.
// Data: the script def @ 0x0143E90C (the bar increment float at +0xc78, the super-screamer timer time at +0xc84).
// Layout = Ego_r.pdb CHS_Screamer (44 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass
// +0x14 (CQ_HeroSoulsMotherScript*: retail ScreamersAlive +0x58, BarToFill +0x5c, GotSuperScreamer +0x65,
// NewSuperScreamerTimer +0x68 -- the PDB offsets sit 0x18 higher), PMasterData +0x18, SuperScreamer +0x1c,
// SuperScreamerEffect +0x20.
// Shape: `BarToFill += def` loads the def first (`fld def ; fadd member`); the parent is re-read for every access.
class CScriptThing { public:                              // 12 bytes (vtable 0x01238C8C, word aligned)
    void* __vftable;
    long  m_data[2];
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptDef { public:
    unsigned char _pad[0xc78];
    float         ScreamerBarIncrease;             // +0xc78
    unsigned char _padc7c[8];
    long          SuperScreamerTimerTime;          // +0xc84
};
class CTimer { public:
    long Handle;                                   // +0x00
};
struct CQ_HeroSoulsMotherScript {
    unsigned char _pad[0x58];
    long          ScreamersAlive;                  // +0x58
    float         BarToFill;                       // +0x5c
    long          BarID;                           // +0x60
    bool          QuestStartScreened;              // +0x64
    bool          GotSuperScreamer;                // +0x65
    unsigned char _pad66[2];
    CTimer        NewSuperScreamerTimer;           // +0x68
};
struct CQ_SunnyvaleMasterData;
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
extern CScriptDef*           pGlobalScriptDef;             // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

class CHS_Screamer {
public:
    void*                     __vftable;           // +0x00
    CGameScriptInterface*     Game;                // +0x04
    CScriptThing              Me;                  // +0x08
    CQ_HeroSoulsMotherScript* ParentClass;         // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;         // +0x18
    bool                      SuperScreamer;       // +0x1c
    CScriptThing              SuperScreamerEffect; // +0x20
    void OnPredicateFail();
};

void CHS_Screamer::OnPredicateFail()
{
    --ParentClass->ScreamersAlive;
    ParentClass->BarToFill += pGlobalScriptDef->ScreamerBarIncrease;
    if (SuperScreamer) {
        ParentClass->GotSuperScreamer = false;
        g_pGameScriptInterface->SetTimer(ParentClass->NewSuperScreamerTimer.Handle, pGlobalScriptDef->SuperScreamerTimerTime);
        Game->RemoveThing(SuperScreamerEffect, false, true);
        Game->EntityEndScreamerSuperAttackThing(Me);
    }
}