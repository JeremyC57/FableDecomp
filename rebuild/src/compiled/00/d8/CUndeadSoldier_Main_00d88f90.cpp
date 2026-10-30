// NScript::CQ_HeroSoulsNostroScript::CUndeadSoldier::Main() -- undead soldier entity script: alive filter on Me, then invulnerable and hostile until the hero comes within 15 units, then damageable for the rest of the script
// Leaf: Main
// Identity (naming wave 11): ?Main@CUndeadSoldier@CQ_HeroSoulsNostroScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012D4F00 slot 1 is stored by the factory 0x00D88EC0 registered as "UndeadSoldier"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D88F90, length 265 bytes (+ int3 padding). Entity-script slot 1 (Main).
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 / ~CScriptThing @ 0x004AA840 (the CIsThingAlive filter, vtable
// 0x012C32F0), the filter registration @ 0x00F35B10, IsActiveThreadTerminating @ 0x00F35B30, CGameScriptInterface slots 7
// NewScriptFrame (+0x1c), 516 EntitySetAsDamageable (+0x810), 601 EntitySetInFaction (+0x964), 70 GetHero (+0x118),
// IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float) @ 0x00CBE2FF (`__fastcall`),
// CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0.
// Strings: 0x012D4F1C "FACTION_GUARDS_ENEMY".
// Layout: the entity-script base {vptr, Game +4, Me +8} (the landed CSummonerToKill::Main 0x00D83DC0 model).
// Shape: the scoped filter object of the landed entity-script Mains; the faction name is an implicit conversion; the
// distance test's float is pushed before GetHero() is evaluated (right-to-left); the final wait is an endless frame loop
// that only leaves when the thread terminates.
namespace NScript { class CPredicateBase; }
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
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
// NScript predicates: the retail models from the shared headers (CPredicateBase: operator() / Clone / ~ slots 0-2; CIsThingAlive: CScriptThing Thing +0x04, 0x10).
#include "engine/CIsThingAlive.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
namespace NScript {
}
namespace NScript {
}
using NScript::CIsThingAlive;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_HeroSoulsNostroScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CUndeadSoldier;                        // the entity class below (naming wave 11)
};
}
using NScript::CQ_HeroSoulsNostroScript;
namespace NScript {
class CQ_HeroSoulsNostroScript::CUndeadSoldier : public CEntityScriptBase {
public:
private:
        virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CQ_HeroSoulsNostroScript::CUndeadSoldier::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetAsDamageable(Me, false);
    Game->EntitySetInFaction(Me, "FACTION_GUARDS_ENEMY");
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetAsDamageable(Me, true);
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}