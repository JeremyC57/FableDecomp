// behaviour test for 0x004E46CD CJackOfBladesBattleDef::operator=: base part then 17 scalars copied, *this returned
#include <cstdio>
static int g_baseCopies = 0;
struct CParentDefClassBase {
    unsigned char body[0x35];
    CParentDefClassBase& operator=(const CParentDefClassBase& o) { ++g_baseCopies; for (int i = 0; i < 0x35; ++i) body[i] = o.body[i]; return *this; }
};
struct CJackOfBladesBattleDef : CParentDefClassBase {
    float IdealShootDistance; long NumGetHitsBeforeRush; float TimeBetweenShots, ScreamInAirTime, TimeBetweenPillars,
        BattleFloatHeight, BattleScale, ScaleUpTime, ScaleDownTime, DeathScreamTime, DeathHeight, BurnDamage, FlashDuration,
        FlashBuildUpTime, RushTriggerDistance, RushToDistance, RushAwayDistance;
    CJackOfBladesBattleDef& operator=(const CJackOfBladesBattleDef& o)
    {
        CParentDefClassBase::operator=(o);
        IdealShootDistance = o.IdealShootDistance; NumGetHitsBeforeRush = o.NumGetHitsBeforeRush; TimeBetweenShots = o.TimeBetweenShots;
        ScreamInAirTime = o.ScreamInAirTime; TimeBetweenPillars = o.TimeBetweenPillars; BattleFloatHeight = o.BattleFloatHeight;
        BattleScale = o.BattleScale; ScaleUpTime = o.ScaleUpTime; ScaleDownTime = o.ScaleDownTime; DeathScreamTime = o.DeathScreamTime;
        DeathHeight = o.DeathHeight; BurnDamage = o.BurnDamage; FlashDuration = o.FlashDuration; FlashBuildUpTime = o.FlashBuildUpTime;
        RushTriggerDistance = o.RushTriggerDistance; RushToDistance = o.RushToDistance; RushAwayDistance = o.RushAwayDistance;
        return *this;
    }
};
int main()
{
    CJackOfBladesBattleDef a, b;
    for (int i = 0; i < 0x35; ++i) { a.body[i] = (unsigned char)(i + 1); b.body[i] = 0; }
    a.IdealShootDistance = 1.5f; a.NumGetHitsBeforeRush = 3; a.TimeBetweenShots = 2.0f; a.ScreamInAirTime = 3.0f; a.TimeBetweenPillars = 4.0f;
    a.BattleFloatHeight = 5.0f; a.BattleScale = 6.0f; a.ScaleUpTime = 7.0f; a.ScaleDownTime = 8.0f; a.DeathScreamTime = 9.0f;
    a.DeathHeight = 10.0f; a.BurnDamage = 11.0f; a.FlashDuration = 12.0f; a.FlashBuildUpTime = 13.0f; a.RushTriggerDistance = 14.0f;
    a.RushToDistance = 15.0f; a.RushAwayDistance = 16.0f;
    CJackOfBladesBattleDef* r = &(b = a);
    if (r != &b || g_baseCopies != 1 || b.body[0] != 1 || b.IdealShootDistance != 1.5f || b.NumGetHitsBeforeRush != 3 ||
        b.FlashDuration != 12.0f || b.RushAwayDistance != 16.0f) { std::printf("004e46cd_TEST FAIL\n"); return 1; }
    std::printf("004e46cd_TEST PASS\n");
    return 0;
}