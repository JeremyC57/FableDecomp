// CExplosiveTrailDef::operator=(const CExplosiveTrailDef& other) -- compiler-generated copy assignment: CSubDefClassBase::operator=, then TrapDefIndex, SecondsBetweenTraps, DistanceBetweenTraps, SecondsForTrapPersistance, SecondsTrailLifetime, DistanceForLateralDeviation, DegreesMaxTurnTowardsTargetPerTrap, FractionOfLifetimeToTurnTowardsTarget, SecondsIcicleAttackTargetPositionPredictionTime memberwise
// Leaf: ??4CExplosiveTrailDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004542D4, 75 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CExplosiveTrailDef::operator=(const CExplosiveTrailDef&) -- proven; evidence: its only caller is the landed CExplosiveTrailDef::Copy 0x004542CF
//   (a `jmp` to it: `*this = *static_cast<const CExplosiveTrailDef*>(other)`); RTTI of vtable 0x01232B54 gives CExplosiveTrailDef : CSubDefClassBase : CDefClassBase : CDefPointeeBase; the
//   copied offsets and widths are Ego_r.pdb CExplosiveTrailDef's nine members (retail offset = Ego_r offset); FableWin.pdb name set holds
//   ??4CExplosiveTrailDef@@QAEAAV0@ABV0@@Z. Family G059 (seven def copy assignments of nine members; catalogue "CThingBuildingDef::operator=" is propagated).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10 (bound under that name), reached through the inline implicit CSubDefClassBase::operator=.
// Shape: implicit operator= copies each scalar member with integer moves (no x87 for the floats); `return *this` in eax.
#pragma optimize("s", on)
class CDefClassBase {                                      // retail 0x25 bytes (+ tail padding)
public:
    virtual ~CDefClassBase();
    CDefClassBase& operator=(const CDefClassBase& other);  // 0x00431F10
    unsigned char _body[0x25 - 4];
};
class CSubDefClassBase : public CDefClassBase {           // no members: its implicit operator= inlines to the base call
};
class CExplosiveTrailDef : public CSubDefClassBase {
public:
    virtual void Copy(const CDefClassBase* other);
    long TrapDefIndex;                       // +0x28
    float SecondsBetweenTraps;               // +0x2c
    float DistanceBetweenTraps;              // +0x30
    float SecondsForTrapPersistance;         // +0x34
    float SecondsTrailLifetime;              // +0x38
    float DistanceForLateralDeviation;       // +0x3c
    float DegreesMaxTurnTowardsTargetPerTrap; // +0x40
    float FractionOfLifetimeToTurnTowardsTarget; // +0x44
    float SecondsIcicleAttackTargetPositionPredictionTime; // +0x48
};
void CExplosiveTrailDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CExplosiveTrailDef*>(other);
}