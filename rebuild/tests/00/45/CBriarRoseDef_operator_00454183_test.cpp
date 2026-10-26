// behaviour test for 0x00454183 CBriarRoseDef::operator=: the base part is assigned first, every member is copied, and the result is *this
#include <cstdio>
static int g_base = 0;
struct Base { long PDefManager; long GlobalIndex; Base& operator=(const Base& o) { ++g_base; PDefManager = o.PDefManager; GlobalIndex = o.GlobalIndex; return *this; } };
struct Def : Base {
    float SecondsToTeleport;
    float SecondsToBeIntangible;
    float SecondsToChargeDrainLife;
    long GuranteedImpostersHitBeforeAllowedToHitBriar;
    long NumberOfHitsToKillImposters;
    long EnvironmentThemeForIntangibleMode;
    float SecondsForIntangibleEnvironmentThemeTransitionIn;
    float SecondsForIntangibleEnvironmentThemeTransitionOut;
    float SecondsBetweenWillUse;
    float SecondsBetweenTeleportAway;
    float DistanceForcePushThreshold;
    Def& Assign(const Def& o)                              // the function under test
    {
        Base::operator=(o);
        SecondsToTeleport = o.SecondsToTeleport;
        SecondsToBeIntangible = o.SecondsToBeIntangible;
        SecondsToChargeDrainLife = o.SecondsToChargeDrainLife;
        GuranteedImpostersHitBeforeAllowedToHitBriar = o.GuranteedImpostersHitBeforeAllowedToHitBriar;
        NumberOfHitsToKillImposters = o.NumberOfHitsToKillImposters;
        EnvironmentThemeForIntangibleMode = o.EnvironmentThemeForIntangibleMode;
        SecondsForIntangibleEnvironmentThemeTransitionIn = o.SecondsForIntangibleEnvironmentThemeTransitionIn;
        SecondsForIntangibleEnvironmentThemeTransitionOut = o.SecondsForIntangibleEnvironmentThemeTransitionOut;
        SecondsBetweenWillUse = o.SecondsBetweenWillUse;
        SecondsBetweenTeleportAway = o.SecondsBetweenTeleportAway;
        DistanceForcePushThreshold = o.DistanceForcePushThreshold;
        return *this;
    }
};
int main()
{
    int fail = 0;
    Def s, d;
    s.PDefManager = 11; s.GlobalIndex = 12; d.PDefManager = 0; d.GlobalIndex = 0;
    s.SecondsToTeleport = 1.25f; d.SecondsToTeleport = 0.0f;
    s.SecondsToBeIntangible = 2.25f; d.SecondsToBeIntangible = 0.0f;
    s.SecondsToChargeDrainLife = 3.25f; d.SecondsToChargeDrainLife = 0.0f;
    s.GuranteedImpostersHitBeforeAllowedToHitBriar = 103L; d.GuranteedImpostersHitBeforeAllowedToHitBriar = 0L;
    s.NumberOfHitsToKillImposters = 104L; d.NumberOfHitsToKillImposters = 0L;
    s.EnvironmentThemeForIntangibleMode = 105L; d.EnvironmentThemeForIntangibleMode = 0L;
    s.SecondsForIntangibleEnvironmentThemeTransitionIn = 7.25f; d.SecondsForIntangibleEnvironmentThemeTransitionIn = 0.0f;
    s.SecondsForIntangibleEnvironmentThemeTransitionOut = 8.25f; d.SecondsForIntangibleEnvironmentThemeTransitionOut = 0.0f;
    s.SecondsBetweenWillUse = 9.25f; d.SecondsBetweenWillUse = 0.0f;
    s.SecondsBetweenTeleportAway = 10.25f; d.SecondsBetweenTeleportAway = 0.0f;
    s.DistanceForcePushThreshold = 11.25f; d.DistanceForcePushThreshold = 0.0f;
    g_base = 0;
    if (&d.Assign(s) != &d) fail |= 1;
    if (g_base != 1 || d.PDefManager != 11 || d.GlobalIndex != 12) fail |= 2;
    if (d.SecondsToTeleport != s.SecondsToTeleport) fail |= 4;
    if (d.SecondsToBeIntangible != s.SecondsToBeIntangible) fail |= 4;
    if (d.SecondsToChargeDrainLife != s.SecondsToChargeDrainLife) fail |= 4;
    if (d.GuranteedImpostersHitBeforeAllowedToHitBriar != s.GuranteedImpostersHitBeforeAllowedToHitBriar) fail |= 4;
    if (d.NumberOfHitsToKillImposters != s.NumberOfHitsToKillImposters) fail |= 4;
    if (d.EnvironmentThemeForIntangibleMode != s.EnvironmentThemeForIntangibleMode) fail |= 4;
    if (d.SecondsForIntangibleEnvironmentThemeTransitionIn != s.SecondsForIntangibleEnvironmentThemeTransitionIn) fail |= 4;
    if (d.SecondsForIntangibleEnvironmentThemeTransitionOut != s.SecondsForIntangibleEnvironmentThemeTransitionOut) fail |= 4;
    if (d.SecondsBetweenWillUse != s.SecondsBetweenWillUse) fail |= 4;
    if (d.SecondsBetweenTeleportAway != s.SecondsBetweenTeleportAway) fail |= 4;
    if (d.DistanceForcePushThreshold != s.DistanceForcePushThreshold) fail |= 4;
    if (fail) { std::printf("00454183_TEST FAIL %d\n", fail); return 1; }
    std::printf("00454183_TEST PASS\n");
    return 0;
}