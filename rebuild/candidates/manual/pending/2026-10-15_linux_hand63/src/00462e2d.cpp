// CPlayerInventoryDef::operator=(const CPlayerInventoryDef&) -- memberwise assignment: base CParentDefClassBase::operator=, three vector<long> and the vector<bool> by their operator=, then InitialSelectedSlot; returns *this
// Leaf: ??4CPlayerInventoryDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00462E2D, length 75 bytes.
// Pragma: #pragma optimize("s", on) (the def module; one register per copy pair).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0,
//          vector<long>::operator= @ 0x00454886 (Slot, MaxInSlot, InitialAmount),
//          vector<bool>::operator= @ 0x00462E78 (IsUsable).
// Identity: the manifest label "CParentDefClassBase::operator=" is propagated. The body is reached
// through a `jmp` thunk at 0x00462E28 in slot 19 of vtable 0x01235274 = CPlayerInventoryDef
// (rtti-vtables.tsv); PDB CPlayerInventoryDef = three vector<long> + vector<bool> + a long, here
// at +0x3c/+0x48/+0x54/+0x60/+0x6c (retail base 0x3c; retail vectors, vector<bool> included, are
// 12 bytes).
// Shape: compiler-generated memberwise assignment in declaration order. The PDB header
// (engine/CParentDefClassBase.h) describes the Ego layout, so the retail base is modelled locally.
#pragma optimize("s", on)   // the def module is size-optimised

struct CParentDefClassBase {
    virtual void v00();
    unsigned char _body[0x38];                                            // +0x04..+0x3c
    CParentDefClassBase& operator=(const CParentDefClassBase& other);     // 0x00431ED0
};

struct CLongVector {                                                      // retail std::vector<long> (12 bytes)
    long* m_first;
    long* m_last;
    long* m_end;
    CLongVector& operator=(const CLongVector& other);                     // 0x00454886
};

struct CBoolVector {                                                      // retail std::vector<bool> (12 bytes)
    unsigned long* m_words;
    unsigned long  m_size;
    unsigned long  m_capacity;
    CBoolVector& operator=(const CBoolVector& other);                     // 0x00462E78
};

struct CPlayerInventoryDef : CParentDefClassBase {                        // vtable 0x01235274
    CLongVector Slot;                                                     // +0x3c
    CLongVector MaxInSlot;                                                // +0x48
    CLongVector InitialAmount;                                            // +0x54
    CBoolVector IsUsable;                                                 // +0x60
    long        InitialSelectedSlot;                                      // +0x6c
    virtual void v00();
    CPlayerInventoryDef& operator=(const CPlayerInventoryDef& other);
};

CPlayerInventoryDef& CPlayerInventoryDef::operator=(const CPlayerInventoryDef& other)
{
    CParentDefClassBase::operator=(other);
    Slot = other.Slot;
    MaxInSlot = other.MaxInSlot;
    InitialAmount = other.InitialAmount;
    IsUsable = other.IsUsable;
    InitialSelectedSlot = other.InitialSelectedSlot;
    return *this;
}
