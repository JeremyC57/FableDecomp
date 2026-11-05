// CMessageEvent::CMessageEvent() -- default constructor
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687270, length 52 bytes (+ int3 padding).
// Callees: CBaseObject::CBaseObject() @ 0x0099A380 (stores vtable 0x0129A7D8 and zeroes +4),
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10 twice (members at +0x48 and
// +0x50, each followed by the inline CIntelligentPointer vtable store 0x01238C6C).
// Vtable 0x0125BE8C is CMessageEvent (rtti-vtables.tsv); the manifest label
// (CAIStateGroup_BalverineCornered ctor) is propagated garbage. Each pointer member's address
// is kept in edi across its base constructor call.
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)

class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CThing;

class CMessageEvent : public CBaseObject { public:        // vtable 0x0125BE8C
    unsigned char _pad[0x40];               // +0x08..+0x47 (message payload)
    CIntelligentPointer<CThing> m_pSender;          // +0x48
    CIntelligentPointer<CThing> m_pTarget;          // +0x50
    CMessageEvent();
};

CMessageEvent::CMessageEvent()
    : m_pSender()
    , m_pTarget()
{
}