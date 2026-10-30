// CTestPos::Main() -- test entity script: register an alive filter on Me, then loop until done: wait for the hero to come within 1.4 of Me, call interface slot 508 on the hero, wait for the hero to move beyond 3.0, call slot 509 on the hero, yield
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
class CScriptThing { public:                          // 12 bytes
    unsigned char _body[12];
    CScriptThing(const CScriptThing& other);  // 0x004ABE90
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA

namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                         // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                     // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive();                         // 0x00E241B0
};
}
using NScript::CIsThingAlive;

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CTestPos : public CEntityScriptBase {
public:
    void*                 __vftable;          // +0x00
    CGameScriptInterface* Game;               // +0x04
    CScriptThing          Me;                 // +0x08
    void Main();
};

void CTestPos::Main()
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