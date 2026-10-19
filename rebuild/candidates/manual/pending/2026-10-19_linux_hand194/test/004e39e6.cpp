// Behaviour test for CJackOfBladesBattleDef::Transfer (0x004E39E6): every member is handed to the right typed
// transfer, in declaration order (a loading context writes a running sequence number into each member).
#include <cstdio>
struct CPersistContext {
    int seq; int kinds[32]; void* addrs[32];
    void TransferLong(long* value) { kinds[seq] = 1; addrs[seq] = value; *value = 100 + seq; ++seq; }
    void TransferFloat(float* value) { kinds[seq] = 2; addrs[seq] = value; *value = (float)seq; ++seq; }
};
struct CJackOfBladesBattleDef {
    virtual void v00() {}
    unsigned char _defBase[0x38];
    float IdealShootDistance; long NumGetHitsBeforeRush; float TimeBetweenShots; float ScreamInAirTime;
    float TimeBetweenPillars; float BattleFloatHeight; float BattleScale; float ScaleUpTime; float ScaleDownTime;
    float DeathScreamTime; float DeathHeight; float BurnDamage; float FlashDuration; float FlashBuildUpTime;
    float RushTriggerDistance; float RushToDistance; float RushAwayDistance;
    virtual void Transfer(CPersistContext& context);
};
void CJackOfBladesBattleDef::Transfer(CPersistContext& context)
{
    context.TransferFloat(&IdealShootDistance);
    context.TransferLong(&NumGetHitsBeforeRush);
    context.TransferFloat(&TimeBetweenShots);
    context.TransferFloat(&ScreamInAirTime);
    context.TransferFloat(&TimeBetweenPillars);
    context.TransferFloat(&BattleFloatHeight);
    context.TransferFloat(&BattleScale);
    context.TransferFloat(&ScaleUpTime);
    context.TransferFloat(&ScaleDownTime);
    context.TransferFloat(&DeathScreamTime);
    context.TransferFloat(&DeathHeight);
    context.TransferFloat(&BurnDamage);
    context.TransferFloat(&FlashDuration);
    context.TransferFloat(&FlashBuildUpTime);
    context.TransferFloat(&RushTriggerDistance);
    context.TransferFloat(&RushToDistance);
    context.TransferFloat(&RushAwayDistance);
}
int main()
{
    CJackOfBladesBattleDef d; CPersistContext c; c.seq = 0;
    d.Transfer(c);
    int bad = c.seq != 17;
    if (c.kinds[0] != 2 || c.kinds[1] != 1 || d.NumGetHitsBeforeRush != 101 || d.IdealShootDistance != 0.0f) bad = 1;
    for (int i = 2; i < 17; ++i) if (c.kinds[i] != 2) bad = 1;
    if (d.TimeBetweenShots != 2.0f || d.RushAwayDistance != 16.0f || d.BurnDamage != 11.0f) bad = 1;
    if (c.addrs[16] != &d.RushAwayDistance || c.addrs[1] != &d.NumGetHitsBeforeRush) bad = 1;
    if (bad) { std::printf("004e39e6_TEST FAIL\n"); return 1; }
    std::printf("004e39e6_TEST PASS\n");
    return 0;
}
