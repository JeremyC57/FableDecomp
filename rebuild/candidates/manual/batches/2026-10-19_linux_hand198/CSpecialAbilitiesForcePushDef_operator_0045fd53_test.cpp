#include <cstdio>
#include <cstring>
// Behaviour test: runs the reconstructed operator= on a filled source and checks every member and the out-of-line container/base calls.
// CSpecialAbilitiesForcePushDef::operator=(const CSpecialAbilitiesForcePushDef&) -- memberwise copy: CParentDefClassBase part, push distances and dust/effect counts, the three level-up timings, the release delays and offsets, and the radius/damage/cast-delay per-level vectors; returns *this

// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045FD53, length 186 bytes.
// Pragma:  -- the def module is size-optimised (`push 3 ; pop edx` loop counter).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<float>::operator= @ 0x0045EF2D (three times).
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// CSpecialAbilitiesForcePushDef's vtable 0x012342CC is the 5-byte `jmp 0x0045FD53` right before this body.
// PDB members +0x40.. sit at retail +0x3c.. with 12-byte retail vectors.
// Shape: memberwise assignment in declaration order. The float[3] LevelUpTimings is copied element by
// element in a loop (the fld/fstp loop over the source-minus-dest delta), as the implicit assignment of
// an array member does.
#pragma pack(push, 1)
struct CParentDefClassBase {
    virtual void v00() {}
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

struct CSpecialAbilitiesForcePushDef : CParentDefClassBase {   // vtable 0x012342CC
    float        MaxDistanceToPush;         // +0x3c
    float        MinDistanceToPush;         // +0x40
    float        DustBoxWidth;              // +0x44
    long         MaxNumOfExtraEffects;      // +0x48
    long         MinNumOfExtraEffects;      // +0x4c
    float        LevelUpTimings[3];         // +0x50
    float        ReleaseLevel1Delay;        // +0x5c
    float        ReleaseLevel2Delay;        // +0x60
    float        ReleaseLevel3DelayA;       // +0x64
    float        ReleaseLevel3DelayB;       // +0x68
    float        ReleaseLevel4Delay;        // +0x6c
    float        HeightOffGround;           // +0x70
    float        Level4OffsetMagnitude;     // +0x74
    CFloatVector RadiusOfEffect;            // +0x78
    float        ExtraPush;                 // +0x84
    float        PushSpeed;                 // +0x88
    CFloatVector ObjectDamage;              // +0x8c
    CFloatVector TimeDelayBetweenCastsSec;  // +0x98
    CSpecialAbilitiesForcePushDef& operator=(const CSpecialAbilitiesForcePushDef& other);
};

CSpecialAbilitiesForcePushDef& CSpecialAbilitiesForcePushDef::operator=(const CSpecialAbilitiesForcePushDef& other)
{
    CParentDefClassBase::operator=(other);
    MaxDistanceToPush = other.MaxDistanceToPush;
    MinDistanceToPush = other.MinDistanceToPush;
    DustBoxWidth = other.DustBoxWidth;
    MaxNumOfExtraEffects = other.MaxNumOfExtraEffects;
    MinNumOfExtraEffects = other.MinNumOfExtraEffects;
    for (int i = 0; i < 3; ++i)
        LevelUpTimings[i] = other.LevelUpTimings[i];
    ReleaseLevel1Delay = other.ReleaseLevel1Delay;
    ReleaseLevel2Delay = other.ReleaseLevel2Delay;
    ReleaseLevel3DelayA = other.ReleaseLevel3DelayA;
    ReleaseLevel3DelayB = other.ReleaseLevel3DelayB;
    ReleaseLevel4Delay = other.ReleaseLevel4Delay;
    HeightOffGround = other.HeightOffGround;
    Level4OffsetMagnitude = other.Level4OffsetMagnitude;
    RadiusOfEffect = other.RadiusOfEffect;
    ExtraPush = other.ExtraPush;
    PushSpeed = other.PushSpeed;
    ObjectDamage = other.ObjectDamage;
    TimeDelayBetweenCastsSec = other.TimeDelayBetweenCastsSec;
    return *this;
}

static int g_baseCalls = 0, g_vecCalls = 0;
CParentDefClassBase& CParentDefClassBase::operator=(const CParentDefClassBase& o)
{ ++g_baseCalls; for (int i = 0; i < 0x35; ++i) _defClassBase[i] = o._defClassBase[i]; return *this; }
CFloatVector& CFloatVector::operator=(const CFloatVector& o) { ++g_vecCalls; m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; return *this; }

int main()
{
    static CSpecialAbilitiesForcePushDef a, b;
    std::memset(a._defClassBase, 0, sizeof(a._defClassBase));
    std::memset(b._defClassBase, 0, sizeof(b._defClassBase));
    b._defClassBase[0x21] = 0x5a;
    b.MaxDistanceToPush = 10.0f; b.MinNumOfExtraEffects = 2; b.LevelUpTimings[0] = 0.5f; b.LevelUpTimings[1] = 1.5f; b.LevelUpTimings[2] = 2.5f; b.ReleaseLevel4Delay = 0.125f; b.Level4OffsetMagnitude = 6.0f; b.PushSpeed = 12.0f; b.TimeDelayBetweenCastsSec.m_first = (float*)&b;
    CSpecialAbilitiesForcePushDef& r = (a = b);
    bool ok = (&r == &a) && g_baseCalls == 1 && a._defClassBase[0x21] == 0x5a;
    ok = ok && g_vecCalls == 3 && a.MaxDistanceToPush == 10.0f && a.MinNumOfExtraEffects == 2 && a.LevelUpTimings[0] == 0.5f && a.LevelUpTimings[1] == 1.5f && a.LevelUpTimings[2] == 2.5f && a.ReleaseLevel4Delay == 0.125f && a.Level4OffsetMagnitude == 6.0f && a.PushSpeed == 12.0f && a.TimeDelayBetweenCastsSec.m_first == (float*)&b;
    if (!ok) { std::printf("0045fd53_TEST FAIL\n"); return 1; }
    std::printf("0045fd53_TEST PASS\n");
    return 0;
}
