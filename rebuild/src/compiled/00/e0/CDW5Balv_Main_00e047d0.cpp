// NScript::CQ_TraderEscortScript::CDW5_Bandit::Main() -- the balverine entity script: register an alive filter over Me, update once, wait frame by frame until Me is aware of the hero, give Me the hero as best enemy target, then idle frame by frame until the script is done
// Leaf: Main
// Identity (naming wave 11): ?Main@CDW5_Bandit@CQ_TraderEscortScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012DF710 slot 1 is stored by the factory 0x00E046F0 registered as "DW5_Bandit"
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E047D0, 178 bytes (+ int3 padding; entity-script vtable slot 1 = Main).
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 (the filter's member; filter vtable 0x012C32F0), the filter registration
//          @ 0x00F35B10 (on `this`), ~CScriptThing @ 0x004AA840, IsDone @ 0x00F35B30, CScriptThing::IsAwareOfHero (virtual slot 16,
//          +0x40; 0x004AAAC0), CGameScriptInterface slots 7 (+0x1c, frame update), 70 (+0x118, GetHero 0x00891CA0) and 451 (+0x70c,
//          GiveThingBestEnemyTarget 0x008A0260).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8 (polymorphic, 12 bytes)}, ParentClass +0x14.
// Shape: the landed CSummonerToKill::Main (0x00D83DC0) model: a scoped filter object (its vtable constant kept in ebx for both
//          stores), a frame loop on Me's virtual awareness test that bails out when the script is done, the nested
//          `Game->GiveThingBestEnemyTarget(Me, Game->GetHero())` (the outer vtable fetched before the inner call) and a final
//          update-until-done loop.
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
class CQ_TraderEscortScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CDW5_Bandit;                        // the entity class below (naming wave 11)
};
}
using NScript::CQ_TraderEscortScript;
namespace NScript {
class CQ_TraderEscortScript::CDW5_Bandit : public CEntityScriptBase {
public:
        void*                 ParentClass;             // +0x14
    void*                 PMasterData;             // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CQ_TraderEscortScript::CDW5_Bandit::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!Me.IsAwareOfHero()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}