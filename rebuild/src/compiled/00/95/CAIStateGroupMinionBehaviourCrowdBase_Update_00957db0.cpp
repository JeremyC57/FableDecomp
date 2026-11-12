// CAIStateGroup_MinionBehaviourCrowdBase::Update() -- walk to the designated crowd position while the creature stays directly aware of its perceived thing and in line of sight of it; once there, look at the crowd thing until done and hand over to the class's UpdateLeaf; any other outcome terminates the group
// Leaf: ?Update@CAIStateGroup_MinionBehaviourCrowdBase@@UAEXXZ
// Retail: Fable.exe 0x00957DB0, 266 bytes, thiscall (vtable 0x0128C494 slot 12; Ego_r.pdb / FableWin.pdb name above).
// Callees: CThing::GetTC<CTCPerceiveThing> (expanded: UsingComponents bit 52 = TCI_PERCEIVE_THING, CVectorMap::LowerBound 0x0040F020),
//   CAIStateGroupBase::AIAction_MoveToPos 0x0097EC00, CTCPerceiveThing::IsAwarenessOfThingDirect 0x00744430 (IsAwareOfThing and
//   IsThingLineOfSight expanded), CAIStateGroupBase::Terminate 0x008FCF40, the CIntelligentPointer<const CThing> copy (0x00A01B10 /
//   0x00A01B50 / 0x00A01B90) for the by-value argument of CAIStateGroupBase::AIAction_LookAtThing 0x0097D6F0, UpdateLeaf (slot 15).
// Shape: the route flag is declared without a value; VC7.1 gives it the byte the expanded GetTC stores its presence bit to
//   (0x00957DC7, passed by address at 0x00957E0F). A lost perception ends the move loop with AI_ACTION_FAILED, which the compiler
//   threads straight to the shared Terminate (0x00957E60); `if (ret != AI_ACTION_FINISHED) { Terminate(); return; }` keeps that
//   Terminate inline after the loop with the look-at loop after it (an `if (ret == AI_ACTION_FINISHED) {...}` arm moves it to the
//   end). The copy constructor's original GetPointer/SetPointer chain (FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN) gives
//   retail's this-in-edi / argument-in-esi colouring; the plain SetPItem(GetPItem()) body swaps them.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCPERCEIVETHING_SMELL
#define FABLE_RETAIL_CTCPERCEIVETHING_AWAREOFTHING
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"
#include "engine/C3DVector.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_MinionBehaviourCrowdBase.h"

void CAIStateGroup_MinionBehaviourCrowdBase::Update()
{
    CTCPerceiveThing* perceive;
    GetCreature().GetTC(&perceive);
    bool nav_route_calculated;
    EAIActionReturn ret;
    do {
        ret = AIAction_MoveToPos(ACTION_REPEAT_CALLBACK, DesignatedCrowdPosition, nav_route_calculated, AI_MOVEMENT_WALKING, true, true, 0, true, 0.0f);
        if (!perceive->IsAwareOfThing() || !perceive->IsAwarenessOfThingDirect() || !perceive->IsThingLineOfSight()) {
            ret = AI_ACTION_FAILED;
            break;
        }
    } while (ret == AI_ACTION_IN_PROGRESS);
    if (ret != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }
    do {
        ret = AIAction_LookAtThing(ACTION_REPEAT_CALLBACK, PCrowdThing, 0x11, 0.01f);
    } while (ret == AI_ACTION_IN_PROGRESS);
    if (ret == AI_ACTION_FINISHED)
        UpdateLeaf();
}