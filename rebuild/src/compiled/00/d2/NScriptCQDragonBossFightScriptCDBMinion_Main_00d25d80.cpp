// NScript::CQ_DragonBossFightScript::CDBMinion::Main() -- the summoner entity script: register an alive filter over Me, update once, give Me the hero as best enemy target, wait frame by frame until the parent quest's state word (+0x48) reaches 4, then unless the script is done remove Me (RemoveThing(Me, true, true))
// Leaf: ?Main@CDBMinion@CQ_DragonBossFightScript@NScript@@EAEXXZ
// Plan phase 4 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line, as retail calls them here). Also: engine/CCountedPointer.h included first (the header's PImp is a by-value CCountedPointer<CScriptThing>, Data +0x04 / Info +0x08; the row never expands its members).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D25D80, 176 bytes (entity-script vtable slot 1 = Main).
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 (the filter's member; filter vtable 0x012C32F0), the filter registration
//          @ 0x00F35B10 (on `this`), ~CScriptThing @ 0x004AA840, IsDone @ 0x00F35B30, CGameScriptInterface slots 7 (+0x1c, frame
//          update), 70 (+0x118, GetHero 0x00891CA0), 451 (+0x70c, GiveThingBestEnemyTarget 0x008A0260) and 108 (+0x1b0, RemoveThing).
// Layout: CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14; the parent's state word at +0x48 (descriptive).
// Shape: the landed CSummonerToKill::Main (0x00D83DC0) model: a scoped filter object (its vtable constant kept in ebx for both
//          stores), the nested `Game->GiveThingBestEnemyTarget(Me, Game->GetHero())` (the outer vtable fetched before the inner
//          call), then a frame loop on the parent's state that bails out when the script is done (the constant 4 in ebx).
// Naming wave 35: identity reg 00d25690 "DBMinion" in 00d255e0; Alloc 00d25c60; vtable 012cb78c slot 1; PDB ?Main@CDBMinion@CQ_DragonBossFightScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); replaces CDBSummoner::Main: current name also held by 00d25f50; CDBSummoner::Main is at 00d25f50 (its own vtable slot) (wave3/labels_alloc.tsv); PDB ?Main@CDBMinion@CQ_DragonBossFightScript@NScript@@EAEXXZ in FableWin.pdb.
//   Was `?Main@CDBSummoner@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own, base CEntityScriptBase made polymorphic); class CDBSummoner renamed CDBMinion); same bytes.
namespace NScript { class CPredicateBase; }
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                              // vtable 0x012C32F0: a filter holding a script thing
    CScriptThing m_thing;                          // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
struct CDBSummonerParent {                         // the owning quest script (descriptive)
    unsigned char _body[0x48];
    long          m_state;                         // +0x48
};
class CEntityScriptBase
{
    virtual ~CEntityScriptBase();   // retail slot 0 (the stand-in is polymorphic, as retail: the vptr moves here)

public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

namespace NScript {
class CQ_DragonBossFightScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CDBMinion;
};
}
using NScript::CQ_DragonBossFightScript;
namespace NScript {
class CQ_DragonBossFightScript::CDBMinion : public CEntityScriptBase {
public:
        CGameScriptInterface* Game;                    // +0x04
    CScriptThing          Me;                      // +0x08
    CDBSummonerParent*    ParentClass;             // +0x14
    void*                 PMasterData;             // +0x18
private:
    virtual void Main();                          // PDB ?Main@CDBMinion@CQ_DragonBossFightScript@NScript@@EAEXXZ
};
}

void NScript::CQ_DragonBossFightScript::CDBMinion::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
    while (ParentClass->m_state != 4) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveThing(Me, true, true);
}