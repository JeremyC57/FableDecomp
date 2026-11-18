// CAIStateGroup_HoverHornetQueen::KeepOutOfWay() -- choose a safe circle point near the target, otherwise idle; clear timing-critical mode afterward.
// Leaf: ?KeepOutOfWay@CAIStateGroup_HoverHornetQueen@@IAE?AW4EAIActionReturn@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095B6F0, length 297 bytes.
// Callees: IsStillRunnable 0x0095A420, CAIStateGroupBase::Terminate 0x008FCF40, CBaseIntelligentPointer::GetPItem 0x00A01B50 (PTargetThing),
//   CThing::GetPos 0x004C73D0, CAIStateGroupBase::AIAction_DoNothing 0x0097E200, GotoSafestCirclePoint 0x0095AD20 (EMovementType 9),
//   CCharString(const char*, long) 0x0099EBF0 ("ST_IDLE" 0x0127138C) / ~CCharString 0x0099EAE0, CCreatureAction_PlayAnimation ctor 0x008424F0,
//   AIAction_PerformActionAndWait 0x0097DB70, ~CCreatureActionBase 0x00693EF0. 15.0f @ 0x0124E640.
// Identity (agentNF77, q10884): Ego_r.pdb + FableWin.pdb ?KeepOutOfWay@CAIStateGroup_HoverHornetQueen@@IAE?AW4EAIActionReturn@@XZ (protected,
//   EAIActionReturn); the self leaf was a local CQueenNative::KeepOutOfWay returning long. The action temporary's destruction is
//   ~CCreatureActionBase (??1CCreatureActionBase@@UAE@XZ, both PDBs), formerly bound as a local ??1CActionNative@@UAE@XZ.
// Layout: engine/CAIStateGroup_HoverHornetQueen.h (the ATTACK_PASS23 retail branch: STLport vectors, CircleMarkers +0x50, TimingCritical +0x60;
//   overlay opt-in FABLE_RETAIL_CAISTATEGROUP_HOVERHORNETQUEEN_KEEPOUTOFWAY declares KeepOutOfWay / GotoSafestCirclePoint) on
//   CAIStateGroup_HoverMovementBase / CAIStateGroup_EnemyCreatureBase (PTargetThing +0x14) / CAIStateGroupBase / CAIBrain, and
//   engine/CCreatureAction_PlayAnimation.h with FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR (the derived action destructor is
//   compiler-generated: the temporary's destruction calls ~CCreatureActionBase straight, 0x0095B7EA). Was a local CAIBrain / CThing /
//   CQueenNative / CQueenPointer / CActionNative / CPlayAnimationNative / CAnimationSound model.
// Shape: the 2D distance is a plain sqrt of the XY deltas (the fsqrt with both deltas kept on the x87 stack); the circle-marker test is the
//   vector's size (`test ecx,0FFFFFFF8h` on the byte span).
#include <math.h>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_HOVERHORNETQUEEN_ATTACK_PASS23
#define FABLE_RETAIL_CAISTATEGROUP_HOVERHORNETQUEEN_KEEPOUTOFWAY
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CAIStateGroup_HoverHornetQueen.h"

EAIActionReturn CAIStateGroup_HoverHornetQueen::KeepOutOfWay()
{
    if (!IsStillRunnable()) {
        Terminate();
        return AI_ACTION_TERMINATE_PROCESS;
    }
    TimingCritical = true;
    CThingAICreature& creature = GetCreature();
    const C3DVector& enemy = PTargetThing->GetPos();
    const C3DVector& owner = creature.GetPos();
    float dx = owner.X - enemy.X, dy = owner.Y - enemy.Y;
    EAIActionReturn result;
    if ((float)sqrt(dx * dx + dy * dy) < 15.0f) {
        if (CircleMarkers.size() == 0)
            result = AIAction_DoNothing();
        else
            result = GotoSafestCirclePoint(EMovementType(9));
        if (result == AI_ACTION_FAILED || !IsStillRunnable()) {
            TimingCritical = false;
            return result;
        }
    } else {
        CCharString animation("ST_IDLE");
        result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), animation, CSoundPair(), false, true, false, 0, 0), false);
    }
    if (!IsStillRunnable())
        Terminate();
    TimingCritical = false;
    return result;
}
