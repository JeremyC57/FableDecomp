#pragma optimize("s", on)
// CCarryingDef::Copy(const CCarryingDef&) -- copies the CDefClassBase part, the AvailableCarrySlots vector
// (out-of-line operator=) and the OverriddenDummyObject string tag.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E1DDC, length 37 bytes.
// Callees: CDefClassBase::Copy @ 0x00431F10 (manifest "CThingBuildingDef::operator=", propagated),
// std::vector<long>::operator= @ 0x00454886 (manifest "CActiveFile::operator=", propagated).
// PDB CCarryingDef (sizeof 0x3C): AvailableCarrySlots at +0x28 (16-byte PDB vector, 12 in retail) and
// OverriddenDummyObject (CDefString) at +0x38, which the retail body reaches at +0x34. No vtable store and
// no `this` return: a plain member function, not a constructor.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

struct CLongVector {                             // retail std::vector<long>: {first, last, end}
    long* m_pFirst;
    long* m_pLast;
    long* m_pEnd;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

// def vtable 0x01241194: slot 0 0x004DFE78 (implicit inline dtor); real overrides from the def vtable scan
class CCarryingDef : public CDefClassBase { public:
    CLongVector AvailableCarrySlots;             // +0x28
    long        OverriddenDummyObject_TablePos;  // +0x34 CDefString::TablePos
    void Copy(const CCarryingDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004E77C8
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x004E1DDC
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004DD920
};

void CCarryingDef::Copy(const CCarryingDef& other)
{
    CDefClassBase::operator=((const CDefClassBase&)(other));
    AvailableCarrySlots = other.AvailableCarrySlots;
    OverriddenDummyObject_TablePos = other.OverriddenDummyObject_TablePos;
}