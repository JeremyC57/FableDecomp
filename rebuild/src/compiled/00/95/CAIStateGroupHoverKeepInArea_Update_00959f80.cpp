// CAIStateGroup_HoverKeepInArea::Update() -- ask the creature's navigation component for a wander position within 20m of the
// area centre StartPos (return at once if there is none); look at a point 3..5m from that position at a random +-0.1..0.4-turn
// angle off the creature->position direction (rotated in the XY plane with CrossWithUp; a failed LookAtPos terminates); then hover
// to the position facing that point with a random movement type 8..10, retrying while the action reports AI_ACTION_IN_PROGRESS and the
// creature is still at least 20m (XY) from StartPos; terminate the state group afterwards.
// Retail: Fable.exe 0x00959F80, 752 bytes (vtable 0x0128C6F4 slot 12).
// Callees: CAIStateGroupBase::GetWorld 0x008FCFC0, CWorld::GetWorldSeed 0x0049D810 (seed pointer fetched once at entry; the
//   movement draw fetches it again), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inline CThing::GetTC(TCI_NAVIGATION),
//   bit 7 of +0x20), CTCCreatureNavigation::GetNearbyWanderPosition 0x006CB6E0, GFROR13 0x00497890 (inline GFRandom x4),
//   CThing::GetPos 0x004C73D0, C3DVector::AccurateNormalise 0x00A14440, CAIStateGroupBase::AIAction_LookAtPos 0x0097D580,
//   virtual CAIStateGroup_HoverMovementBase::AIAction_MoveToPosAndFacePos (slot 22 = 0x0091D7F0), CAIStateGroupBase::Terminate 0x008FCF40.
// Inline in retail: GFRandom, GFSin/GFCos (angle*TWO_PI double, fcos/fsin), C3DVector operators, ?CrossWithUp@C3DVector@@QBE?AV1@XZ,
//   sqrt (fsqrt). Same shape as the sibling HoverWander::Update 0x00959BF0 (dir = target - here, then Z cleared).
// Headers (overlay opt-ins): FABLE_RETAIL_C3DVECTOR_CROSSWITHUP, FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS,
//   FABLE_RETAIL_CAISTATEGROUP_HOVERMOVEMENTBASE_MOVETOPOSANDFACEPOS, FABLE_RETAIL_CAISTATEGROUP_HOVERKEEPINAREA.
// Leaf: ?Update@CAIStateGroup_HoverKeepInArea@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE
#define FABLE_RETAIL_C3DVECTOR_ADD_XY_GROUP
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS
#define FABLE_RETAIL_CAISTATEGROUP_HOVERMOVEMENTBASE_MOVETOPOSANDFACEPOS
#define FABLE_RETAIL_CAISTATEGROUP_HOVERKEEPINAREA
#include <math.h>
#pragma intrinsic(sin, cos, sqrt)
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/C3DVector.h"
#include "engine/CThingAICreature.h"
#include "engine/CVectorMap.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CAIStateGroup_HoverMovementBase.h"
#include "engine/GFRandom.h"
#include "engine/GFSin.h"
#include "engine/GFCos.h"

#include "engine/CAIStateGroup_HoverKeepInArea.h"

void CAIStateGroup_HoverKeepInArea::Update()
{
    unsigned long* seed = GetWorld().GetWorldSeed();
    CTCCreatureNavigation* nav;
    GetCreature().GetTC(TCI_NAVIGATION, &nav);
    C3DVector target;
    if (!nav->GetNearbyWanderPosition(StartPos, 20.0f, target, false))
        return;
    float angle = (float)GFRandom(100, seed) * 0.003f + 0.1f;
    if (GFRandom(2, seed) == 0)
        angle *= -1.0f;
    float dist = (float)GFRandom(100, seed) * 0.02f + 3.0f;
    C3DVector dir = target - GetCreature().GetPos();
    dir.Z = 0.0f;
    dir.AccurateNormalise();
    C3DVector perp = dir.CrossWithUp();
    C3DVector look = target + perp * (float)GFSin(angle) * dist + dir * (float)GFCos(angle) * dist;
    if (AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED, look, 0x11, 0.05f) != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }
    {
        EMovementType movement = (EMovementType)(GFRandom(3, GetWorld().GetWorldSeed()) + 8);
        bool reached;
        EAIActionReturn result;
        float dx, dy;
        do {
            C3DVector diff = GetCreature().GetPos() - StartPos;
            dx = diff.X;
            dy = diff.Y;
            result = AIAction_MoveToPosAndFacePos(ACTION_REPEAT_CALLBACK, target, look, reached, movement);
        } while (result == AI_ACTION_IN_PROGRESS && (float)sqrt(dy * dy + dx * dx) >= 20.0f);
    }
    Terminate();
}