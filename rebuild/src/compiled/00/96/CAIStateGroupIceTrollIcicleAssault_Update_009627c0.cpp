// CAIStateGroup_IceTrollIcicleAssault::Update() -- mark the target in danger, wait for current actions and perform the icicle assault before terminating.
// Leaf: ?Update@CAIStateGroup_IceTrollIcicleAssault@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009627C0, 260 bytes. Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PTarget +0x14),
//   CVectorMap::LowerBound 0x0040F020 (inline GetTC, interface 54 CTCStealth), CTCStealth::SetAsInDanger 0x00747B80,
//   CThingCreatureBase::IsPerformingAnyAction 0x006622A0, CAIStateGroupBase::AIAction_DoNothing 0x0097E200,
//   CCreatureAction_IceTrollIcicleAssaultInto ctor 0x00962990, CAIStateGroupBase::AIAction_CombatPerformActionAndWait 0x0097DA40,
//   ~CCombatActionBase 0x0062E900, CAIStateGroupBase::Terminate 0x008FCF40.
// NF67 (round 7): ported onto the shared headers (the 0x00961640 / 0x0093C030 / 0x0095BCF0 macro blocks). The IceAssaultMethods /
//   CIceThing / CIceMap / CStealth / CIcePointer / CIceCombatAction stand-ins and the local CVectorMap / std::pair / ETCInterfaceType
//   models are gone; the retail classes come from overlay opt-ins FABLE_RETAIL_CAISTATEGROUP_ICETROLLICICLEASSAULT and
//   FABLE_RETAIL_CCREATUREACTION_ICETROLLICICLEASSAULTINTO. 0x006622A0 now binds ?IsPerformingAnyAction@CThingCreatureBase@@QBE_NXZ (nf41:e106).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCSTEALTH_STATTYPE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_COMBATPERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_ICETROLLICICLEASSAULT
#define FABLE_RETAIL_CCREATUREACTION_ICETROLLICICLEASSAULTINTO
#include "rebuild_abi.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCStealth.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureAction_IceTrollIcicleAssaultInto.h"
#include "engine/CAIStateGroup_IceTrollIcicleAssault.h"

void CAIStateGroup_IceTrollIcicleAssault::Update()
{
    if (!IS_THING_ALIVE(PTarget)) {
        Terminate();
        return;
    }
    CTCStealth* stealth;
    if (PTarget->GetTC(&stealth))
        stealth->SetAsInDanger(GetCreature(), (NStealth::EDangerStatus)3);
    while (GetCreature().IsPerformingAnyAction()) {
        if (AIAction_DoNothing() == AI_ACTION_TERMINATE_PROCESS)
            goto finish;
    }
    if (IS_THING_ALIVE(PTarget)) {
        CThingAICreature* creature = &GetCreature();
        EAIActionReturn ret = AIAction_CombatPerformActionAndWait(CCreatureAction_IceTrollIcicleAssaultInto(*creature, *PTarget), false);
        if (ret == AI_ACTION_FINISHED) {
            Terminate();
            return;
        }
    }
finish:
    Terminate();
}
