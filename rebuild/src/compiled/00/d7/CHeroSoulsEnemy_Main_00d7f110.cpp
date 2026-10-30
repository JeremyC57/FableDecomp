// NScript::CQ_HeroSoulsArenaScript::CHeroSoulsEnemy::Main() -- Hero Souls arena enemy script: register an alive-filter over the script's thing (Me), start a script frame, and unless the script is already done pause a second, set the hero as Me's best enemy target and keep running script frames until done
// Leaf: Main
// Identity (naming wave 11): ?Main@CHeroSoulsEnemy@CQ_HeroSoulsArenaScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012D3EE8 slot 1 is stored by the factory 0x00D7F020 registered as "HeroSoulsEnemy"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D7F110, length 142 bytes.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90, the filter registration @ 0x00F35B10,
// CScriptThing::~CScriptThing @ 0x004AA840, IsDone @ 0x00F35B30; CGameScriptInterface (vtable 0x01260F0C) slots
// 7 NewScriptFrame, 376 Pause(1.0f), 70 GetHero, 451 GiveThingBestEnemyTarget (names from rtti-vtables.tsv + manifest).
// Layout = Ego_r.pdb CHeroSoulsEnemy (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8},
// ParentClass +0x14, PMasterData +0x18.
// Shape: as the landed CHTE_Prisoner::Main 0x00D69F50 (scoped filter, `do/while` wait); the target call is the
// nested `Game->GiveThingBestEnemyTarget(Me, Game->GetHero())` (the outer vtable fetched before the inner call).
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
namespace NScript {
class CQ_HeroSoulsArenaScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CHeroSoulsEnemy;                        // the entity class below (naming wave 11)
};
}
using NScript::CQ_HeroSoulsArenaScript;
struct CQ_SunnyvaleMasterData;

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_HeroSoulsArenaScript::CHeroSoulsEnemy : public CEntityScriptBase {
public:
        CQ_HeroSoulsArenaScript* ParentClass;          // +0x14
    CQ_SunnyvaleMasterData*  PMasterData;          // +0x18
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CQ_HeroSoulsArenaScript::CHeroSoulsEnemy::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (!IsActiveThreadTerminating()) {
        Game->Pause(1.0f);
        Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
        do {
            Game->NewScriptFrame();
        } while (!IsActiveThreadTerminating());
    }
}