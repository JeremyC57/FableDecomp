// CAIStateGroup_ApprenticeHero::GoToLibrary() -- walk to the library marker, face along it and play the peruse-library animation; true when there is no marker or the animation finished
// Grade: DIFFER(ours v retail) -- DIFFER(389v389), 2 hunks, on shared headers (w/0092ace0_m3): the LookAtPos argument setup only -- retail
//   loads the turn speed after the first fld (0x0092ADD5 `mov edx,[0138CBA8]`; ours before it) and builds the by-value look target
//   through eax after `mov ecx,esi` (0x0092ADEF..0x0092ADFC `mov ecx,esi ; fadd [eax] ; mov eax,esp`); ours through ecx with
//   `mov ecx,esi` after the stores. Probes: named position reference (fixed the call order), position + direction (no change).
//   Levers that closed the rest (from the 393v389 local model): the else arm after the MoveToPos test keeps the first `return false`
//   copy in place (0x0092AD6A), `return failed ? false : true` drops the `xor eax,eax`, `direction /= direction.GetMagnitude2D()`
//   (header opt-in FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D, one-line X*X+Y*Y) gives the 1/|XY| reciprocal local and the Y-by-memory
//   multiply. Header opt-ins: CAIStateGroup_ApprenticeHero.h FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO_GOTOLIBRARY (declaration).
// Name: FableWin.pdb ?GoToLibrary@CAIStateGroup_ApprenticeHero@@IAE_NXZ (debug 0x02C6B220); the prior draft's FindDummy is the
//   landed CAIStateGroup_ApprenticeHero::GetObjectWithScriptName 0x00929FC0 (Ego_r.pdb, header opt-in _SCRIPTNAME).
// Leaf: ?GoToLibrary@CAIStateGroup_ApprenticeHero@@IAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0092ACE0, 389 bytes, thiscall, plain `ret`.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO
#define FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO_SCRIPTNAME
#define FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO_GOTOLIBRARY
#include <math.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCreatureAction_PlayCombatAnimation.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_ApprenticeHero.h"

extern float g_LookAtTurnSpeed;                    // 0x0138CBA8 (landed spelling, 0x0092AE70)

bool CAIStateGroup_ApprenticeHero::GoToLibrary()
{
    CThingGameObject* library = GetObjectWithScriptName("LibraryMarker");
    if (!library)
        return true;
    C3DVector centre;
    float radius;
    library->GetPhysicsTC()->GetBoundingInfo(centre, radius);
    bool reached;
    if (AIAction_MoveToPos(ACTION_REPEAT_UNTIL_FINISHED, centre, reached, AI_MOVEMENT_WALKING, true, true, 0, true, 0.0f) != AI_ACTION_FINISHED)
        return false;
    else {
        C3DVector direction = library->GetPhysicsTC()->GetRHSet().GetForward();
        direction /= direction.GetMagnitude2D();
        const C3DVector& position = library->GetPos();
        if (AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED, direction + position, 0xb, g_LookAtTurnSpeed) != AI_ACTION_FINISHED)
            return false;
        bool failed = AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(), "ST_TRAINEE_PERUSE_LIBRARY", true),
            false) != AI_ACTION_FINISHED;
        return failed ? false : true;
    }
}
