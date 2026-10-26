#pragma optimize("s", on)
// CHitLocationDef::CHitLocationDef() -- CParentDefClassBase(), this vtable, then the embedded
// CPhysicalPrimitiveInit member zeroed by its inline constructor.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004585E0, length 26 bytes.
// Callees: CParentDefClassBase::CParentDefClassBase @ 0x00430370. Vtable 0x01234B54 = CHitLocationDef
// (rtti-vtables.tsv); the manifest label "CCarriedReadableDef" is propagated and rejected.
// PDB CHitLocationDef: PhysicalPrimitive (CPhysicalPrimitiveInit, 8 bytes) at +0x40 behind a 0x3D-byte
// CParentDefClassBase; the retail base is 0x3C bytes (its CDefString is 4 bytes shorter), so the member
// sits at +0x3C / +0x40. The vtable store precedes the zero stores (`and dword ptr,0` = size TU) because
// the zeroed words belong to an embedded member object with an inline constructor.
class CParentDefClassBase { public:
    virtual void v00();
    CParentDefClassBase();                       // 0x00430370
};

class CPhysicalPrimitiveInit { public:
    long m_type;                                 // +0x00
    long m_param;                                // +0x04
    CPhysicalPrimitiveInit() : m_type(0), m_param(0) { }
};

class CHitLocationDef : public CParentDefClassBase { public:
    unsigned char          _body[0x38];          // +0x04..+0x3B base members
    CPhysicalPrimitiveInit PhysicalPrimitive;    // +0x3C
    virtual void v00();                          // own vtable (0x01234B54)
    CHitLocationDef();
};

CHitLocationDef::CHitLocationDef()
    : CParentDefClassBase(), PhysicalPrimitive()
{
}