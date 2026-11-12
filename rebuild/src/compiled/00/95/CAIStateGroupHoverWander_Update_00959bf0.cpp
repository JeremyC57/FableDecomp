// CAIStateGroup_HoverWander::Update() -- three times: ask the creature's navigation component for a wander position within 5m
// of the creature, look at a point 3..5m from that position at a random +-0.1..0.4-turn angle off the creature->position
// direction (rotated in the XY plane with CrossWithUp), then hover to the position facing that point with a random movement type
// AI_MOVEMENT_HOVER_SLOW..AI_MOVEMENT_HOVER_FAST (8..10; retrying while the action reports AI_ACTION_IN_PROGRESS); any LookAtPos /
// move result other than AI_ACTION_FINISHED terminates the state group. The look-at flags 0x11 are an int parameter (no enum).
// Retail: Fable.exe 0x00959BF0, 739 bytes (vtable 0x0128C5D4 slot 12).
// Callees: CAIStateGroupBase::GetWorld 0x008FCFC0, CWorld::GetWorldSeed 0x0049D810 (fetched before the loop for the angle / distance
//   randoms and again each iteration, 0x00959E54 / 0x00959E5B, for the movement random), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inline CThing::GetTC(TCI_NAVIGATION), bit 7 of +0x20),
//   CThing::GetPos 0x004C73D0, CTCCreatureNavigation::GetNearbyWanderPosition 0x006CB6E0, GFROR13 0x00497890 (inline GFRandom x4),
//   C3DVector::AccurateNormalise 0x00A14440, CAIStateGroupBase::AIAction_LookAtPos 0x0097D580 (C3DVector by value, `ret 18h`),
//   virtual CAIStateGroup_HoverMovementBase::AIAction_MoveToPosAndFacePos (slot 22 = 0x0091D7F0), CAIStateGroupBase::Terminate 0x008FCF40.
// Inline in retail: GFRandom, GFSin/GFCos (one angle*TWO_PI shared by fcos/fsin), C3DVector operators and FableWin's
//   ?CrossWithUp@C3DVector@@QBE?AV1@XZ (fchs of dir.X, Z 0.0f). The direction is `target - here` with Z cleared afterwards (the
//   dead Z subtraction is dropped; spelling the XY components into a constructor leaves a different x87 schedule).
// Headers: repo opt-ins FABLE_RETAIL_C3DVECTOR_CROSSWITHUP, FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VSLOTS,
//   FABLE_RETAIL_CAISTATEGROUP_HOVERMOVEMENTBASE_MOVETOPOSANDFACEPOS; overlay opt-in FABLE_RETAIL_CAISTATEGROUP_HOVERWANDER (and the
//   VSLOTS evidence/slot-20 access rewrite in the overlay CAIStateGroup_EnemyCreatureBase.h). Enumerators: EAIAction.h
//   (ACTION_REPEAT_UNTIL_FINISHED=1, ACTION_REPEAT_CALLBACK=2, AI_ACTION_FINISHED=2, AI_ACTION_IN_PROGRESS=4) and EMovementType.h
//   (AI_MOVEMENT_HOVER_SLOW=8), both PDB TPI enums.
// Leaf: ?Update@CAIStateGroup_HoverWander@@UAEXXZ
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
#define FABLE_RETAIL_CAISTATEGROUP_HOVERWANDER
#include <math.h>
#pragma intrinsic(sin, cos)
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
#include "engine/CAIStateGroup_HoverWander.h"
#include "engine/GFRandom.h"
#include "engine/GFSin.h"
#include "engine/GFCos.h"

void CAIStateGroup_HoverWander::Update()
{
    unsigned long* seed = GetWorld().GetWorldSeed();
    CTCCreatureNavigation* nav;
    GetCreature().GetTC(TCI_NAVIGATION, &nav);
    for (long i = 0; i != 3; ++i) {
        C3DVector target;
        if (!nav->GetNearbyWanderPosition(GetCreature().GetPos(), 5.0f, target, true))
            continue;
        float angle = (float)GFRandom(100, seed) * 0.003f + 0.1f;
        if (GFRandom(2, seed) == 0)
            angle *= -1.0f;
        float dist = (float)GFRandom(100, seed) * 0.02f + 3.0f;
        const C3DVector& here = GetCreature().GetPos();
        C3DVector dir = target - here;
        dir.Z = 0.0f;
        dir.AccurateNormalise();
        C3DVector perp = dir.CrossWithUp();
        C3DVector look = target + perp * (float)GFSin(angle) * dist + dir * (float)GFCos(angle) * dist;
        if (AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED, look, 0x11, 0.05f) != AI_ACTION_FINISHED) {
            Terminate();
            return;
        }
        EMovementType movement = (EMovementType)(GFRandom(3, GetWorld().GetWorldSeed()) + AI_MOVEMENT_HOVER_SLOW);
        bool reached;
        EAIActionReturn result;
        do {
            result = AIAction_MoveToPosAndFacePos(ACTION_REPEAT_CALLBACK, target, look, reached, movement);
        } while (result == AI_ACTION_IN_PROGRESS);
        if (result != AI_ACTION_FINISHED) {
            Terminate();
            return;
        }
    }
}