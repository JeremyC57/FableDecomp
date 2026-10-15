// CSpecialAbilitiesSummonSpellDef::operator=(const CSpecialAbilitiesSummonSpellDef&) -- memberwise copy: CParentDefClassBase part, the summon-time vector, the creature def name, three scalars, the tint vector and two dummy-point strings; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00460457, length 93 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<float>::operator= @ 0x0045EF2D, vector<long>::operator= @ 0x00454886,
// CCharString::operator=(const CCharString&) @ 0x0099EFB0 (twice).
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// CSpecialAbilitiesSummonSpellDef's vtable 0x01234674 is the 5-byte `jmp 0x00460457` right before this body, and
// the PDB layout of CSpecialAbilitiesSummonSpellDef sits exactly at the copied offsets once the
// retail 4-byte-smaller containers are applied (PDB +0x40.. -> retail +0x3c..; PDB vectors/maps are 16
// bytes, retail 12).
// Shape: the compiler-style memberwise assignment written out: base part first (out of line), then each
// member in declaration order -- containers and strings through their out-of-line operator=, scalars
// (floats included) as dword moves through eax.
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

struct CDefString {                         // a def-name table position (4 bytes, plain copy)
    long TablePos;
};

struct CCharString {                        // retail ref-counted string (one pointer)
    void* m_pData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct CSpecialAbilitiesSummonSpellDef : CParentDefClassBase { // vtable 0x01234674
    CFloatVector         TimeOfSummon;                // +0x3c
    CDefString           BaseSummonCreatureName;      // +0x48
    float                FadeInTime;                  // +0x4c
    float                FadeOutTime;                 // +0x50
    long                 ResetCreatureManaCost;       // +0x54
    CLongVector          SummonTintColour;            // +0x58
    CCharString          EyeLeftDummyPointName;       // +0x64
    CCharString          EyeRightDummyPointName;      // +0x68
    CSpecialAbilitiesSummonSpellDef& operator=(const CSpecialAbilitiesSummonSpellDef& other);
};

CSpecialAbilitiesSummonSpellDef& CSpecialAbilitiesSummonSpellDef::operator=(const CSpecialAbilitiesSummonSpellDef& other)
{
    CParentDefClassBase::operator=(other);
    TimeOfSummon = other.TimeOfSummon;
    BaseSummonCreatureName = other.BaseSummonCreatureName;
    FadeInTime = other.FadeInTime;
    FadeOutTime = other.FadeOutTime;
    ResetCreatureManaCost = other.ResetCreatureManaCost;
    SummonTintColour = other.SummonTintColour;
    EyeLeftDummyPointName = other.EyeLeftDummyPointName;
    EyeRightDummyPointName = other.EyeRightDummyPointName;
    return *this;
}