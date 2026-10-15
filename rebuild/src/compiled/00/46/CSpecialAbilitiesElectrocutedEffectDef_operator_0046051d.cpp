// CSpecialAbilitiesElectrocutedEffectDef::operator=(const CSpecialAbilitiesElectrocutedEffectDef&) -- memberwise copy: CParentDefClassBase part, three fade times, the two colour vectors and three sizes; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046051D, length 81 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<long>::operator= @ 0x00454886 (twice).
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// CSpecialAbilitiesElectrocutedEffectDef's vtable 0x01234744 is the 5-byte `jmp 0x0046051D` right before this body, and
// the PDB layout of CSpecialAbilitiesElectrocutedEffectDef sits exactly at the copied offsets once the
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

struct CLongVector {                    // retail std::vector<long> (12 bytes)
    long* m_first;
    long* m_last;
    long* m_end;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

struct CSpecialAbilitiesElectrocutedEffectDef : CParentDefClassBase { // vtable 0x01234744
    float                FadeInTimeSecs;              // +0x3c
    float                FadeOutTimeStartSecs;        // +0x40
    float                FadeOutTimeSecs;             // +0x44
    CLongVector          Color;                       // +0x48
    CLongVector          HighlightColor;              // +0x54
    float                HighlightWidth;              // +0x60
    float                HeightSmall;                 // +0x64
    float                HeightMedium;                // +0x68
    CSpecialAbilitiesElectrocutedEffectDef& operator=(const CSpecialAbilitiesElectrocutedEffectDef& other);
};

CSpecialAbilitiesElectrocutedEffectDef& CSpecialAbilitiesElectrocutedEffectDef::operator=(const CSpecialAbilitiesElectrocutedEffectDef& other)
{
    CParentDefClassBase::operator=(other);
    FadeInTimeSecs = other.FadeInTimeSecs;
    FadeOutTimeStartSecs = other.FadeOutTimeStartSecs;
    FadeOutTimeSecs = other.FadeOutTimeSecs;
    Color = other.Color;
    HighlightColor = other.HighlightColor;
    HighlightWidth = other.HighlightWidth;
    HeightSmall = other.HeightSmall;
    HeightMedium = other.HeightMedium;
    return *this;
}