// NCreatureMode::CModeDazed::Recover() -- the dazed creature recovers: set it performing the "recover from dazed" combat action
// Leaf: ?Recover@CModeDazed
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DE620, 45 bytes (+ int3 padding), `ret`; vtable 0x01267CC4 slot 15.
// Callees: CCombatAction_RecoverFromDazed::CCombatAction_RecoverFromDazed(CThingCreatureBase*) @ 0x008E43A0 (landed; the
//   manifest label "CCreatureAction_BattleChargeTurnLeft" is propagated: the ctor stores vtable 0x01285C94 =
//   CCombatAction_RecoverFromDazed and the "DAZED_RECOVER" animation), CThingCreatureBase::SetCurrentAction @ 0x006644F0,
//   ~CCombatActionBase @ 0x0062E900 (the action declares no dtor; retail sizeof 0x114 = the frame).
// Name: FableWin.pdb `?Recover@CModeDazed@NCreatureMode@@EAEXXZ` (private virtual, slot 15 of the out-of-action modes).
// Shape: as the landed CModeScorpionKingExhausted::Recover @ 0x008DF640: an explicit action temporary passed by reference
//   (the ctor's returned `this` is pushed); the creature is a named local in esi for the ctor argument and the call.
struct CCreatureActionBase;

struct CThingCreatureBase {
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

struct CCreatureActionBase {               // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa4];
};
struct CCombatActionBase : CCreatureActionBase {   // retail sizeof 0x114
    unsigned char _combat[0x6c];
    ~CCombatActionBase();                  // 0x0062E900
};
struct CCombatAction_RecoverFromDazed : CCombatActionBase {
    CCombatAction_RecoverFromDazed(CThingCreatureBase* owner);   // 0x008E43A0
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThingCreatureBase* ParentThing;       // +0x04
    unsigned char       _body[0x14];
};

namespace NCreatureMode {
struct CModeDazed : CModeBase {            // vtable 0x01267CC4 (CModeOutOfActionBase family)
    void Recover();
};
}

void NCreatureMode::CModeDazed::Recover()
{
    CThingCreatureBase* creature = ParentThing;
    creature->SetCurrentAction(CCombatAction_RecoverFromDazed(creature));
}