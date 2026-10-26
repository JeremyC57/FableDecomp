#pragma optimize("s", on)
// CCarryingDef::Copy(const CCarryingDef&) -- copies the CDefClassBase part, the AvailableCarrySlots vector
// (out-of-line operator=) and the OverriddenDummyObject string tag.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E1DDC, length 37 bytes.
// Callees: CDefClassBase::Copy @ 0x00431F10 (manifest "CThingBuildingDef::operator=", propagated),
// std::vector<long>::operator= @ 0x00454886 (manifest "CActiveFile::operator=", propagated).
// PDB CCarryingDef (sizeof 0x3C): AvailableCarrySlots at +0x28 (16-byte PDB vector, 12 in retail) and
// OverriddenDummyObject (CDefString) at +0x38, which the retail body reaches at +0x34. No vtable store and
// no `this` return: a plain member function, not a constructor.
class CDefClassBase { public:
    virtual void v00();
    unsigned char _body[0x24];                   // +0x04..+0x27
    CDefClassBase& operator=(const CDefClassBase& other);       // 0x00431F10
};

struct CLongVector {                             // retail std::vector<long>: {first, last, end}
    long* m_pFirst;
    long* m_pLast;
    long* m_pEnd;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

class CCarryingDef : public CDefClassBase { public:
    CLongVector AvailableCarrySlots;             // +0x28
    long        OverriddenDummyObject_TablePos;  // +0x34 CDefString::TablePos
    void Copy(const CCarryingDef& other);
};

void CCarryingDef::Copy(const CCarryingDef& other)
{
    CDefClassBase::operator=((const CDefClassBase&)(other));
    AvailableCarrySlots = other.AvailableCarrySlots;
    OverriddenDummyObject_TablePos = other.OverriddenDummyObject_TablePos;
}