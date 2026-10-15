// CExplosiveTrailDef::Transfer(CPersistContext& context) -- persist the trap definition index and the eight trail tuning floats, then clamp FractionOfLifetimeToTurnTowardsTarget into [0, 1]
// Retail: Fable.exe (TLC, VC7.1) @ 0x00453178, 152 bytes.
// Callees: the persist context's typed transfer helpers @ 0x00431102 (the long def index) and @ 0x00431061 (each float);
//   both manifest labels ("CEngineLightingManager::UpdateShadowScene") are propagated, so they are named by type.
//   Float pool: 0x0122DEDC = 0.0f, 0x0122DED8 = 1.0f.
// Layout: engine/CExplosiveTrailDef.h (TrapDefIndex +0x28 .. SecondsIcicleAttackTargetPositionPredictionTime +0x48); the
//   persist method is declared on a thin derived view because the generated header flattens the vptr into a void*.
// Shape: each member goes by address in declaration order; FractionOfLifetimeToTurnTowardsTarget's address is kept in
//   ebx for the clamp and the last member's address is formed in place (`add esi,48h`); the clamp is
//   `if (f < 0) f = 0; else if (f > 1) f = 1;` -- fldz / fld1 meet in one shared `fstp`, and the in-range path stores nothing
//   (`test ah,5 ; jp` is the `< 0` test, `test ah,41h ; jne` the `> 1` test).
// Pragma: `#pragma optimize("s", on)` -- a size TU: every member address goes through eax, and the clamp constants are
//   fldz / fld1 into one shared fstp (the speed build stores integer immediates).
#pragma optimize("s", on)
#include "engine/CExplosiveTrailDef.h"

struct CPersistContext {
    void TransferLong(long* value);              // 0x00431102
    void TransferFloat(float* value);            // 0x00431061
};

struct CExplosiveTrailDefPersist : CExplosiveTrailDef {
    void Transfer(CPersistContext& context);
};

void CExplosiveTrailDefPersist::Transfer(CPersistContext& context)
{
    context.TransferLong(&TrapDefIndex);
    context.TransferFloat(&SecondsBetweenTraps);
    context.TransferFloat(&DistanceBetweenTraps);
    context.TransferFloat(&SecondsForTrapPersistance);
    context.TransferFloat(&SecondsTrailLifetime);
    context.TransferFloat(&DistanceForLateralDeviation);
    context.TransferFloat(&DegreesMaxTurnTowardsTargetPerTrap);
    context.TransferFloat(&FractionOfLifetimeToTurnTowardsTarget);
    context.TransferFloat(&SecondsIcicleAttackTargetPositionPredictionTime);
    if (FractionOfLifetimeToTurnTowardsTarget < 0.0f)
        FractionOfLifetimeToTurnTowardsTarget = 0.0f;
    else if (FractionOfLifetimeToTurnTowardsTarget > 1.0f)
        FractionOfLifetimeToTurnTowardsTarget = 1.0f;
}
