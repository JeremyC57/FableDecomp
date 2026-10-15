// COpinionDeedMaskDef::operator=(const COpinionDeedMaskDef&) -- memberwise copy: CParentDefClassBase part, the default flag byte (+0x39, right after the packed base) and the per-deed map; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045E187, length 39 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, map<EOpinionDeedType,bool>::operator= @ 0x0045E1AE
// (manifest "CTCCarryable_AssignClonedTree3", propagated).
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// COpinionDeedMaskDef's vtable 0x01233DEC is the 5-byte `jmp 0x0045E187` right before this body, and
// the PDB layout of COpinionDeedMaskDef sits exactly at the copied offsets once the
// retail 4-byte-smaller containers are applied (PDB +0x3d/+0x40 -> retail +0x39/+0x3c; PDB vectors/maps are 16
// bytes, retail 12).
// Shape: the compiler-style memberwise assignment written out: base part first (out of line), then each
// member in declaration order -- containers and strings through their out-of-line operator=, scalars
// (floats included) as dword moves through eax.
// Twin of COpinionReactionMaskDef::operator= (0x0045DD05) with the deed map's operator=.
// The engine header CParentDefClassBase.h is the PDB layout (0x3d bytes, 16-byte SubDefInfoMap); the
// retail base is 0x39 bytes (12-byte vector map), so it is modelled locally as a packed polymorphic base
// whose operator= is out of line; the derived members follow at their natural alignment.
#pragma pack(push, 1)
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _defClassBase[0x35];      // +0x04 CDefClassBase part, InstantiationName (+0x25), SubDefInfoMap (+0x29)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

struct CDeedEnabledMap {                // retail map<EOpinionDeedType,bool> (12 bytes)
    void*         m_allocator;
    void*         m_head;
    unsigned long m_size;
    CDeedEnabledMap& operator=(const CDeedEnabledMap& other);   // 0x0045E1AE
};

struct COpinionDeedMaskDef : CParentDefClassBase { // vtable 0x01233DEC
    bool                 DeedEnabledDefault;          // +0x39
    CDeedEnabledMap      DeedEnabled;                 // +0x3c
    COpinionDeedMaskDef& operator=(const COpinionDeedMaskDef& other);
};

COpinionDeedMaskDef& COpinionDeedMaskDef::operator=(const COpinionDeedMaskDef& other)
{
    CParentDefClassBase::operator=(other);
    DeedEnabledDefault = other.DeedEnabledDefault;
    DeedEnabled = other.DeedEnabled;
    return *this;
}
