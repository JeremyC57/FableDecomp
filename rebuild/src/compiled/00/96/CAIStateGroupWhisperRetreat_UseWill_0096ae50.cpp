// CAIStateGroup_WhisperRetreat::UseWill() -- against a live creature target with a physics component, perform (and wait for) an assassin-rush cast at it whose follow-up action is a new mid-attack-360 on the target
// Leaf: ?UseWill@CAIStateGroup_WhisperRetreat@@IAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096AE50, 258 bytes, plain `ret`. Ego_r.pdb / FableWin.pdb ?UseWill@CAIStateGroup_WhisperRetreat@@IAEXXZ (protected).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PTargetThing +0x14, CIntelligentPointer<CThingCreatureBase>, through the inline
//   Get / operator->), CThing::GetPos const 0x004C73D0 (twice, results unused), operator new 0x00BFEA1A (0x114),
//   CCreatureAction_MidAttack360(CThingCreatureBase&, CThingCreatureBase&) 0x0090A700 (Ego_r / FableWin ctor mangling; the draft's
//   DragonRoar label was the propagated manifest name), CCountedPointer<CCreatureActionBase>(T*) 0x00695880 / ~ 0x0048C5C0 (the retail
//   header's inline bodies; VC7.1 keeps both out of line as retail does), CCreatureAction_CastAssassinRush(CThingCreatureBase&, CThing&,
//   bool, long, CCountedPointer<CCreatureActionBase>) 0x008CE040 / ~ 0x0072D990, CAIStateGroupBase::AIAction_PerformActionAndWait
//   (const CCreatureActionBase&, bool) 0x0097DB70 (header overlay FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION).
// Shape: the retail headers (CAIStateGroupBase::GetCreature() -> *PBrain->Creature, CThing::HasTC / IsDead / GetTypeFlags, the retail
//   CCountedPointer copy into the argument area) replace the draft's local model; that model's out-of-line counted-pointer members and
//   pointer-typed signatures moved the argument pushes and the creature register.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#define CCreatureAction_CastAssassinRush DonorCCreatureAction_CastAssassinRush
#include "engine/CCreatureAction_CastAssassinRush.h"
#undef CCreatureAction_CastAssassinRush
class CCreatureAction_CastAssassinRush : public CCombatActionBase {      // RTTI: CCombatActionBase@0
public:
    CCreatureAction_CastAssassinRush(CThingCreatureBase& creature, CThing& target, bool zTargeting, long level,
                                     CCountedPointer<CCreatureActionBase> finalAction);   // 0x008CE040
    ~CCreatureAction_CastAssassinRush();                                                  // 0x0072D990
    virtual void PerformAction();                                                         // 0x008CEBE0
    virtual CCreatureActionBase* Clone() const;                                           // 0x008CEF20
    unsigned char _pad_0x114[sizeof(DonorCCreatureAction_CastAssassinRush) - 0x114];
};
class CCreatureAction_MidAttack360 : public CCombatActionBase {        // no shared header; RTTI: CCombatActionBase@0, retail sizeof 0x114
public:
    CCreatureAction_MidAttack360(CThingCreatureBase& creature, CThingCreatureBase& target);   // 0x0090A700 (Ego_r / FableWin ??0..@AAVCThingCreatureBase@@0@Z)
};
class CAIStateGroup_EnemyCreatureBase : public CAIStateGroupBase {
protected:
    CIntelligentPointer<CThingCreatureBase> PTargetThing;              // +0x14
};
class CAIStateGroup_WhisperRetreat : public CAIStateGroup_EnemyCreatureBase {
protected:
    void UseWill();
};

void CAIStateGroup_WhisperRetreat::UseWill()
{
    if (PTargetThing.Get() && !PTargetThing->IsDead() && (PTargetThing->GetTypeFlags() & 8)) {
        CThingCreatureBase* target = PTargetThing.Get();
        bool physics = PTargetThing->HasTC(TCI_PHYSICS);
        if (physics) {
            GetCreature().GetPos();
            GetCreature().GetPos();
            CCountedPointer<CCreatureActionBase> finalAction(new CCreatureAction_MidAttack360(GetCreature(), *target));
            AIAction_PerformActionAndWait(CCreatureAction_CastAssassinRush(GetCreature(), *PTargetThing.Get(), false, 0, finalAction), false);
        }
    }
}