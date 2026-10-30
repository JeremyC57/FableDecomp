// NScript::CQ_MinionClifftopChaseScript::CMinionEscort::Main() -- the clifftop-chase minion escort: register an alive-filter over Me, advance a script frame, and unless already done attach Me to the active quest, turn it to face the hero and make the hero its best enemy target, then keep advancing frames until done
// Leaf: Main
// Identity (naming wave 11): ?Main@CMinionEscort@CQ_MinionClifftopChaseScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012D7420 slot 1 is stored by the factory 0x00DA4490 registered as "MinionEscort"
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DA45C0, 194 bytes (`ret`).
// Callees: CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90 (the filter's member at +4; the filter's vtable
// 0x012C32F0 kept in ebx and stored first), the entity-script filter registration 0x00F35B10 (on `this`; manifest label
// "CWorldMap::AllocateMapThingUID" is propagated), CScriptThing::~CScriptThing 0x004AA840 (the filter's inline destructor
// re-stores its vtable first; manifest label "~C3DClothPrimitive" is propagated), IsDone 0x00F35B30 (on `this`, bool in al;
// manifest label "CCreatureAction_TrollWhackGroundBase::Initialise" is propagated), ~CCharString 0x0099EAE0.
// CGameScriptInterface virtuals (slot / address): NewScriptFrame 7 0x006E7410, GetActiveQuestName 655 0x00891880 (CCharString
// by value), EntityAttachToScript 465 0x00890690, GetHero 70 0x00891CA0, EntitySetFacingAngleTowardsThing 475 0x0088E620,
// GiveThingBestEnemyTarget 451 0x008A0260.
// Layout (PDB CMinionEscort 0x20): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18, SpecialMinion +0x1c.
// Shape: as the landed CHTE_Prisoner::Main 0x00D69F50: the filter is a scoped 0x10-byte frame object, the address of Me stays in
// edi; each outer interface call fetches its vtable before the inner GetHero / GetActiveQuestName call (VC evaluates the
// callee first); the frame loop is `do { } while (!IsActiveThreadTerminating());`.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* m_pData;
    ~CCharString();                           // 0x0099EAE0
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
class CQ_MinionClifftopChaseScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CMinionEscort;                        // the entity class below (naming wave 11)
};
}
using NScript::CQ_MinionClifftopChaseScript;
struct CQ_SunnyvaleMasterData;

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_MinionClifftopChaseScript::CMinionEscort : public CEntityScriptBase {
public:
        CQ_MinionClifftopChaseScript*  ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*        PMasterData;   // +0x18
    bool                           SpecialMinion; // +0x1c
private:
    virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CQ_MinionClifftopChaseScript::CMinionEscort::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (!IsActiveThreadTerminating()) {
        Game->EntityAttachToScript(Me, Game->GetActiveQuestName());
        Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
        Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
        do {
            Game->NewScriptFrame();
        } while (!IsActiveThreadTerminating());
    }
}