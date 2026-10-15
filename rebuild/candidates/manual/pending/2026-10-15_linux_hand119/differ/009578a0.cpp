// Grade: DIFFER(148v148) -- only the tail block order: retail `cmp eax,2 ; je turn ; xor al,al ; ret ; turn: call TurnToFace`, the lane emits `jne false ; call TurnToFace ; ret ; false: xor al,al`
// CAIStateGroupBehaviourBase::UpdateFocusAttentionOnThingActive() -- while the perception component still has an attention level and is focused, walk to within 2 units of the target thing; on arrival turn to face it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009578A0, length 148 bytes (+ int3 padding).
// Callees: the perception component's predicate @ 0x00744430 (thiscall, no arguments; the manifest's
// "CTCInventoryBase::GetCanItemBeQuickAssigned" label is propagated), CAIStateGroupBase::AIAction_MoveToThingProximity
// @ 0x0097E380 (ten arguments), CAIStateGroupBehaviourBase::TurnToFace @ 0x00957750 (manifest label says
// CAIStateGroup_OpinionReactionRoot: propagated), CBaseIntelligentPointer ctor/GetPItem/SetPItem @ 0x00A01B10/50/90.
// Shape: a `for (;;)` whose head is 16-aligned (`jmp` over `lea esp,[esp] ; mov edi,edi` padding); the
// attention float compared `!= 0.0f` against the 0x0122DEDC zero (`fld zero ; fld member ; fucompp ; test ah,44h ;
// jnp`); the action status 4 (still running) loops, 2 (arrived) returns TurnToFace(), anything else false. The
// action call pushes its arguments right to left: 0.0f, the four flags, movement type 3, the address of an
// uninitialised `bool` local (the high byte of the `push ecx` frame slot), 2.0f, the by-value
// CIntelligentPointer<const CThing> copy of PTargetThing (inline copy constructor, vtable 0x01237F1C), repeat 1.
// Model: engine/CAIStateGroupBehaviourBase.h (PTargetThing +0x14, PTCPerception +0x1c).
#include "engine/CAIStateGroupBehaviourBase.h"

struct CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                      // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {      // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CTCPerceiveThing {
    unsigned char _pad0[0x18];
    float         AttentionLevel;                       // +0x18
    unsigned char _pad1[0x11];
    unsigned char Focused;                              // +0x2d
    bool IsPerceivingTarget();                          // 0x00744430
};

enum EAIActionRepeat { AI_ACTION_REPEAT_NONE, AI_ACTION_REPEAT_ONCE };
enum EAIActionReturn { AI_ACTION_RETURN_NONE, AI_ACTION_RETURN_1, AI_ACTION_RETURN_SUCCEEDED, AI_ACTION_RETURN_3, AI_ACTION_RETURN_RUNNING };
enum EMovementType { MOVEMENT_TYPE_0, MOVEMENT_TYPE_1, MOVEMENT_TYPE_2, MOVEMENT_TYPE_WALK };

struct CAIStateGroupBehaviourBase_Methods : CAIStateGroupBehaviourBase {
    EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat repeat, CIntelligentPointerThing target, float proximity,
        bool* pArrived, EMovementType movement, bool a, bool b, bool c, bool d, float e);   // 0x0097E380
    bool TurnToFace();                                                                       // 0x00957750
    bool UpdateFocusAttentionOnThingActive();
};

bool CAIStateGroupBehaviourBase_Methods::UpdateFocusAttentionOnThingActive()
{
    for (;;) {
        CTCPerceiveThing* perception = (CTCPerceiveThing*)PTCPerception;
        if (!(perception->AttentionLevel != 0.0f && perception->IsPerceivingTarget() && ((CTCPerceiveThing*)PTCPerception)->Focused))
            return false;
        bool arrived;
        EAIActionReturn result = AIAction_MoveToThingProximity(AI_ACTION_REPEAT_ONCE, *(CIntelligentPointerThing*)PTargetThing,
            2.0f, &arrived, MOVEMENT_TYPE_WALK, true, false, true, true, 0.0f);
        if (result == AI_ACTION_RETURN_RUNNING)
            continue;
        if (result == AI_ACTION_RETURN_SUCCEEDED)
            return TurnToFace();
        return false;
    }
}
