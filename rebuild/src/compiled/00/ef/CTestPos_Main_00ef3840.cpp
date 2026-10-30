// NScript::CPersonalScript_EmptyScript::CTestPos::Main() -- test entity script: register an alive filter on Me, then loop until done: wait for the hero to come within 1.4 of Me, call interface slot 508 on the hero, wait for the hero to move beyond 3.0, call slot 509 on the hero, yield
// Identity (naming wave 11): ?Main@CTestPos@CPersonalScript_EmptyScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012F63D0 slot 1 is stored by the factory 0x00EF3740 registered as "TestPos"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EF3840, length 372 bytes (plain `ret`; entity-script slot 1 = Main).
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90 (the filter's member; filter vtable 0x012C32F0
//   stored first), the filter registration @ 0x00F35B10 (manifest "CWorldMap::AllocateMapThingUID" is propagated),
//   ~CIsThingAlive @ 0x00E241B0 (out of line here), IsDone @ 0x00F35B30 (manifest "CCreatureAction_TrollWhackGroundBase::
//   Initialise" is propagated), IsDistanceBetweenThingsUnder @ 0x00CBE2FF / IsDistanceBetweenThingsOver @ 0x00CBE3EA
//   (__fastcall: both things in ecx/edx, the distance on the stack -- pushed before the GetHero call), and the game
//   interface slots 7 NewScriptFrame, 70 GetHero, 508/509 (names from ghidra_out/cgsi_slots_aligned.tsv:
//   EntityUnsheatheMeleeWeapon / EntityUnsheatheRangedWeapon, both (const CScriptThing&, bool)).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8} as the landed CHTE_Prisoner::Main 0x00D69F50.
// Shape: the filter is a scoped frame object; each wait is `while (!test) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }`
//   followed by `if (IsActiveThreadTerminating()) return;`; the outer loop is a top-tested `while (!IsActiveThreadTerminating())` after the first frame and
//   its own early-out test.
namespace NScript { class CPredicateBase; }
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA

// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
namespace NScript {
}
namespace NScript {
}
using NScript::CIsThingAlive;

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CPersonalScript_EmptyScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CTestPos;                        // the entity class below (naming wave 11)
};
}
using NScript::CPersonalScript_EmptyScript;
namespace NScript {
class CPersonalScript_EmptyScript::CTestPos : public CEntityScriptBase {
public:
private:
        virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CPersonalScript_EmptyScript::CTestPos::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IsActiveThreadTerminating()) {
        while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 1.4f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->EntityUnsheatheMeleeWeapon(Game->GetHero(), false);
        while (!IsDistanceBetweenThingsOver(Game->GetHero(), Me, 3.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->EntityUnsheatheRangedWeapon(Game->GetHero(), false);
        Game->NewScriptFrame();
    }
}