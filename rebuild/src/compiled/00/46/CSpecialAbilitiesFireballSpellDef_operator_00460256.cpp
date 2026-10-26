// CSpecialAbilitiesFireballSpellDef::operator=(const CSpecialAbilitiesFireballSpellDef&) -- memberwise copy: CParentDefClassBase part, two floats, the per-level stamina vector and the targetting arc; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00460256, length 51 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<float>::operator= @ 0x0045EF2D.
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// CSpecialAbilitiesFireballSpellDef's vtable 0x012345A4 is the 5-byte `jmp 0x00460256` right before this body, and
// the PDB layout of CSpecialAbilitiesFireballSpellDef sits exactly at the copied offsets once the
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

class CSpecialAbilitiesFireballSpellDef : public CParentDefClassBase { public: // vtable 0x012345A4
    float                ReleaseDelay;                // +0x3c
    float                Inclination;                 // +0x40
    CFloatVector         StaminaCost;                 // +0x44
    float                TargettingArcAngle;          // +0x50
    CSpecialAbilitiesFireballSpellDef& operator=(const CSpecialAbilitiesFireballSpellDef& other);
};

CSpecialAbilitiesFireballSpellDef& CSpecialAbilitiesFireballSpellDef::operator=(const CSpecialAbilitiesFireballSpellDef& other)
{
    CParentDefClassBase::operator=(other);
    ReleaseDelay = other.ReleaseDelay;
    Inclination = other.Inclination;
    StaminaCost = other.StaminaCost;
    TargettingArcAngle = other.TargettingArcAngle;
    return *this;
}