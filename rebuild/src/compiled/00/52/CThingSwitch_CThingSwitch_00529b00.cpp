// CThingSwitch::CThingSwitch() -- CThing constructed with type 9 (0x005296B0), the CThingGameObject layer's vtable (0x01244F7C) and its intelligent pointer at +0xc0 (CBaseIntelligentPointer @ 0x00A01B10, the CIntelligentPointer vtable 0x01238C6C), then this class's vtable (0x012453FC) and its two CCharString members at +0xd4 and +0xe0 (0x0099E4B0)
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00529B00, length 69 bytes.
// Callees: CThing::CThing(long type) @ 0x005296B0 (the manifest label "CTCDPhysicalObstruction" is propagated), CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CCharString::CCharString @ 0x0099E4B0 (x2). The chunk label "CMapFilenames::CMapFilenames" is propagated: the vtables are CThingGameObject's (rtti 0x01244F7C) and CThingSwitch's (0x012453FC). Shape: the intermediate class's vtable stored before its member's constructor, the derived vtable before the string constructors, the pointer member in edi, `ret`.
class CCharString { public:
    void* m_pStringData;
    CCharString();                                 // 0x0099E4B0
};
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
    unsigned char       _padc8[0xc];               // +0xc8 (left to later initialisation)
    CThingGameObject() : CThing(9), m_owner() { }
};
class CThingSwitch : public CThingGameObject { public:           // vtable 0x012453FC
    virtual void v00();
    CCharString m_onName;                          // +0xd4
    unsigned char _padd8[8];                       // +0xd8
    CCharString m_offName;                         // +0xe0
    CThingSwitch();
};

CThingSwitch::CThingSwitch()
    : CThingGameObject(), m_onName(), m_offName()
{
}