// Behaviour test for 0x00453178 CExplosiveTrailDef::Transfer
#include <cstdio>
struct CPersistContext {
    int m_calls; float m_loadValue;
    void TransferLong(long* value) { ++m_calls; *value = 7; }
    void TransferFloat(float* value) { ++m_calls; *value = m_loadValue; }
};
struct CExplosiveTrailDef {
    long TrapDefIndex; float SecondsBetweenTraps, DistanceBetweenTraps, SecondsForTrapPersistance, SecondsTrailLifetime,
        DistanceForLateralDeviation, DegreesMaxTurnTowardsTargetPerTrap, FractionOfLifetimeToTurnTowardsTarget,
        SecondsIcicleAttackTargetPositionPredictionTime;
    void Transfer(CPersistContext& context)
    {
        context.TransferLong(&TrapDefIndex);
        context.TransferFloat(&SecondsBetweenTraps); context.TransferFloat(&DistanceBetweenTraps);
        context.TransferFloat(&SecondsForTrapPersistance); context.TransferFloat(&SecondsTrailLifetime);
        context.TransferFloat(&DistanceForLateralDeviation); context.TransferFloat(&DegreesMaxTurnTowardsTargetPerTrap);
        context.TransferFloat(&FractionOfLifetimeToTurnTowardsTarget); context.TransferFloat(&SecondsIcicleAttackTargetPositionPredictionTime);
        if (FractionOfLifetimeToTurnTowardsTarget < 0.0f) FractionOfLifetimeToTurnTowardsTarget = 0.0f;
        else if (FractionOfLifetimeToTurnTowardsTarget > 1.0f) FractionOfLifetimeToTurnTowardsTarget = 1.0f;
    }
};
int main()
{
    CExplosiveTrailDef d; CPersistContext c; c.m_calls = 0;
    c.m_loadValue = 2.5f; d.Transfer(c);
    bool ok = c.m_calls == 9 && d.TrapDefIndex == 7 && d.FractionOfLifetimeToTurnTowardsTarget == 1.0f && d.SecondsIcicleAttackTargetPositionPredictionTime == 2.5f;
    c.m_loadValue = -0.5f; d.Transfer(c); ok = ok && d.FractionOfLifetimeToTurnTowardsTarget == 0.0f && d.SecondsTrailLifetime == -0.5f;
    c.m_loadValue = 0.25f; d.Transfer(c); ok = ok && d.FractionOfLifetimeToTurnTowardsTarget == 0.25f;
    if (!ok) { std::printf("00453178_TEST FAIL\n"); return 1; }
    std::printf("00453178_TEST PASS\n");
    return 0;
}
