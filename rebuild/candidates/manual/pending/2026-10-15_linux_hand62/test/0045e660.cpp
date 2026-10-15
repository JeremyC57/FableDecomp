#include <cstdio>

// COpinionPersonalityDef::operator=: copies the flag, the map (out-of-line assignment) and the
// tolerance, copies the owned trait table IN PLACE (both pointers unchanged), never assigns the base.
static int g_baseAssigns = 0, g_mapAssigns = 0;

struct CParentDefClassBase {
    virtual void v00() { }
    long m_marker;
    CParentDefClassBase& operator=(const CParentDefClassBase& other) { m_marker = other.m_marker; ++g_baseAssigns; return *this; }
};
struct COpinionPersonalityTraits {
    float BaseOffset, HeroStatScaling, VillageOpinionScaling, TimeScalingRunIn, TimeScalingRunOut;
    float PeakScalingIfPos, PeakScalingIfNeg, PersistScalingIfPos, PersistScalingIfNeg;
};
struct CPersonalityTraitTable {
    COpinionPersonalityTraits Attitude[5];
};
struct CAttitudeEnabledMap {
    long m_id;
    CAttitudeEnabledMap& operator=(const CAttitudeEnabledMap& other) { m_id = other.m_id; ++g_mapAssigns; return *this; }
};
struct COpinionPersonalityDef : CParentDefClassBase {
    CPersonalityTraitTable* PersonalityTraits;
    bool                    AttitudeEnabledDefaultForNotNone;
    CAttitudeEnabledMap     AttitudeEnabled;
    float                   ToleranceToBeingHit;
    COpinionPersonalityDef& operator=(const COpinionPersonalityDef& other);
};
COpinionPersonalityDef& COpinionPersonalityDef::operator=(const COpinionPersonalityDef& other)
{
    AttitudeEnabledDefaultForNotNone = other.AttitudeEnabledDefaultForNotNone;
    AttitudeEnabled = other.AttitudeEnabled;
    ToleranceToBeingHit = other.ToleranceToBeingHit;
    *PersonalityTraits = *other.PersonalityTraits;
    return *this;
}

int main()
{
    CPersonalityTraitTable ta, tb;
    for (int i = 0; i < 5; ++i) {
        ta.Attitude[i].BaseOffset = 1.0f + i;
        ta.Attitude[i].PersistScalingIfNeg = 10.0f + i;
        tb.Attitude[i].BaseOffset = 0.0f;
        tb.Attitude[i].PersistScalingIfNeg = 0.0f;
    }
    COpinionPersonalityDef a, b;
    a.m_marker = 7; b.m_marker = 1;
    a.PersonalityTraits = &ta; b.PersonalityTraits = &tb;
    a.AttitudeEnabledDefaultForNotNone = true; b.AttitudeEnabledDefaultForNotNone = false;
    a.AttitudeEnabled.m_id = 42; b.AttitudeEnabled.m_id = 0;
    a.ToleranceToBeingHit = 2.5f; b.ToleranceToBeingHit = 0.0f;
    COpinionPersonalityDef& r = (b = a);
    if (&r != &b || g_baseAssigns != 0 || b.m_marker != 1 || g_mapAssigns != 1 || b.AttitudeEnabled.m_id != 42
        || b.AttitudeEnabledDefaultForNotNone != true || b.ToleranceToBeingHit != 2.5f
        || b.PersonalityTraits != &tb || tb.Attitude[0].BaseOffset != 1.0f || tb.Attitude[4].PersistScalingIfNeg != 14.0f) {
        std::printf("0045e660_TEST FAIL\n");
        return 1;
    }
    std::printf("0045e660_TEST PASS\n");
    return 0;
}
