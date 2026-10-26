// behaviour test for 0x0045F58A CSpecialAbilitiesAssassinRushDef::operator=
#include <cstdio>
static int g_vecCopies = 0;
class CParentDefClassBase { public:
    int baseTag;
    CParentDefClassBase& operator=(const CParentDefClassBase& o) { baseTag = o.baseTag; return *this; }
};
struct CFloatVector {
    float v;
    CFloatVector& operator=(const CFloatVector& o) { v = o.v; ++g_vecCopies; return *this; }
};
class CSpecialAbilitiesAssassinRushDef : public CParentDefClassBase { public:
    float DistanceAwayFromtargetToAssassinRushTo, SafeCollideRadiusPercentage, DistFromFloor, AssassinStartSeconds,
          AssassinEndSeconds, AssassinThreshold, AssassinLength, AssassinIntensity, AssassinFade, AssassinRange,
          AssassinTwist, RunSpeed, TrailDelaySecs, SlowTimeLength;
    long AssassinAlpha, SlowTimeMultiplier;
    float RushDelay;
    CFloatVector DashDistance;
    CSpecialAbilitiesAssassinRushDef& operator=(const CSpecialAbilitiesAssassinRushDef& other)
    {
        CParentDefClassBase::operator=(other);
        DistanceAwayFromtargetToAssassinRushTo = other.DistanceAwayFromtargetToAssassinRushTo;
        SafeCollideRadiusPercentage = other.SafeCollideRadiusPercentage;
        DistFromFloor = other.DistFromFloor;
        AssassinStartSeconds = other.AssassinStartSeconds;
        AssassinEndSeconds = other.AssassinEndSeconds;
        AssassinThreshold = other.AssassinThreshold;
        AssassinLength = other.AssassinLength;
        AssassinIntensity = other.AssassinIntensity;
        AssassinFade = other.AssassinFade;
        AssassinRange = other.AssassinRange;
        AssassinTwist = other.AssassinTwist;
        RunSpeed = other.RunSpeed;
        TrailDelaySecs = other.TrailDelaySecs;
        SlowTimeLength = other.SlowTimeLength;
        AssassinAlpha = other.AssassinAlpha;
        SlowTimeMultiplier = other.SlowTimeMultiplier;
        RushDelay = other.RushDelay;
        DashDistance = other.DashDistance;
        return *this;
    }
};
int main()
{
    CSpecialAbilitiesAssassinRushDef a, b;
    b.baseTag = 3; b.RunSpeed = 6.5f; b.AssassinAlpha = 200; b.RushDelay = 0.25f; b.DashDistance.v = 9.0f;
    a.baseTag = 0; a.RunSpeed = 0; a.AssassinAlpha = 0; a.RushDelay = 0; a.DashDistance.v = 0;
    CSpecialAbilitiesAssassinRushDef* r = &(a = b);
    if (r != &a || a.baseTag != 3 || a.RunSpeed != 6.5f || a.AssassinAlpha != 200 || a.RushDelay != 0.25f
        || a.DashDistance.v != 9.0f || g_vecCopies != 1)
    { std::printf("0045f58a_TEST FAIL\n"); return 1; }
    std::printf("0045f58a_TEST PASS\n");
    return 0;
}