// CAIStateGroup_WorkSawingWood::Update() -- while it is work time: walk to the sawhorse's action point, face it, play the sawing intro, saw in a loop (while on the spot and before the 15..45 s session end, chosen once), play the outro; repeat until the session ends or the group stops
// Leaf: ?Update@CAIStateGroup_WorkSawingWood@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00944940, 883 bytes (plain `ret`).
// Name: Ego_r.pdb ?Update@CAIStateGroup_WorkSawingWood@@MAEXXZ (donor 0x00968560; C27 LABELS).
//   Model: `this` is a class deriving the shared CAIStateGroup_WorkBase<CAIStateGroupBase> (RTTI .?AV..@@ : CAIStateGroup_WorkBase<CAIStateGroupBase>
//   : CAIStateGroupBase : CBaseClass, rtti_bases.tsv) with the Ego_r members at their Ego_r offsets (struct_layouts_egor.tsv: TookActionPoint +0x14,
//   the CIntelligentPointer<CThingObject> +0x15, ActionPointPos +0x1d, ActionPointRHSet +0x29, ActionPointIndex +0x44; the byte-packed run is
//   what retail reads: [esi+1Dh] / [esi+35h] = RHSet.Forward), written here because the generated engine/CAIStateGroup_WorkSawingWood.h is a flat
//   pack(1) struct that cannot declare the protected virtual Update (the landed siblings' OnActivate use a `_Methods` stand-in with a wrong leaf).
// Callees: AIAction_MoveToPos 0x0097EC00, CTCVillageMember::GetPVillage 0x007EE540, CTCVillage::IsWorkTime 0x00658B20 (the inline
//   CAIStateGroup_WorkBase::IsInWorkHours, Ego_r ?IsInWorkHours@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IBE_NXZ, with the creature's
//   TCI_VILLAGE_MEMBER lookup expanded: FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND), AIAction_LookAtPos 0x0097D580 (Pos + RHSet.Forward, 1/72),
//   CCharString 0x0099EBF0 / 0x0099EAE0, CCreatureAction_PlayAnimation 0x008424F0 / ~CCreatureActionBase 0x00693EF0, AIAction_PerformActionAndWait
//   0x0097DB70, GetPWorldSeed 0x008FCFD0, GFFloatRandom 0x004978A0, CWorld::GetFrame 0x0049D870, __ftol2, Terminate 0x008FCF40;
//   CMainGameComponent::ConstantFPS 0x01375550 (GetConstantFPS), 0.5 0x012316F0, 30.0 0x01292B40, 0.25f 0x0123078C.
// Shape: one EAIActionReturn `result` reused for every AIAction result kept across destructors (retail keeps them in one register;
//   separate locals or direct tests move it to a setcc byte and change the LookAtPos add order); uninitialised `bool arrived`.
//   Also CAIStateGroupBase::PeekPlayerManager 0x008FCF80 + CPlayerManager::GetMainPlayerNumber 0x00449700, CThing::GetPos, CGameDefinitionManager::Get
//   0x0044C6B0 + GetDefGlobalIndexFromName("OBJECT_SAW_01") 0x009AD410 and the TCI_CARRYING lookup (0x00944A53..0x00944A90): results unused
//   in retail (the lookup's end test is gone, the calls stay) -- written as the statements the bytes require; IsStopping 0x008FD010;
//   ST_SAWING_INTO / _LOOP / _OUTOF.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SETPUSHABLEBYHERO
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_WORKBASE_ISINWORKHOURS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_ISWORKTIME
#define FABLE_RETAIL_CTCAISCRATCHPAD_SETISPUSHABLEBYHERO
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEKPLAYERMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingObject.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFFloatRandom.h"
#include "engine/CPlayerManager.h"
#include "engine/CTCCarrying.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_WorkBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"

#pragma pack(push, 1)
class CAIStateGroup_WorkSawingWood : public CAIStateGroup_WorkBase<CAIStateGroupBase> {
protected:
    virtual void Update();
    bool TookActionPoint;                               // +0x14
    CIntelligentPointer<CThingObject> PSawhorse;        // +0x15
    C3DVector ActionPointPos;                           // +0x1d
    CRightHandedSet ActionPointRHSet;                   // +0x29
    unsigned char _pad_0x41[3];
    long ActionPointIndex;                              // +0x44
};
#pragma pack(pop)

void CAIStateGroup_WorkSawingWood::Update()
{
    long end_frame = 0;
    do {
        EAIActionReturn result;
        do {
            bool arrived;
            result = AIAction_MoveToPos(ACTION_REPEAT_CALLBACK, ActionPointPos, arrived, AI_MOVEMENT_WALKING, true, true, 0, true, 0.0f);
            if (!IsInWorkHours()) {
                Terminate();
                return;
            }
        } while (result == AI_ACTION_IN_PROGRESS);
        if (result != AI_ACTION_FINISHED)
            break;
        if (AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED, ActionPointPos + ActionPointRHSet.GetForward(), 1, 1.0f / 72.0f) != AI_ACTION_FINISHED)
            break;
        CTCCarrying* carrying;
        GetCreature().GetTC(&carrying);
        PeekPlayerManager().GetMainPlayerNumber();
        GetCreature().GetPos();
        CGameDefinitionManager::Get().GetDefGlobalIndexFromName("OBJECT_SAW_01");
        result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "ST_SAWING_INTO", CSoundPair(), false, true, false, 0, 0), false);
        if (result != AI_ACTION_FINISHED)
            break;
        if (end_frame == 0)
            end_frame = (long)(CWorld::GetFrame() + CMainGameComponent::GetConstantFPS() * (GFFloatRandom(0.5f, GetPWorldSeed()) + 0.5) * 30.0);
        do {
            result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "ST_SAWING_LOOP", CSoundPair(), false, true, false, 0, 0), false);
            if (result != AI_ACTION_FINISHED) {
                Terminate();
                return;
            }
        } while ((GetCreature().GetPos() - ActionPointPos).GetSquaredMagnitude2D() < 0.25f && end_frame > CWorld::GetFrame());
        result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "ST_SAWING_OUTOF", CSoundPair(), false, true, false, 0, 0), false);
        if (result != AI_ACTION_FINISHED)
            break;
        if (IsStopping())
            break;
    } while (end_frame > CWorld::GetFrame());
    Terminate();
}
