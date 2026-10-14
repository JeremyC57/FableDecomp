// CAIStateGroup_OpinionReactionRoot::TurnToFace() -- one look-at-thing step at ThingReactingTo; terminate when done turning fails.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095EDF0, length 76 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem
// @ 0x00A01B90 (the by-value CIntelligentPointer<const CThing> argument, vtable 0x01237F1C, converted in its
// argument slot from the CIntelligentPointer<CThing> member at +0x6b), CAIStateGroupBase::AIAction_LookAtThing
// @ 0x0097D6F0 (repeat 1, flags 0x21, turn speed 0.0027777f), CAIStateGroupBase::Terminate @ 0x008FCF40.
// Shape: literals pushed, the 8-byte copy built in its slot (base constructor, vtable, SetPItem(GetPItem())),
// then `if (r != 2) { Terminate(); return false; } return true;` (the failure path falls through, the
// `mov al,1` tail is out of line). The member is unaligned
// at +0x6b (engine header, pack 1), so the class is modelled packed.
struct CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer { };   // vtable 0x01238C6C (member type)

struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // vtable 0x01237F1C (argument type)
    CIntelligentPointerConstThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // +0x4..+0x13
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long flags, float turnSpeed);   // 0x0097D6F0
    void Terminate();                           // 0x008FCF40
};

struct CTCTalk;

#pragma pack(push, 1)
struct CAIStateGroup_OpinionReactionRoot : CAIStateGroupBase {
    CTCTalk* PTCTalk;                           // +0x14
    long CurrentReactionType;                   // +0x18
    bool ReactionFlags[79];                     // +0x1c
    CIntelligentPointerThing ThingReactingTo;   // +0x6b
    bool TurnToFace();
};
#pragma pack(pop)

bool CAIStateGroup_OpinionReactionRoot::TurnToFace()
{
    if (AIAction_LookAtThing(1, ThingReactingTo, 0x21, 0.0027777f) != 2) {
        Terminate();
        return false;
    }
    return true;
}