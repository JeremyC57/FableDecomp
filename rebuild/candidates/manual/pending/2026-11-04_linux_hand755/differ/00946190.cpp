// CAIStateGroup_PrepareMeal::Update() -- walk to the cooking action point, turn to look along the action
// Grade: DIFFER(259v259) -- register choice only: the LookAtPos C3DVector argument-slot pointer is ecx in ours (mov ecx,esp before the last fadd, this loaded after the stores) and eax in retail (this loaded into ecx before the last fadd, then mov eax,esp)
// Leaf: ?Update@CAIStateGroup_PrepareMeal@@
// Flags: none
// orientation's forward from the creature's physics position, then hold the cooking mode (0x0E) while idling until
// the idle action ends; always terminate afterwards.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00946190, length 259 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::AIAction_MoveToPos @ 0x0097EC00 (C3DVector by value built in its slot through the user
// copy constructor), the creature's physics-position accessor @ 0x006A4D00 (+0x60 sub-object + 0xc),
// AIAction_LookAtPos @ 0x0097D580 (the sum built straight into its argument slot), CVectorMap::LowerBound @ 0x0040F020
// (inlined GetInterface 0x31 = CTCCreatureModeManager, used unconditionally), CTCCreatureModeManager::AddMode
// @ 0x0073AE10 / RemoveMode @ 0x0073ABA0, AIAction_DoNothing @ 0x0097E200, Terminate @ 0x008FCF40.
// Shape: two `do { ... } while (r == 4)` yield loops; the forward vector is a named by-value copy taken before the
// position call. Engine header names (rebuild/include/engine/CAIStateGroup_PrepareMeal.h; flattened base, so the
// class is modelled locally).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCCreatureModeManager.h"
#define CAIStateGroup_PrepareMeal DonorCAIStateGroup_PrepareMeal
#include "engine/CAIStateGroup_PrepareMeal.h"
#undef CAIStateGroup_PrepareMeal
enum EAIActionReturn { AI_ACTION_RETURN_SUCCESS = 2, AI_ACTION_RETURN_IN_PROGRESS = 4 };   // descriptive values
enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 1, AI_ACTION_REPEAT_UNTIL_DONE = 2 };       // descriptive values
enum EMovementType { MOVEMENT_WALK = 3 };                                                 // as the landed 0x00830000
class CAIStateGroup_PrepareMeal : public CAIStateGroupBase {
public:
    CIntelligentPointer<CThing> PCookingPot;    // +0x14
    long ActionPointIndex;                      // +0x1c
    C3DVector ActionPointPos;                   // +0x20
    CRightHandedSet ActionOrientation;          // +0x2c
protected:
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat, C3DVector pos, bool* reached, EMovementType movement, bool a, bool b,
        CThing* avoid, bool c, float speed);                                         // 0x0097EC00
    EAIActionReturn AIAction_LookAtPos(EAIActionRepeat repeat, C3DVector pos, long frames, float speed);   // 0x0097D580
    EAIActionReturn AIAction_DoNothing();       // 0x0097E200
    virtual void Update();
};
void CAIStateGroup_PrepareMeal::Update()
{
    bool reached;
    EAIActionReturn result;
    do {
        result = AIAction_MoveToPos(AI_ACTION_REPEAT_UNTIL_DONE, ActionPointPos, &reached, MOVEMENT_WALK, true, true, 0, true, 0.0f);
    } while (result == AI_ACTION_RETURN_IN_PROGRESS);
    if (result == AI_ACTION_RETURN_SUCCESS) {
        C3DVector forward = ActionOrientation.GetForward();
        const C3DVector& pos = PBrain->Creature->PeekPos();
        if (AIAction_LookAtPos(AI_ACTION_REPEAT_ONCE, forward + pos, 0x11, 0.0027777f) == AI_ACTION_RETURN_SUCCESS) {
            CTCCreatureModeManager* modes;
            PBrain->Creature->GetTC(&modes);
            modes->AddMode(0xe);
            while (AIAction_DoNothing() == AI_ACTION_RETURN_IN_PROGRESS) { }
            modes->RemoveMode(0xe);
        }
    }
    Terminate();
}
