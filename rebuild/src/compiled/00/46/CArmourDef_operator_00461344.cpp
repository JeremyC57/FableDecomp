// CArmourDef::operator=(const CArmourDef&) -- memberwise assignment: base CParentDefClassBase::operator=, the three response maps by their operator=, three flag bytes and two material ids; returns *this
// Leaf: ??4CArmourDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00461344, length 87 bytes.
// Pragma: #pragma optimize("s", on) (one register, eax, for every copy pair).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0,
//          map<EObjectAugmentationType,float>::operator= @ 0x0046139B,
//          map<EDamageAttribute,float>::operator= @ 0x004614C0 (twice: DamageTypeResponse, ArmourThresholds).
// Identity: the manifest label "CParentDefClassBase::operator=" is propagated. The body is reached
// through a `jmp` thunk at 0x0046133F that sits in slot 19 of vtable 0x01234AEC = CArmourDef
// (rtti-vtables.tsv), and the touched offsets are the PDB CArmourDef members shifted down by the
// 4 bytes the retail CParentDefClassBase is narrower (0x3c, PDB 0x40) and with 12-byte retail maps
// (PDB 16): AugmentationResponse +0x3c, DamageTypeResponse +0x48, ArmourThresholds +0x54,
// AllHitsKnockdown/AllHitsNegated/AllHitsCauseRecoil +0x60..+0x62, DamageMaterial +0x64,
// BlockMaterial +0x68.
// Shape: the compiler-generated memberwise assignment -- base assignment out of line, then each
// member in declaration order: the maps through their out-of-line operator=, the bools and longs
// one load/store pair each through eax. The PDB header (engine/CParentDefClassBase.h) describes the
// Ego layout (0x3d bytes, 16-byte CVectorMap), so the 0x3c-byte retail base is modelled locally.
#pragma optimize("s", on)   // the def module is size-optimised: every copy pair goes through eax

class CParentDefClassBase { public:
    virtual void v00();
    unsigned char _body[0x38];                                            // +0x04..+0x3c
    CParentDefClassBase& operator=(const CParentDefClassBase& other);     // 0x00431ED0
};

struct CAugmentationResponseMap {                                         // retail std::map (12 bytes)
    void*         m_allocator;
    void*         m_head;
    unsigned long m_size;
    CAugmentationResponseMap& operator=(const CAugmentationResponseMap& other);   // 0x0046139B
};

struct CDamageAttributeMap {                                              // retail std::map (12 bytes)
    void*         m_allocator;
    void*         m_head;
    unsigned long m_size;
    CDamageAttributeMap& operator=(const CDamageAttributeMap& other);     // 0x004614C0
};

class CArmourDef : public CParentDefClassBase { public:                                 // vtable 0x01234AEC
    CAugmentationResponseMap AugmentationResponse;                        // +0x3c
    CDamageAttributeMap      DamageTypeResponse;                          // +0x48
    CDamageAttributeMap      ArmourThresholds;                            // +0x54
    bool                     AllHitsKnockdown;                            // +0x60
    bool                     AllHitsNegated;                              // +0x61
    bool                     AllHitsCauseRecoil;                          // +0x62
    long                     DamageMaterial;                              // +0x64
    long                     BlockMaterial;                               // +0x68
    virtual void v00();
    CArmourDef& operator=(const CArmourDef& other);
};

CArmourDef& CArmourDef::operator=(const CArmourDef& other)
{
    CParentDefClassBase::operator=(other);
    AugmentationResponse = other.AugmentationResponse;
    DamageTypeResponse = other.DamageTypeResponse;
    ArmourThresholds = other.ArmourThresholds;
    AllHitsKnockdown = other.AllHitsKnockdown;
    AllHitsNegated = other.AllHitsNegated;
    AllHitsCauseRecoil = other.AllHitsCauseRecoil;
    DamageMaterial = other.DamageMaterial;
    BlockMaterial = other.BlockMaterial;
    return *this;
}