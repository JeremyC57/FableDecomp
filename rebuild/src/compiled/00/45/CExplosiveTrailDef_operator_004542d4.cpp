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
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
#include "engine/CSubDefClassBase.h"                    // vtable 0x01231D54
// def vtable 0x01232B54: slot 0 0x0045011B (implicit inline dtor); real overrides from the def vtable scan
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
    virtual void Transfer(CPersistContext& context);         // slot 18 0x00453178
    virtual unsigned long GetSizeofClass();                  // slot 20 0x0044C6AB
};
void CExplosiveTrailDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CExplosiveTrailDef*>(other);
}