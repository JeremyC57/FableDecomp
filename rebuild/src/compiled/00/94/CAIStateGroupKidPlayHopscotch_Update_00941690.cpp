// CAIStateGroup_KidPlayHopscotch::Update() -- while the hopscotch board lives, walk to the start position (looking up the board's
// single-user action point component on each pass), look at the face position, then play the hopscotch action; if the kid then
// faces down the board (normalised offset from the start along the forward vector above 0.9) play it once more and do nothing;
// every failure and the normal end terminate the state group.
// Leaf: ?Update@CAIStateGroup_KidPlayHopscotch@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00941690, 609 bytes, plain `ret` (vtable 0x0128BB90 slot 12; Ego_r.pdb / FableWin.pdb name above).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CAIStateGroupBase::AIAction_MoveToPos 0x0097EC00 /
//   AIAction_LookAtPos 0x0097D580 / AIAction_PerformActionAndWait 0x0097DB70 / AIAction_DoNothing 0x0097E200 / Terminate 0x008FCF40,
//   CCreatureAction_PlayHopscotch ctor 0x00941B20, ~CCreatureActionBase 0x00693EF0, CThing::GetPos 0x004C73D0.
//   Inline: IS_THING_ALIVE, CThing::GetTC (HasTC bit 13 of +0x24 = TCI_SINGLE_USER_ACTION_POINTS, inline CVectorMap::LowerBound),
//   C3DVector operator- / Normalise (GFFastOneOverSqrt, table 0x013CE558) / Dot, CAIStateGroupBase::GetCreature.
// Shape: the board lookup's result is dead -- VC7.1 drops the end-or-greater select and the store but keeps the LowerBound loop.
//   The PerformActionAndWait result goes into a named local before the temporary's dtor (`mov edi,eax`), not into a bool
//   compared inside the full expression (which caches 1 in ebx for the whole function).
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYHOPSCOTCH
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#include "rebuild_abi.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/CThing.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CThingAICreature.h"
#include "engine/CCreatureAction_PlayHopscotch.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
class CTCUserActionPointBase;

#pragma pack(push, 1)
class CAIStateGroup_KidPlayHopscotch : public CAIStateGroupBase {
public:
    bool TookActionPoint;                               // +0x14
    CIntelligentPointer<CThing> PHopscotchBoard;        // +0x15
    unsigned char _pad_0x1d[3];
    long HopscotchStartIndex;                           // +0x20
    C3DVector StartPos;                                 // +0x24
    C3DVector FacePos;                                  // +0x30
    C3DVector HopscotchForwardVec;                      // +0x3c
    C3DVector HopscotchRightVec;                        // +0x48
protected:
    virtual void Update();
};
#pragma pack(pop)

void CAIStateGroup_KidPlayHopscotch::Update()
{
    EAIActionReturn ret;
    do {
        if (!IS_THING_ALIVE(PHopscotchBoard)) {
            Terminate();
            return;
        }
        CTCUserActionPointBase* board;
        PHopscotchBoard->GetTC(TCI_SINGLE_USER_ACTION_POINTS, &board);
        bool moving;
        ret = AIAction_MoveToPos(ACTION_REPEAT_CALLBACK, StartPos, moving, AI_MOVEMENT_RUNNING, true, true, 0, true, 0.0f);
    } while (ret == AI_ACTION_IN_PROGRESS);
    if (ret != AI_ACTION_FINISHED || !IS_THING_ALIVE(PHopscotchBoard)) {
        Terminate();
        return;
    }
    do {
        if (PHopscotchBoard.Get() == 0) {
            Terminate();
            return;
        }
        ret = AIAction_LookAtPos(ACTION_REPEAT_CALLBACK, FacePos, 1, 0.013888889f);
    } while (ret == AI_ACTION_IN_PROGRESS);
    if (ret != AI_ACTION_FINISHED || PHopscotchBoard.Get() == 0) {
        Terminate();
        return;
    }
    ret = AIAction_PerformActionAndWait(CCreatureAction_PlayHopscotch(GetCreature()), false);
    if (ret != AI_ACTION_FINISHED) {
        Terminate();
        return;
    }
    C3DVector offset = GetCreature().GetPos() - StartPos;
    offset.Normalise();
    if (offset.Dot(HopscotchForwardVec) > 0.9f) {
        ret = AIAction_PerformActionAndWait(CCreatureAction_PlayHopscotch(GetCreature()), false);
        if (ret != AI_ACTION_FINISHED) {
            Terminate();
            return;
        }
    }
    AIAction_DoNothing();
    Terminate();
}