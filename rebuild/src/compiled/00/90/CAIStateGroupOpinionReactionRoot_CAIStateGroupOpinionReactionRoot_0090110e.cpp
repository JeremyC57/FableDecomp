// CAIStateGroup_OpinionReactionRoot::CAIStateGroup_OpinionReactionRoot() -- base, own vtable, the ThingReactingTo
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// pointer at +0x6b, LastRunWasTryAndLook cleared.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090110E, length 43 bytes.
// Callees: CAIStateGroupBase::CAIStateGroupBase() @ 0x008FD255, CBaseIntelligentPointer::CBaseIntelligentPointer()
// @ 0x00A01B10 (the member at +0x6b, vtable 0x01238C6C = CIntelligentPointer<CThing>). Own vtable 0x0128C984
// (rtti-vtables: CAIStateGroup_OpinionReactionRoot).
// Shape: base constructor, the derived vtable store, the member constructor (the CIntelligentPointer's own vtable
// store follows its base call), then the body assignment `LastRunWasTryAndLook = false` (+0x88). The engine
// header (pack 1) puts the 79 reaction flag bytes at +0x1c, so the pointer member is unaligned at +0x6b and the
// class is modelled packed; the other members are not initialised here.
class CThing;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // +0x4..+0x13
    CAIStateGroupBase();                        // 0x008FD255
};

class CTCTalk;

#pragma pack(push, 1)
class CAIStateGroup_OpinionReactionRoot : public CAIStateGroupBase { public:   // vtable 0x0128C984
    CTCTalk* PTCTalk;                           // +0x14
    long CurrentReactionType;                   // +0x18 EOpinionReactionType
    bool ReactionFlags[79];                     // +0x1c
    CIntelligentPointer<CThing> ThingReactingTo;   // +0x6b
    unsigned char _pad_0x73[0x5];               // +0x73
    unsigned char CurrentUpdateFunc[0x10];      // +0x78 pointer to member function
    bool LastRunWasTryAndLook;                  // +0x88
    bool ReactingToPlayer;                      // +0x89
    bool AllowSpeechOnNonPureAISpeaker;         // +0x8a
    unsigned char _pad_0x8b;
    float ExtraDataFloat;                       // +0x8c
    long ExtraDataSLONG;                        // +0x90
    CThing* ExtraDataPThing;                    // +0x94
    CAIStateGroup_OpinionReactionRoot();
};
#pragma pack(pop)

CAIStateGroup_OpinionReactionRoot::CAIStateGroup_OpinionReactionRoot()
    : CAIStateGroupBase()
    , ThingReactingTo()
{
    LastRunWasTryAndLook = false;
}