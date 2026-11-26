// CAIStateGroup_MinionBehaviourCrowdBase::CAIStateGroup_MinionBehaviourCrowdBase() -- default constructor
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Leaf: ??0CAIStateGroup_MinionBehaviourCrowdBase@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009009EA, length 68 bytes.
// NF473 (callee-only, MinionKidCalledToMother unit; target 0x0128B0C0): the +0x24 member is Ego_r.pdb's MyMessageEvent, a
//   CIntelligentPointer<const CMessageEvent> (struct_layouts_egor.tsv), the type of retail's vftable 0x0128B0C0 (RTTI
//   .?AV?$CIntelligentPointer@$$CBVCMessageEvent@@@@), from the shared template (engine/CIntelligentPointer.h); was the local stand-in
//   CIntelligentPointerOfCrowdMember (??_7CIntelligentPointerOfCrowdMember@@6B@). Same bytes. The other two members keep their HEAD stand-ins.
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
class CMessageEvent;

struct CAIStateGroup_MinionBehaviourCrowdBase : CAIStateGroupBase {   // vtable 0x0128C494
    CIntelligentPointerOfThing       m_pTargetThing;    // +0x14
    CIntelligentPointerOfCrowd       m_pCrowd;          // +0x1c
    CIntelligentPointer<const CMessageEvent> MyMessageEvent;   // +0x24 (Ego_r.pdb; vtable 0x0128B0C0)
    CAIStateGroup_MinionBehaviourCrowdBase();
};

CAIStateGroup_MinionBehaviourCrowdBase::CAIStateGroup_MinionBehaviourCrowdBase()
    : m_pTargetThing()
    , m_pCrowd()
    , MyMessageEvent()
{
}