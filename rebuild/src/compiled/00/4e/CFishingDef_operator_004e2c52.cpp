// CFishingDef::operator=(const CFishingDef&) -- def copy: CDefClassBase part, then the CFishingDef fields memberwise in declaration order
// Leaf: ??4CFishingDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E2C52, 141 bytes, size-optimised def TU.
// Identity: reached through the 5-byte `jmp` thunk at 0x004E2C4D in slot 19 of vtable 0x012418C4 = CFishingDef
//   (rtti-vtables.tsv); the manifest label "CThingBuildingDef::operator=" is propagated over every def assignment.
// Callees: CDefClassBase::operator= @ 0x00431F10, vector<long>::operator= @ 0x00454886.
// Layout (engine/CFishingDef.h, PDB minus 4 after the vector): FishTypes +0x28, FishingSpotProximity +0x34, FishingSpotMinDepth +0x38, FishingSpotMinHeightDiff +0x3c, FishingSpotMaxHeightDiff +0x40, PointsPerFishingLevel +0x44, SlowFishingSpeed +0x48, NormalFishingSpeed +0x4c, FastFishingSpeed +0x50, FastestFishingSpeed +0x54, FishWeightRainMult +0x58, FishWeightSnowMult +0x5c, FishWeightMistMult +0x60, BaseWaitPeriodSeconds +0x64, MaxRandomWaitPeriodSeconds +0x68, HookWindowSeconds +0x6c, BaseHookPresses +0x70, MaxRandomHookPresses +0x74, StrainIncreasePerPress +0x78.
// Shape: the implicit memberwise assignment written out (the def module out-lines it); the generated engine header
//   flattens the base (no base operator= to call), so the classes are modelled locally like the landed 0x00463BA2.
// Retail vector<long> is 12 bytes: every member after FishTypes sits 4 below the PDB.
#pragma optimize("s", on)

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
struct CLongVector {                       // retail std::vector<long> (retail 0xc bytes)
    unsigned long m_words[3];
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

// def vtable 0x012418C4: slot 0 0x004E0DCF (implicit inline dtor); real overrides from the def vtable scan
class CFishingDef : public CDefClassBase { public:
    CLongVector              FishTypes;                   // +0x28
    float                    FishingSpotProximity;        // +0x34
    float                    FishingSpotMinDepth;         // +0x38
    float                    FishingSpotMinHeightDiff;    // +0x3c
    float                    FishingSpotMaxHeightDiff;    // +0x40
    long                     PointsPerFishingLevel;       // +0x44
    float                    SlowFishingSpeed;            // +0x48
    float                    NormalFishingSpeed;          // +0x4c
    float                    FastFishingSpeed;            // +0x50
    float                    FastestFishingSpeed;         // +0x54
    float                    FishWeightRainMult;          // +0x58
    float                    FishWeightSnowMult;          // +0x5c
    float                    FishWeightMistMult;          // +0x60
    float                    BaseWaitPeriodSeconds;       // +0x64
    float                    MaxRandomWaitPeriodSeconds;  // +0x68
    float                    HookWindowSeconds;           // +0x6c
    long                     BaseHookPresses;             // +0x70
    long                     MaxRandomHookPresses;        // +0x74
    float                    StrainIncreasePerPress;      // +0x78
    CFishingDef& operator=(const CFishingDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x007B1F10
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x004E2C4D
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004DE912
};

CFishingDef& CFishingDef::operator=(const CFishingDef& other)
{
    CDefClassBase::operator=(other);
    FishTypes = other.FishTypes;
    FishingSpotProximity = other.FishingSpotProximity;
    FishingSpotMinDepth = other.FishingSpotMinDepth;
    FishingSpotMinHeightDiff = other.FishingSpotMinHeightDiff;
    FishingSpotMaxHeightDiff = other.FishingSpotMaxHeightDiff;
    PointsPerFishingLevel = other.PointsPerFishingLevel;
    SlowFishingSpeed = other.SlowFishingSpeed;
    NormalFishingSpeed = other.NormalFishingSpeed;
    FastFishingSpeed = other.FastFishingSpeed;
    FastestFishingSpeed = other.FastestFishingSpeed;
    FishWeightRainMult = other.FishWeightRainMult;
    FishWeightSnowMult = other.FishWeightSnowMult;
    FishWeightMistMult = other.FishWeightMistMult;
    BaseWaitPeriodSeconds = other.BaseWaitPeriodSeconds;
    MaxRandomWaitPeriodSeconds = other.MaxRandomWaitPeriodSeconds;
    HookWindowSeconds = other.HookWindowSeconds;
    BaseHookPresses = other.BaseHookPresses;
    MaxRandomHookPresses = other.MaxRandomHookPresses;
    StrainIncreasePerPress = other.StrainIncreasePerPress;
    return *this;
}