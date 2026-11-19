// CAIStateGroup_HoverHornetQueen::GotoCirclePoint(long n, EMovementType movement_type) -- hover to circle marker n, facing the target, until
//   the move ends or the route to it is blocked by the target; a finished move makes n the current circle point.
// Leaf: ?GotoCirclePoint@CAIStateGroup_HoverHornetQueen@@IAE?AW4EAIActionReturn@@JW4EMovementType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095A6C0, length 259 bytes, thiscall `ret 8`.
// Callees: IsStillRunnable 0x0095A420, CAIStateGroupBase::Terminate 0x008FCF40, CBaseIntelligentPointer::GetPItem 0x00A01B50 (the marker and the
//   target), CThing::GetPos 0x004C73D0, the protected virtual CAIStateGroup_HoverMovementBase::AIAction_MoveToPosAndFacePos (slot 22, +0x58),
//   IsRouteToPosBlockedByTarget 0x0095A490.
// Name fix (NF100, round 9, q11464): Ego_r.pdb 0x0097EFF0 + FableWin.pdb 0x02C114E0 ?GotoCirclePoint@CAIStateGroup_HoverHornetQueen@@IAE?AW4EAIActionReturn@@JW4EMovementType@@@Z
//   (protected; engine/CAIStateGroup_HoverHornetQueen.h DIVEBOMB declares it; the landed DiveBomb 0x0095B9E0 binds it so). The old row defined
//   ?GotoCirclePoint@CQueenNative@@QAEJJM@Z on a local CQueenNative view (23 placeholder virtuals, local pointer / marker models); it now sits on
//   the header set of DiveBomb 0x0095B9E0 (overlay opt-in FABLE_RETAIL_CAISTATEGROUP_HOVERHORNETQUEEN_ISROUTETOPOSBLOCKEDBYTARGET). Ego_d.pdb
//   parameter and local names: n, movement_type, ret, new_pos, blocked, nav.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_COMBATPERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS
#define FABLE_RETAIL_CAISTATEGROUP_HOVERHORNETQUEEN_ATTACK_PASS23
#define FABLE_RETAIL_CAISTATEGROUP_HOVERMOVEMENTBASE_ATTACK_PASS23
#define FABLE_RETAIL_CAISTATEGROUP_HOVERHORNETQUEEN_DIVEBOMB
#define FABLE_RETAIL_CAISTATEGROUP_HOVERMOVEMENTBASE_MOVETOPOSANDFACEPOS
#define FABLE_RETAIL_CAISTATEGROUP_HOVERHORNETQUEEN_ISROUTETOPOSBLOCKEDBYTARGET
#define FABLE_RETAIL_CCREATUREACTION_WASPQUEENDIVEATTACK_BASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_ONEPOINTLOS
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ORIGINAL_SUM_PASS23
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_HoverHornetQueen.h"
#include "engine/CCreatureAction_WaspQueenDiveAttack.h"
#include "engine/CTCCreatureNavigation.h"

EAIActionReturn CAIStateGroup_HoverHornetQueen::GotoCirclePoint(long n, EMovementType movement_type)
{
    if (CircleMarkers.size() == 0)
        return AI_ACTION_FAILED;
    if (!IsStillRunnable()) {
        Terminate();
        return AI_ACTION_FINISHED;
    }
    C3DVector new_pos = CircleMarkers[n]->GetPos();
    EAIActionReturn ret;
    bool blocked;
    bool nav;
    do {
        ret = AIAction_MoveToPosAndFacePos(ACTION_REPEAT_CALLBACK, new_pos, PTargetThing->GetPos(), nav, movement_type);
        if (!IsStillRunnable()) {
            Terminate();
            return AI_ACTION_FINISHED;
        }
        blocked = IsRouteToPosBlockedByTarget(n);
    } while (ret == AI_ACTION_IN_PROGRESS && !blocked);
    if (blocked)
        ret = AI_ACTION_FAILED;
    if (ret == AI_ACTION_FINISHED)
        CurrentCirclePoint = n;
    return ret;
}
