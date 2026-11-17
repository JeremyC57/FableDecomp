// CAIStateGroup_MinionKidAnnoy::Update() -- while the target lives and stays within MAX_RANGE (9m, 2D) of the kid: run to it (the two physics radii apart) and, on arrival with the target still alive, kick it; terminate when anything fails
// Leaf: ?Update@CAIStateGroup_MinionKidAnnoy@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00933DB0, 347 bytes, `ret`.
// Identity: Ego_r.pdb 0x00957560 ?Update@CAIStateGroup_MinionKidAnnoy@@UAEXXZ; Ego_d ai_state_group_minion_kid_annoy.obj: locals
//   MAX_RANGE (float, function scope), block { ret (EAIActionReturn), nav_route_calculated (bool), { attack_range (float) } }.
// Callees: CBaseIntelligentPointer GetPItem / ctor / SetPItem / dtor 0x00A01B50 / 0x00A01B10 / 0x00A01B90 / 0x00A01C10, CThing::GetPos
//   0x004C73D0, CAIStateGroupBase::AIAction_MoveToThingProximity 0x0097E380 / AIAction_PerformActionAndWait 0x0097DB70 / Terminate
//   0x008FCF40, CCreatureAction_KickCreature ctor 0x00933F10 (same TU, after this row), ~CCreatureActionBase 0x00693EF0.
// Shape: the range test is the inline GFWorld2DDistanceSmallerThan(pos1, pos2, compare, false) (both PDBs; FableWin 0x01F284E0): its
//   GFSqr(pos1.X - pos2.X) + GFSqr(pos1.Y - pos2.Y) gives retail's X-then-Y differences and Y-first squares, and GFSqr(MAX_RANGE) folds
//   to 81.0f; the creature's radius is read as the member (FABLE_RETAIL_CTCPHYSICSBASE_RADIUS_ACCESS: pointer loaded before the
//   GetPItem call, `fadd [esi+34h]` after it, 0x00933E2F / 0x00933E41); the kick result goes back into `ret`, tested after the action
//   temporary's destructors (0x00933EE0 / 0x00933EF0).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_RADIUS_ACCESS
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUP_MINIONKIDANNOY
#include "fable_string.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/GFWorldDistance.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/ThingAlive.h"
#include "engine/CCreatureAction_KickCreature.h"
#include "engine/CAIStateGroup_MinionKidAnnoy.h"

void CAIStateGroup_MinionKidAnnoy::Update()
{
    float MAX_RANGE = 9.0f;
    while (IS_THING_ALIVE(PTarget)) {
        if (!GFWorld2DDistanceSmallerThan(GetCreature().GetPos(), PTarget->GetPos(), MAX_RANGE, false))
            break;
        EAIActionReturn ret;
        bool nav_route_calculated;
        {
            float attack_range = GetCreature().GetPhysicsTC()->Radius + PTarget->GetPhysicsTC()->GetRadius();
            ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, PTarget, attack_range, nav_route_calculated,
                AI_MOVEMENT_RUNNING, true, false, true, true, 0.0f);
        }
        if (ret == AI_ACTION_IN_PROGRESS)
            continue;
        if (ret != AI_ACTION_FINISHED)
            break;
        if (IS_THING_ALIVE(PTarget)) {
            ret = AIAction_PerformActionAndWait(CCreatureAction_KickCreature(GetCreature(), PTarget.Get()), false);
            if (ret != AI_ACTION_FINISHED)
                break;
        }
    }
    Terminate();
}
