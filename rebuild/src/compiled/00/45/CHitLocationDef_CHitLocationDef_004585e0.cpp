#pragma optimize("s", on)
// CHitLocationDef::CHitLocationDef() -- CParentDefClassBase(), this vtable, then the embedded
// CPhysicalPrimitiveInit member zeroed by its inline constructor.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004585E0, length 26 bytes.
// Callees: CParentDefClassBase::CParentDefClassBase @ 0x00430370. Vtable 0x01234B54 = CHitLocationDef
// (rtti-vtables.tsv); the manifest label "CCarriedReadableDef" is propagated and rejected.
// PDB CHitLocationDef: PhysicalPrimitive (CPhysicalPrimitiveInit, 8 bytes) at +0x40 behind a 0x3D-byte
// CParentDefClassBase; the retail base is 0x39 bytes packed (its CDefString is 4 bytes shorter), so the 4-aligned member
// sits at +0x3C / +0x40. The vtable store precedes the zero stores (`and dword ptr,0` = size TU) because
// the zeroed words belong to an embedded member object with an inline constructor.
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"

class CPhysicalPrimitiveInit { public:
    long m_type;                                 // +0x00
    long m_param;                                // +0x04
    CPhysicalPrimitiveInit() : m_type(0), m_param(0) { }
};

class CHitLocationDef : public CParentDefClassBase { public:
    virtual void Transfer(CPersistContext& context);              // ?Transfer@CHitLocationDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase* other);                // ?Copy@CHitLocationDef@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                       // ?GetSizeofClass@CHitLocationDef@@UAEKXZ
    CPhysicalPrimitiveInit PhysicalPrimitive;    // +0x3C
    CHitLocationDef();
};

CHitLocationDef::CHitLocationDef()
    : CParentDefClassBase(), PhysicalPrimitive()
{
}