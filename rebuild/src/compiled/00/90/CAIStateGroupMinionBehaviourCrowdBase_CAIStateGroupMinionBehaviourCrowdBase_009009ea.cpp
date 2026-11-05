// CAIStateGroup_MinionBehaviourCrowdBase::CAIStateGroup_MinionBehaviourCrowdBase() -- default constructor
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009009EA, length 68 bytes.
// Callees: CAIStateGroupBase::CAIStateGroupBase() @ 0x008FD255, CBaseIntelligentPointer::
// CBaseIntelligentPointer() @ 0x00A01B10 three times (members +0x14, +0x1c, +0x24), each
// followed by the inline vtable store of its own CIntelligentPointer<T> instantiation
// (0x01237F1C, 0x0128B0B8, 0x0128B0C0 -- three different pointee types). Vtable 0x0128C494 is
// CAIStateGroup_MinionBehaviourCrowdBase (rtti-vtables.tsv); the manifest label
// (CarryCrateAndFollow ctor) is propagated. The member address rides in edi across each call.
class CAIStateGroupBase { public:
    virtual void v00();
    CAIStateGroupBase();                    // 0x008FD255
    unsigned char _pad[0x10];               // +0x4..+0x13
};

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CIntelligentPointerOfThing : CBaseIntelligentPointer {          // vtable 0x01237F1C
    CIntelligentPointerOfThing() { }
};
struct CIntelligentPointerOfCrowd : CBaseIntelligentPointer {          // vtable 0x0128B0B8
    CIntelligentPointerOfCrowd() { }
};
struct CIntelligentPointerOfCrowdMember : CBaseIntelligentPointer {    // vtable 0x0128B0C0
    CIntelligentPointerOfCrowdMember() { }
};

struct CAIStateGroup_MinionBehaviourCrowdBase : CAIStateGroupBase {   // vtable 0x0128C494
    CIntelligentPointerOfThing       m_pTargetThing;    // +0x14
    CIntelligentPointerOfCrowd       m_pCrowd;          // +0x1c
    CIntelligentPointerOfCrowdMember m_pCrowdMember;    // +0x24
    CAIStateGroup_MinionBehaviourCrowdBase();
};

CAIStateGroup_MinionBehaviourCrowdBase::CAIStateGroup_MinionBehaviourCrowdBase()
    : m_pTargetThing()
    , m_pCrowd()
    , m_pCrowdMember()
{
}