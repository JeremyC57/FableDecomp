#include <cstdio>
#include <cstring>
// Behaviour test: runs the reconstructed operator= on a filled source and checks every member and the out-of-line container/base calls.
// CHeroAbilityDef::operator=(const CHeroAbilityDef&) -- memberwise copy: CParentDefClassBase part, the ability/stat/icon indices and offset, the text ids, the three per-level vectors, the priority/morality/aggressive fields and the dummy object; returns *this

// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045CB1C, length 177 bytes.
// Pragma:  -- the def module is size-optimised (every copy goes through eax/al).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<long>::operator= @ 0x00454886 (three times).
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// CHeroAbilityDef's vtable 0x01233974 is the 5-byte `jmp 0x0045CB1C` right before this body. PDB members
// +0x40.. sit at retail +0x3c.. (retail base 0x3c), and each retail vector is 12 bytes instead of 16.
// Shape: memberwise assignment in declaration order; the vectors through their out-of-line operator=,
// the C2DVector icon offset as its two floats (dword moves), the rest as dword/byte moves.
#pragma pack(push, 1)
struct CParentDefClassBase {
    virtual void v00() {}
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

struct C2DVector { float X; float Y; };

struct CHeroAbilityDef : CParentDefClassBase {  // vtable 0x01233974
    long        Ability;                     // +0x3c EHeroAbility
    long        Stat;                        // +0x40 EHeroExperienceStatCategory
    long        IconGraphicIndex;            // +0x44
    long        IconEffectIndex;             // +0x48
    long        IconActiveEffectIndex;       // +0x4c
    C2DVector   IconEffectOffset;            // +0x50
    long        Name;                        // +0x58
    long        Description;                 // +0x5c
    CLongVector LevDescription;              // +0x60
    long        MaxedOutDescription;         // +0x6c
    CLongVector StaminaCost;                 // +0x70
    long        StatExperienceGainedOnUse;   // +0x7c
    CLongVector ExperienceCostsToUpgrade;    // +0x80
    long        ABXYPriority;                // +0x8c
    float       MoralityCostFactor;          // +0x90
    bool        Aggressive;                  // +0x94
    long        DummyObject;                 // +0x98 CDefIndex
    CHeroAbilityDef& operator=(const CHeroAbilityDef& other);
};

CHeroAbilityDef& CHeroAbilityDef::operator=(const CHeroAbilityDef& other)
{
    CParentDefClassBase::operator=(other);
    Ability = other.Ability;
    Stat = other.Stat;
    IconGraphicIndex = other.IconGraphicIndex;
    IconEffectIndex = other.IconEffectIndex;
    IconActiveEffectIndex = other.IconActiveEffectIndex;
    IconEffectOffset = other.IconEffectOffset;
    Name = other.Name;
    Description = other.Description;
    LevDescription = other.LevDescription;
    MaxedOutDescription = other.MaxedOutDescription;
    StaminaCost = other.StaminaCost;
    StatExperienceGainedOnUse = other.StatExperienceGainedOnUse;
    ExperienceCostsToUpgrade = other.ExperienceCostsToUpgrade;
    ABXYPriority = other.ABXYPriority;
    MoralityCostFactor = other.MoralityCostFactor;
    Aggressive = other.Aggressive;
    DummyObject = other.DummyObject;
    return *this;
}

static int g_baseCalls = 0, g_vecCalls = 0;
CParentDefClassBase& CParentDefClassBase::operator=(const CParentDefClassBase& o)
{ ++g_baseCalls; for (int i = 0; i < 0x35; ++i) _defClassBase[i] = o._defClassBase[i]; return *this; }
CLongVector& CLongVector::operator=(const CLongVector& o) { ++g_vecCalls; m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; return *this; }

int main()
{
    static CHeroAbilityDef a, b;
    std::memset(a._defClassBase, 0, sizeof(a._defClassBase));
    std::memset(b._defClassBase, 0, sizeof(b._defClassBase));
    b._defClassBase[0x21] = 0x5a;
    b.Ability = 5; b.Stat = 2; b.IconGraphicIndex = 11; b.IconActiveEffectIndex = 13; b.IconEffectOffset.X = 0.5f; b.IconEffectOffset.Y = -2.0f; b.Description = 99; b.MaxedOutDescription = 42; b.StaminaCost.m_first = (long*)&b; b.ABXYPriority = 3; b.MoralityCostFactor = 1.25f; b.Aggressive = true; b.DummyObject = 77;
    CHeroAbilityDef& r = (a = b);
    bool ok = (&r == &a) && g_baseCalls == 1 && a._defClassBase[0x21] == 0x5a;
    ok = ok && g_vecCalls == 3 && a.Ability == 5 && a.Stat == 2 && a.IconGraphicIndex == 11 && a.IconActiveEffectIndex == 13 && a.IconEffectOffset.X == 0.5f && a.IconEffectOffset.Y == -2.0f && a.Description == 99 && a.MaxedOutDescription == 42 && a.StaminaCost.m_first == (long*)&b && a.ABXYPriority == 3 && a.MoralityCostFactor == 1.25f && a.Aggressive && a.DummyObject == 77;
    if (!ok) { std::printf("0045cb1c_TEST FAIL\n"); return 1; }
    std::printf("0045cb1c_TEST PASS\n");
    return 0;
}
