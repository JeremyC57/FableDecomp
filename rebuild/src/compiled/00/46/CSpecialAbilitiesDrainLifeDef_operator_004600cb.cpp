// CSpecialAbilitiesDrainLifeDef::operator=(const CSpecialAbilitiesDrainLifeDef&) -- memberwise copy: CParentDefClassBase part, four per-level vectors and the spell delay; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004600CB, length 75 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<float>::operator= @ 0x0045EF2D, vector<long>::operator= @ 0x00454886,
// vector<CCharString>::operator= @ 0x00432EE9 (manifest labels "CActiveFile::operator=" / "CCombatWheel::operator=", propagated).
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// CSpecialAbilitiesDrainLifeDef's vtable 0x01234404 is the 5-byte `jmp 0x004600CB` right before this body, and
// the PDB layout of CSpecialAbilitiesDrainLifeDef sits exactly at the copied offsets once the
// retail 4-byte-smaller containers are applied (PDB +0x40.. -> retail +0x3c..; PDB vectors/maps are 16
// bytes, retail 12).
// Shape: the compiler-style memberwise assignment written out: base part first (out of line), then each
// member in declaration order -- containers and strings through their out-of-line operator=, scalars
// (floats included) as dword moves through eax.
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

struct CFloatVector {                   // retail std::vector<float> (12 bytes)
    float* m_first;
    float* m_last;
    float* m_end;
    CFloatVector& operator=(const CFloatVector& other);   // 0x0045EF2D
};

struct CLongVector {                    // retail std::vector<long> (12 bytes)
    long* m_first;
    long* m_last;
    long* m_end;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

struct CStringVector {                  // retail std::vector<CCharString> (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CStringVector& operator=(const CStringVector& other);   // 0x00432EE9
};

class CSpecialAbilitiesDrainLifeDef : public CParentDefClassBase { public: // vtable 0x01234404
    CFloatVector         AmountOfHealthSucked;        // +0x3c
    CLongVector          NumEffects;                  // +0x48
    CFloatVector         SpellRange;                  // +0x54
    CStringVector        ShotName;                    // +0x60
    float                SpellDelaySecs;              // +0x6c
    CSpecialAbilitiesDrainLifeDef& operator=(const CSpecialAbilitiesDrainLifeDef& other);
};

CSpecialAbilitiesDrainLifeDef& CSpecialAbilitiesDrainLifeDef::operator=(const CSpecialAbilitiesDrainLifeDef& other)
{
    CParentDefClassBase::operator=(other);
    AmountOfHealthSucked = other.AmountOfHealthSucked;
    NumEffects = other.NumEffects;
    SpellRange = other.SpellRange;
    ShotName = other.ShotName;
    SpellDelaySecs = other.SpellDelaySecs;
    return *this;
}