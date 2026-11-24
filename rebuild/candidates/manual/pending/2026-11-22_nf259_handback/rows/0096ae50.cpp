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
// NF259 (2026-11-22, q12731, nf257 port debt): the row-local CAIStateGroup_WhisperRetreat / CAIStateGroup_EnemyCreatureBase models and the
//   renaming-macro CCreatureAction_CastAssassinRush model (`#define ... Donor...` + include + local class) moved onto the shared headers:
//   engine/CAIStateGroup_WhisperRetreat.h (nf257 WHISPERRETREAT_UPDATE: UseWill protected), engine/CAIStateGroup_EnemyCreatureBase.h
//   (PTargetThing +0x14), engine/CCreatureAction_CastAssassinRush.h (retail class, its implicit destructor = 0x0072D990's pick-any COMDAT,
//   and the add-only NF259 opt-in TARGETCTOR for the 0x008CE040 constructor). CCreatureAction_MidAttack360 has no shared header (none under
//   rebuild/include) and keeps the HEAD row's local declaration over the shared CCombatActionBase.
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
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_WHISPERRETREAT
#define FABLE_RETAIL_CAISTATEGROUP_WHISPERRETREAT_UPDATE
#define FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH
#define FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH_TARGETCTOR
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
#include "engine/CCreatureAction_CastAssassinRush.h"
#include "engine/CAIStateGroup_WhisperRetreat.h"
class CCreatureAction_MidAttack360 : public CCombatActionBase {        // no shared header; RTTI: CCombatActionBase@0, retail sizeof 0x114
public:
    CCreatureAction_MidAttack360(CThingCreatureBase& creature, CThingCreatureBase& target);   // 0x0090A700 (Ego_r / FableWin ??0..@AAVCThingCreatureBase@@0@Z)
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