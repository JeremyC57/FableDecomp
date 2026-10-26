// COpinionReactionMaskDef::operator=(const COpinionReactionMaskDef&) -- memberwise copy: CParentDefClassBase part, the default flag byte (+0x39, right after the packed base) and the per-reaction map; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045DD05, length 39 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, map<EOpinionReactionType,bool>::operator= @ 0x0045DD2C
// (manifest "CTCCarryable_CopyAssign", propagated).
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// COpinionReactionMaskDef's vtable 0x01233D84 is the 5-byte `jmp 0x0045DD05` right before this body, and
// the PDB layout of COpinionReactionMaskDef sits exactly at the copied offsets once the
// retail 4-byte-smaller containers are applied (PDB +0x3d/+0x40 -> retail +0x39/+0x3c; PDB vectors/maps are 16
// bytes, retail 12).
// Shape: the compiler-style memberwise assignment written out: base part first (out of line), then each
// member in declaration order -- containers and strings through their out-of-line operator=, scalars
// (floats included) as dword moves through eax.
// the bool lands at +0x39 because the retail base is 0x39 bytes and a byte needs no alignment;
// the map follows at +0x3c. The byte store sinks below `add edi,3Ch` (last-member address).
// The engine header CParentDefClassBase.h is the PDB layout (0x3d bytes, 16-byte SubDefInfoMap); the
// retail base is 0x39 bytes (12-byte vector map), so it is modelled locally as a packed polymorphic base
// whose operator= is out of line; the derived members follow at their natural alignment.
#pragma pack(push, 1)
class CParentDefClassBase { public:
    virtual void v00();
    unsigned char _defClassBase[0x35];      // +0x04 CDefClassBase part, InstantiationName (+0x25), SubDefInfoMap (+0x29)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

struct CReactionEnabledMap {            // retail map<EOpinionReactionType,bool> (12 bytes)
    void*         m_allocator;
    void*         m_head;
    unsigned long m_size;
    CReactionEnabledMap& operator=(const CReactionEnabledMap& other);   // 0x0045DD2C
};

class COpinionReactionMaskDef : public CParentDefClassBase { public: // vtable 0x01233D84
    bool                 ReactionEnabledDefault;      // +0x39
    CReactionEnabledMap  ReactionEnabled;             // +0x3c
    COpinionReactionMaskDef& operator=(const COpinionReactionMaskDef& other);
};

COpinionReactionMaskDef& COpinionReactionMaskDef::operator=(const COpinionReactionMaskDef& other)
{
    CParentDefClassBase::operator=(other);
    ReactionEnabledDefault = other.ReactionEnabledDefault;
    ReactionEnabled = other.ReactionEnabled;
    return *this;
}