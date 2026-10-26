// CFlammableDef::operator=(const CFlammableDef&) -- def copy: CDefClassBase part, fire scalars, fire-effect creation set
// Leaf: ??4CFlammableDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E5839, 63 bytes, size-optimised TU (/Os def region, like the landed
//         0x004663AE / 0x004E05AD def assignments).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the manifest label "CThingBuildingDef::operator=" is propagated
//          over every def base assignment), std::map<CDefString,long>::operator= @ 0x004E5878 (labelled "CTCCarryable::operator_assign", propagated).
// Identification: reached through the incremental-link thunk 0x004E5834 (manifest "CFlammableDef::Copy", vtable 0x012429D0);
//   PDB CFlammableDef = CDefClassBase + FireResistance +0x28, MaxDamageCaused, FireDamagePerCycle, FireDamagePeriod (floats)
//   +0x2c..+0x34, EffectCreationSet (CFireEffectCreationSet: vptr, byte +4, ContainmentMap +8) +0x38.
// Shape: the set's assignment is inlined (`__forceinline`; the DDK out-lines the implicit one): its vptr is not copied,
//   the byte at +0x3c is loaded before `add edi,40h` and stored after the `lea ecx`, then the map's out-of-line operator=.
// The generated engine headers flatten the bases (no base operator= to call), so the classes are modelled locally.
#pragma optimize("s", on)

class CDefClassBase { public:
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct CContainmentMap {                // std::map<CDefString, long, CCompareDefString<long> > (retail STLport _Rb_tree, 12 bytes)
    void* m_pHeader;
    unsigned long m_nodeCount;
    unsigned char m_keyCompare;
    CContainmentMap& operator=(const CContainmentMap& other);   // 0x004E5878
};

class CFireEffectCreationSet { public:         // PDB 0x18 bytes (retail 0x14): vptr, a base byte at +4, ContainmentMap +8
    virtual void v00();
    bool m_baseFlag;                    // +0x04 (unnamed base-class byte in the PDB)
    CContainmentMap ContainmentMap;     // +0x08
    __forceinline CFireEffectCreationSet& operator=(const CFireEffectCreationSet& other)
    {
        m_baseFlag = other.m_baseFlag;
        ContainmentMap = other.ContainmentMap;
        return *this;
    }
};

class CFlammableDef : public CDefClassBase { public:
    long FireResistance;                // +0x28
    float MaxDamageCaused;              // +0x2c
    float FireDamagePerCycle;           // +0x30
    float FireDamagePeriod;             // +0x34
    CFireEffectCreationSet EffectCreationSet;// +0x38
    CFlammableDef& operator=(const CFlammableDef& other);
};

CFlammableDef& CFlammableDef::operator=(const CFlammableDef& other)
{
    CDefClassBase::operator=(other);
    FireResistance = other.FireResistance;
    MaxDamageCaused = other.MaxDamageCaused;
    FireDamagePerCycle = other.FireDamagePerCycle;
    FireDamagePeriod = other.FireDamagePeriod;
    EffectCreationSet = other.EffectCreationSet;
    return *this;
}