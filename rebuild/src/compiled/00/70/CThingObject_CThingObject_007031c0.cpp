// CThingObject::CThingObject() -- CThing constructed with type 5, the CThingGameObject layer (vtable 0x01244F7C, intelligent pointer at +0xc0), then CThingObject's vtable and its PDef / DieOffTimer / DiesOff cleared
// Leaf: ??0CThingObject
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007031C0, length 67 bytes.
// Callees: CThing::CThing(long type) @ 0x005296B0 (manifest "CTCDPhysicalObstruction" is propagated),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (then the CIntelligentPointer vtable 0x01238C6C).
// Own vtable 0x01264DCC. The landed CThingSwitch constructor 0x00529B00 model (same layering);
// CThingObject members from the PDB (retail 0x18 lower): PDef (CDefPointer) +0xd0,
// DieOffTimer +0xd4, DiesOff +0xd8.
// Shape: the intermediate vtable store before the member constructor, the member's own vtable
// after it, then the derived vtable and the zeroes from one `xor eax,eax`: PDef is a member
// OBJECT with an inline constructor (a plain pointer initialiser sinks the vtable store below
// every zero store).
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CThing { public:                                    // 0xc0 bytes
    virtual void v00();
    unsigned char _body[0xbc];
    CThing(long type);                             // 0x005296B0
};
class CThingGameObject : public CThing { public:                 // vtable 0x01244F7C
    virtual void v00();
    CIntelligentPointer<CThing> m_owner;                   // +0xc0
    unsigned char       _padc8[0x8];               // +0xc8
    CThingGameObject(long type) : CThing(type), m_owner() { }
};
class CThingObjectDef;
struct CDefPointerThingObjectDef {                 // CDefPointer<const CThingObjectDef>
    const CThingObjectDef* m_pDef;
    CDefPointerThingObjectDef() : m_pDef(0) { }
};
class CThingObject : public CThingGameObject { public:           // vtable 0x01264DCC
    virtual void v00();
    CDefPointerThingObjectDef PDef;                // +0xd0
    long                   DieOffTimer;            // +0xd4
    bool                   DiesOff;                // +0xd8
    CThingObject();
};

CThingObject::CThingObject()
    : CThingGameObject(5), PDef(), DieOffTimer(0), DiesOff(false)
{
}