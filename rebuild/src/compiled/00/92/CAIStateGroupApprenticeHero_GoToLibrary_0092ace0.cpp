// CAIStateGroup_ApprenticeHero::GoToLibrary() -- walk to the library marker, face along it and play the peruse-library animation; true when there is no marker or the animation finished
// Lever (chunk Q13): the look turn speed 0x0138CBA8 is a file-static `static float g_LookAtTurnSpeed = 0.01f;` (internal
//   linkage, .data, no PDB public symbol at that address): an extern global schedules its `mov edx,[0138CBA8]` before the first
//   look-target fld, the static after it (0x0092ADD1..0x0092ADDE `fld [esp+1c] ; mov edx,[0138CBA8] ; fadd [eax+8] ; push edx`),
//   as retail; same effect as the landed 0x0092AFE0 function-local-static radius. The look target is a named by-value
//   `C3DVector target = direction + position;` (builds it through eax after `mov ecx,esi`, 0x0092ADEF..0x0092ADF3). Also: the
//   else arm after the MoveToPos test, `return failed ? false : true`, `direction /= direction.GetMagnitude2D()` (header
//   opt-in FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D). Header opt-in (repo headers): CAIStateGroup_ApprenticeHero.h
//   FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO_GOTOLIBRARY. Graded RELOCATION_MATCH with FABLE_HDR_OVERLAY=agentQ13/hdr (no
//   overlay header used). g_LookAtTurnSpeed: descriptive, the landed spelling (0x0092AFE0/0x0092AE70/0x0092A8A0).
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
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ONE_EXPRESSION
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

static float g_LookAtTurnSpeed = 0.01f;                    // 0x0138CBA8 (landed spelling, 0x0092AE70)

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
        C3DVector target = direction + position;
        if (AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED, target, 0xb, g_LookAtTurnSpeed) != AI_ACTION_FINISHED)
            return false;
        bool failed = AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(), "ST_TRAINEE_PERUSE_LIBRARY", true),
            false) != AI_ACTION_FINISHED;
        return failed ? false : true;
    }
}