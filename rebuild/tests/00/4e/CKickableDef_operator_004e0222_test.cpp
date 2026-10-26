// behaviour test for 0x004E0222 CKickableDef::operator=: the base part is assigned first, every member is copied, and the result is *this
#include <cstdio>
static int g_base = 0;
struct Base { long PDefManager; long GlobalIndex; Base& operator=(const Base& o) { ++g_base; PDefManager = o.PDefManager; GlobalIndex = o.GlobalIndex; return *this; } };
struct Def : Base {
    float BaseDistanceToTravel;
    float StrengthScaleRange;
    float SpeedScaleRange;
    float BaseSpeedScaleRange;
    float RandomBase;
    float RandomRange;
    long ShotToAttach;
    long OpinionDeedType;
    long OpinionDeedFaction;
    long OpinionDeedTypeToEnemies;
    bool IgnoreNavigationTest;
    Def& Assign(const Def& o)                              // the function under test
    {
        Base::operator=(o);
        BaseDistanceToTravel = o.BaseDistanceToTravel;
        StrengthScaleRange = o.StrengthScaleRange;
        SpeedScaleRange = o.SpeedScaleRange;
        BaseSpeedScaleRange = o.BaseSpeedScaleRange;
        RandomBase = o.RandomBase;
        RandomRange = o.RandomRange;
        ShotToAttach = o.ShotToAttach;
        OpinionDeedType = o.OpinionDeedType;
        OpinionDeedFaction = o.OpinionDeedFaction;
        OpinionDeedTypeToEnemies = o.OpinionDeedTypeToEnemies;
        IgnoreNavigationTest = o.IgnoreNavigationTest;
        return *this;
    }
};
int main()
{
    int fail = 0;
    Def s, d;
    s.PDefManager = 11; s.GlobalIndex = 12; d.PDefManager = 0; d.GlobalIndex = 0;
    s.BaseDistanceToTravel = 1.25f; d.BaseDistanceToTravel = 0.0f;
    s.StrengthScaleRange = 2.25f; d.StrengthScaleRange = 0.0f;
    s.SpeedScaleRange = 3.25f; d.SpeedScaleRange = 0.0f;
    s.BaseSpeedScaleRange = 4.25f; d.BaseSpeedScaleRange = 0.0f;
    s.RandomBase = 5.25f; d.RandomBase = 0.0f;
    s.RandomRange = 6.25f; d.RandomRange = 0.0f;
    s.ShotToAttach = 106L; d.ShotToAttach = 0L;
    s.OpinionDeedType = 107L; d.OpinionDeedType = 0L;
    s.OpinionDeedFaction = 108L; d.OpinionDeedFaction = 0L;
    s.OpinionDeedTypeToEnemies = 109L; d.OpinionDeedTypeToEnemies = 0L;
    s.IgnoreNavigationTest = true; d.IgnoreNavigationTest = false;
    g_base = 0;
    if (&d.Assign(s) != &d) fail |= 1;
    if (g_base != 1 || d.PDefManager != 11 || d.GlobalIndex != 12) fail |= 2;
    if (d.BaseDistanceToTravel != s.BaseDistanceToTravel) fail |= 4;
    if (d.StrengthScaleRange != s.StrengthScaleRange) fail |= 4;
    if (d.SpeedScaleRange != s.SpeedScaleRange) fail |= 4;
    if (d.BaseSpeedScaleRange != s.BaseSpeedScaleRange) fail |= 4;
    if (d.RandomBase != s.RandomBase) fail |= 4;
    if (d.RandomRange != s.RandomRange) fail |= 4;
    if (d.ShotToAttach != s.ShotToAttach) fail |= 4;
    if (d.OpinionDeedType != s.OpinionDeedType) fail |= 4;
    if (d.OpinionDeedFaction != s.OpinionDeedFaction) fail |= 4;
    if (d.OpinionDeedTypeToEnemies != s.OpinionDeedTypeToEnemies) fail |= 4;
    if (d.IgnoreNavigationTest != s.IgnoreNavigationTest) fail |= 4;
    if (fail) { std::printf("004e0222_TEST FAIL %d\n", fail); return 1; }
    std::printf("004e0222_TEST PASS\n");
    return 0;
}