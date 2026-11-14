// CAIStateGroup_WorkHammerAnvil::Update() -- walk the blacksmith to the anvil's action point while it is work time, face the anvil, play the hammer intro, hammer in a loop for 15..45 s (while he stays on the spot), play the outro and schedule the next session 10..30 s on
// Leaf: ?Update@CAIStateGroup_WorkHammerAnvil@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00945060, 936 bytes (plain `ret`).
// Name: Ego_r.pdb ?Update@CAIStateGroup_WorkHammerAnvil@@MAEXXZ (donor 0x00968DE0, after OnReplace = retail 0x00944DB0 landed).
//   Model: `this` is a class deriving the shared CAIStateGroup_WorkBase<CAIStateGroupBase> (RTTI .?AV..@@ : CAIStateGroup_WorkBase<CAIStateGroupBase>
//   : CAIStateGroupBase : CBaseClass, rtti_bases.tsv) with the Ego_r members at their Ego_r offsets (struct_layouts_egor.tsv: TookActionPoint +0x14,
//   the CIntelligentPointer<CThingObject> +0x15, ActionPointPos +0x1d, ActionPointRHSet +0x29, ActionPointIndex +0x44, NextFrameHammer +0x48; the byte-packed run is
//   what retail reads: [esi+1Dh] / [esi+35h] = RHSet.Forward), written here because the generated engine/CAIStateGroup_WorkHammerAnvil.h is a flat
//   pack(1) struct that cannot declare the protected virtual Update (the landed siblings' OnActivate use a `_Methods` stand-in with a wrong leaf).
// Callees: AIAction_MoveToPos 0x0097EC00, CTCVillageMember::GetPVillage 0x007EE540, CTCVillage::IsWorkTime 0x00658B20 (the inline
//   CAIStateGroup_WorkBase::IsInWorkHours, Ego_r ?IsInWorkHours@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IBE_NXZ, with the creature's
//   TCI_VILLAGE_MEMBER lookup expanded: FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND), AIAction_LookAtPos 0x0097D580 (Pos + RHSet.Forward, 1/72),
//   CCharString 0x0099EBF0 / 0x0099EAE0, CCreatureAction_PlayAnimation 0x008424F0 / ~CCreatureActionBase 0x00693EF0, AIAction_PerformActionAndWait
//   0x0097DB70, GetPWorldSeed 0x008FCFD0, GFFloatRandom 0x004978A0, CWorld::GetFrame 0x0049D870, __ftol2, Terminate 0x008FCF40;
//   CMainGameComponent::ConstantFPS 0x01375550 (GetConstantFPS), 0.5 0x012316F0, 30.0 0x01292B40, 0.25f 0x0123078C.
// Shape: the move and hammer-loop results share one EAIActionReturn `result` (retail: ebp / edi; a separate loop local changes the
//   allocation and the LookAtPos add order of X); the intro / outro tests are named bools (retail `setne bl` / `sete bl`); uninitialised
//   `bool arrived`; `end_frame > CWorld::GetFrame()` (retail `cmp ebx,eax ; jg`).
//   Also SetPushableByHero 0x0097E330, CVectorMap::LowerBound 0x0040F020 (later TCI_AI_SCRATCHPAD lookups, out of line by budget),
//   CTCAIScratchpad::SetIsPushableByHero (inline, +0x79), CThing::GetPos 0x004C73D0, 20.0 0x01239068; BLACKSMITH_HAMMER_INTO / _LOOP / _OUTOF.
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
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_WorkBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"

#pragma pack(push, 1)
class CAIStateGroup_WorkHammerAnvil : public CAIStateGroup_WorkBase<CAIStateGroupBase> {
protected:
    virtual void Update();
    bool TookActionPoint;                               // +0x14
    CIntelligentPointer<CThingObject> PAnvil;           // +0x15
    C3DVector ActionPointPos;                           // +0x1d
    CRightHandedSet ActionPointRHSet;                   // +0x29
    unsigned char _pad_0x41[3];
    long ActionPointIndex;                              // +0x44
    long NextFrameHammer;                               // +0x48
};
#pragma pack(pop)

void CAIStateGroup_WorkHammerAnvil::Update()
{
    EAIActionReturn result;
    do {
        bool arrived;
        result = AIAction_MoveToPos(ACTION_REPEAT_CALLBACK, ActionPointPos, arrived, AI_MOVEMENT_WALKING, true, true, 0, true, 0.0f);
        if (!IsInWorkHours()) {
            Terminate();
            return;
        }
    } while (result == AI_ACTION_IN_PROGRESS);
    if (result != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }
    if (AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED, ActionPointPos + ActionPointRHSet.GetForward(), 1, 1.0f / 72.0f) != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }
    SetPushableByHero(false);
    bool failed = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "BLACKSMITH_HAMMER_INTO", CSoundPair(), false, true, false, 0, 0), false) != AI_ACTION_FINISHED;
    if (failed) {
        Terminate();
        SetPushableByHero(true);
        return;
    }
    CTCAIScratchpad* scratchpad;
    if (GetCreature().GetTC(&scratchpad))
        scratchpad->SetIsPushableByHero(false);
    long end_frame = (long)(CWorld::GetFrame() + CMainGameComponent::GetConstantFPS() * (GFFloatRandom(0.5f, GetPWorldSeed()) + 0.5) * 30.0);
    do {
        result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "BLACKSMITH_HAMMER_LOOP", CSoundPair(), false, true, false, 0, 0), false);
        if (result != AI_ACTION_FINISHED) {
            CTCAIScratchpad* scratchpad;
            if (GetCreature().GetTC(&scratchpad))
                scratchpad->SetIsPushableByHero(true);
            Terminate();
            break;
        }
    } while ((GetCreature().GetPos() - ActionPointPos).GetSquaredMagnitude2D() < 0.25f && end_frame > CWorld::GetFrame());
    bool finished = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "BLACKSMITH_HAMMER_OUTOF", CSoundPair(), false, true, false, 0, 0), false) == AI_ACTION_FINISHED;
    if (finished)
        NextFrameHammer = (long)(CWorld::GetFrame() + CMainGameComponent::GetConstantFPS() * (GFFloatRandom(0.5f, GetPWorldSeed()) + 0.5) * 20.0);
    SetPushableByHero(true);
    Terminate();
}
