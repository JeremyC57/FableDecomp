// CMinionEscort::Main() -- the clifftop-chase minion escort: register an alive-filter over Me, advance a script frame, and unless already done attach Me to the active quest, turn it to face the hero and make the hero its best enemy target, then keep advancing frames until done
// Leaf: Main
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

class CScriptThing { public:                          // 12 bytes
    unsigned char _body[12 - 4];
    CScriptThing(const CScriptThing& other);  // 0x004ABE90
    virtual ~CScriptThing();                          // 0x004AA840
};

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
class CIsThingAlive : public NScript::CPredicateBase { public:                         // vtable 0x012C32F0: a filter holding a script thing (as landed 0x00D69F50)
    CScriptThing m_thing;                     // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;

struct CQ_MinionClifftopChaseScript;
struct CQ_SunnyvaleMasterData;

class CEntityScriptBase
{
public:
    virtual void v00();
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

class CMinionEscort : public CEntityScriptBase {
public:
    CGameScriptInterface*          Game;          // +0x04
    CScriptThing                   Me;            // +0x08
    CQ_MinionClifftopChaseScript*  ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*        PMasterData;   // +0x18
    bool                           SpecialMinion; // +0x1c
    void Main();
};

void CMinionEscort::Main()
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