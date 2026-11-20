// CAIStateGroup_OpinionReactionRoot::TurnToFace() -- one look-at-thing step at ThingReactingTo; terminate when done turning fails.
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095EDF0, length 76 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem
// @ 0x00A01B90 (the by-value CIntelligentPointer<const CThing> argument, vtable 0x01237F1C, converted in its
// argument slot from the CIntelligentPointer<CThing> member at +0x6b), CAIStateGroupBase::AIAction_LookAtThing
// @ 0x0097D6F0 (repeat 1, flags 0x21, turn speed 0.0027777f), CAIStateGroupBase::Terminate @ 0x008FCF40.
// Shape: literals pushed, the 8-byte copy built in its slot (base constructor, vtable, SetPItem(GetPItem())),
// then `if (r != 2) { Terminate(); return false; } return true;` (the failure path falls through, the
// `mov al,1` tail is out of line). The member is unaligned
// at +0x6b (engine header, pack 1), so the class is modelled packed.
#include "engine/EAIAction.h"
class CThing;

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE
#include "engine/CIntelligentPointer.h"

// CIntelligentPointer<CThing> (member, vtable 0x01238C6C) and the by-value CIntelligentPointer<const CThing> argument (vtable
// 0x01237F1C, built in its slot through the shared converting constructor) are the shared engine/CIntelligentPointer.h templates.

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // +0x4..+0x13
    protected: EAIActionReturn AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointer<const CThing> thing, long flags, float turnSpeed); public:   // 0x0097D6F0
    void Terminate();                           // 0x008FCF40
};

class CTCTalk;

#pragma pack(push, 1)
class CAIStateGroup_OpinionReactionRoot : public CAIStateGroupBase { public:
    CTCTalk* PTCTalk;                           // +0x14
    long CurrentReactionType;                   // +0x18
    bool ReactionFlags[79];                     // +0x1c
    CIntelligentPointer<CThing> ThingReactingTo;   // +0x6b
    bool TurnToFace();
};
#pragma pack(pop)

bool CAIStateGroup_OpinionReactionRoot::TurnToFace()
{
    if (AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED, ThingReactingTo, 0x21, 0.0027777f) != 2) {
        Terminate();
        return false;
    }
    return true;
}