// CAIStateGroupBehaviourBase::TurnToFace() -- one look-at-thing step at PTargetThing; true once facing it.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00957750, length 63 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem
// @ 0x00A01B90 (the by-value CIntelligentPointer<const CThing> argument, vtable 0x01237F1C, copy-constructed in
// its argument slot from PTargetThing at +0x14), CAIStateGroupBase::AIAction_LookAtThing(EAIActionRepeat,
// CIntelligentPointer<const CThing>, long, float) @ 0x0097D6F0 (repeat 1, flags 0x23, turn speed 0.0027777f).
// Shape: the two literals are pushed first, then the 8-byte by-value pointer is built in its slot (`sub esp,8 ;
// mov esi,esp`, base constructor, vtable, SetPItem(GetPItem())), then the repeat mode; the callee destroys the
// copy. `cmp eax,2 ; sete al` is `return r == 2 ? true : false` (a plain `return r == 2` goes through ecx).
// The manifest module "CAIStateGroup_OpinionReactionRoot" is wrong: the target pointer at +0x14 is
// CAIStateGroupBehaviourBase::PTargetThing (its Cleanup @ 0x00957790 and OnActivate @ 0x009576C0 surround it).
struct CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // vtable 0x01237F1C
    CIntelligentPointerConstThing(const CIntelligentPointerConstThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // +0x4..+0x13
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long flags, float turnSpeed);   // 0x0097D6F0
};

struct CAIStateGroupBehaviourBase : CAIStateGroupBase {
    CIntelligentPointerConstThing PTargetThing; // +0x14
    bool TurnToFace();
};

bool CAIStateGroupBehaviourBase::TurnToFace()
{
    return AIAction_LookAtThing(1, PTargetThing, 0x23, 0.0027777f) == 2 ? true : false;
}