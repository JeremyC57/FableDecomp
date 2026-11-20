// CAIStateGroup_FollowPlayer::Update() -- if the followed thing is alive, keeps moving to within FollowDistance of it (idling a tick after each arrival) until a move does not succeed, then terminates
// Leaf: ?Update@CAIStateGroup_FollowPlayer@@UAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094F000, length 125 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem
// @ 0x00A01B90 (the by-value argument copy), CAIStateGroupBase::AIAction_MoveToThingProximity @ 0x0097E380 (ten
// arguments), CAIStateGroupBase::AIAction_DoNothing @ 0x0097E200, CAIStateGroupBase::Terminate @ 0x008FCF40.
// Shape: `if (alive) { while (MoveToThingProximity(..) == 2) DoNothing(); } Terminate();` (the alive test is NOT
// repeated: `jmp` back to the argument pushes; a `while (alive)` loop is rotated by the compiler) -- the followed
// pointer's address formed once into ebx; the move arguments right to left: 0.0f, the four flags, movement type 5,
// the address of an uninitialised `bool` local (high byte of the `push ecx` slot), the follow distance float
// member (+0x1c, loaded as a dword before the constant pushes), the by-value CIntelligentPointer<const CThing>
// converted from the member (inline converting copy constructor, vtable 0x01237F1C), repeat 1.
// Model: local (engine/CAIStateGroup_FollowPlayer.h flattens the base).
#include "engine/CThing.h"

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE
#include "engine/CIntelligentPointer.h"

// CIntelligentPointer<CThing> (member) and the by-value CIntelligentPointer<const CThing> argument (vtable 0x01237F1C, built in its
// slot through the shared converting constructor) are the shared engine/CIntelligentPointer.h templates; the AIAction enums are
// engine/EAIAction.h / EMovementType.h (both PDBs).
#include "engine/EAIAction.h"
#include "engine/EMovementType.h"

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    void* PBrain;                                   // +0x04
    long ScriptAIPriority;                          // +0x08
    void* Name_PStringData;                         // +0x0c
    unsigned char Terminated;                       // +0x10
    unsigned char Priority;                         // +0x11
    unsigned char Enabled;                          // +0x12
    unsigned char _pad_0x13;
    protected: EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat repeat, CIntelligentPointer<const CThing> target, float proximity,
        bool& pArrived, EMovementType movement, bool a, bool b, bool c, bool d, float e);    // 0x0097E380
    EAIActionReturn AIAction_DoNothing(); public:                      // 0x0097E200
    void Terminate();                               // 0x008FCF40
};

struct CAIStateGroup_FollowPlayer : CAIStateGroupBase {
    CIntelligentPointer<CThing> PThingToFollow;        // +0x14
    float FollowDistance;                           // +0x1c
    virtual void Update();
};

void CAIStateGroup_FollowPlayer::Update()
{
    if (((CThing*)PThingToFollow.GetPItem()) != 0 && !((CThing*)PThingToFollow.GetPItem())->IsDead()) {
        bool arrived;
        while (AIAction_MoveToThingProximity(ACTION_REPEAT_UNTIL_FINISHED, PThingToFollow, FollowDistance, arrived,
                AI_MOVEMENT_RUNNING, true, false, true, true, 0.0f) == AI_ACTION_FINISHED)
            AIAction_DoNothing();
    }
    Terminate();
}