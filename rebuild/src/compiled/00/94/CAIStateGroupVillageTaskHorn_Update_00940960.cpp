// CAIStateGroup_VillageTaskHorn::Update() -- accept the village horn task, walk to the horn's action point and blow the horn three times, then mark the task completed; any failure terminates
// Leaf: ?Update@CAIStateGroup_VillageTaskHorn@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00940960, 431 bytes (+ int3 padding), thiscall, plain `ret`; vtable slot 12.
// Identity: Ego_r.pdb ?Update@CAIStateGroup_VillageTaskHorn@@UAEXXZ; the sibling CAIStateGroup_VillageTaskTorch::Update 0x0093EA90
//   (landed) has the same task / village / use-object machinery.
// Callees: CBaseIntelligentPointer GetPItem 0x00A01B50 / ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10, CTCVillageMember::
//   GetPVillage 0x007EE540, CTCVillage::SetHornTaskAsAcceptedByThing 0x006584F9 / SetHornTaskAsCompleted 0x0065A00F, CThing::GetPos
//   0x004C73D0, CTCGraphicAppearance::GetDummyObject 0x004C08C0 (ACTION_POINT, was g_CCharString_013bac9c),
//   CAIStateGroupBase::AIAction_MoveToPosProximity 0x0097E890 (DELTA_NOTIONAL_ZERO 0x0129BA3C) / AIAction_PerformActionAndWait
//   0x0097DB70 / Terminate 0x008FCF40, CCreatureAction_UseObject ctor 0x0069F230 (~CCreatureActionBase 0x00693EF0), the task's
//   IsCreatureAssignedToTask (slot 7, +0x1c).
// Shape: the landed Torch sibling's spelling; the UseObject temporary's destructor is the compiler-generated one here (new opt-in
//   FABLE_RETAIL_CCREATUREACTION_USEOBJECT_IMPLICIT_DTOR: retail expands it at 0x00940A84, the Torch TU calls the COMDAT).
// Data: PTCVillageMember +0x14, PTask +0x18 (new engine/CAIStateGroup_VillageTaskHorn.h, Ego_r.pdb).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTION_USEOBJECT
#define FABLE_RETAIL_CCREATUREACTION_USEOBJECT_IMPLICIT_DTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_HORNTASK
#define FABLE_RETAIL_CVILLAGETASKHORN
#define FABLE_RETAIL_HORN_TASKS
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingAICreature.h"
#include "engine/CRightHandedSet.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CVillageTaskHorn.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_VillageTaskHorn.h"
#include "engine/CCreatureAction_UseObject.h"
extern const CCharString ACTION_POINT;              // 0x013BAC9C ("ACTION_POINT"; the landed initializer 0x0121E960 spelling)
extern const float DELTA_NOTIONAL_ZERO;                 // 0x0129BA3C ?DELTA_NOTIONAL_ZERO@@3MB (1e-4f)

void CAIStateGroup_VillageTaskHorn::Update()
{
    if (PTask != 0) {
        CTCVillage* pvillage = PTCVillageMember->GetPVillage();
        if (pvillage->SetHornTaskAsAcceptedByThing(*PTask, GetCreature())) {
            CThing* horn = PTask->GetHorn();
            horn->GetPos();                     // retail calls the position getter and drops the result (0x009409BD)
            C3DVector action_pos;
            CRightHandedSet rhset;
            horn->GetTCGraphicAppearance()->GetDummyObject(ACTION_POINT, 0, 0, action_pos, rhset, 1.0f);
            EAIActionReturn ret;
            do {
                bool nav_route_calculated = false;
                ret = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, action_pos, DELTA_NOTIONAL_ZERO, nav_route_calculated,
                    AI_MOVEMENT_WALKING, true, false, true, 0, true, 0.0f);
                if (PTask == 0 || !PTask->IsCreatureAssignedToTask(GetCreature())) {
                    Terminate();
                    return;
                }
            } while (ret == AI_ACTION_IN_PROGRESS);
            if (ret != AI_ACTION_FINISHED) {
                Terminate();
                return;
            }
            for (long i = 0; i < 3; ++i) {
                ret = AIAction_PerformActionAndWait(CCreatureAction_UseObject(GetCreature(), *horn, true), false);
                if (ret != AI_ACTION_FINISHED || PTask == 0 || !PTask->IsCreatureAssignedToTask(GetCreature())) {
                    Terminate();
                    return;
                }
            }
#ifdef _MSC_VER
            // Select the original mutable same-type constructor template on VC71 (as the landed Torch row 0x0093EA90).
            pvillage->SetHornTaskAsCompleted(CIntelligentPointer<CVillageTaskHorn>::CIntelligentPointer<CVillageTaskHorn>(PTask));
#else
            pvillage->SetHornTaskAsCompleted(CIntelligentPointer<CVillageTaskHorn>(PTask));
#endif
        }
    }
    Terminate();
}