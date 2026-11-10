// CCombatManager::IsCreatureInAttackRingSector(const CThingCreatureBase& attacker) const -- true when the attacker stands inside its assigned ring of the defender's combat wheel (between the inner and outer ring radius, each padded by the defender's radius) and within its prong's angular sector
// Grade: DIFFER(423v426) -- on shared headers (5 grades): two residues left. (1) frame 0x24 v 0x1c: retail shares one frame slot between the two find-key temporaries (key copies at +0x14 both times), ours gives the second key its own slot; (2) retail materialises the attacked info `add eax,18h` after the second _M_find and takes the wheel as `lea ebx,[eax+40h]`, ours folds to `lea ebx,[eax+58h]` (-3 bytes). Closed on the way: one-push _M_find call (STLP_TREE_CONSTFIND + opaque STLP_TREE_MFIND_DECL: the ring index reloaded after the second lookup), no ecx reload before the second GetRingRadius (same-TU helper bodies, GetRing expanded in GetRingRadius).
// Leaf: ?IsCreatureInAttackRingSector@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E94D0, 426 bytes, thiscall `ret 4`, bool in al.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_GETCOMBATANTINFO_INLINE
#define FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_WHEEL
#define FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PRONG
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#include <math.h>
#include "stlport/fable_containers.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCombatManager.h"
#ifdef _MSC_VER
#pragma intrinsic(sqrt, cos)
#endif

// Same-TU bodies of the combat-wheel helpers (CombatManager TU; landed separately at 0x006E8520 / 0x006E8850 / 0x006E8810 /
// 0x006E8460): retail calls them out of line but knows their register use (GetRingRadius is called twice without reloading ecx).
inline const CDefendingCombatantInfo::CCombatRing* CDefendingCombatantInfo::CCombatWheel::GetRing(long index) const
{
    if (index >= 0 && (unsigned long)index < CombatRings.size())
        return &CombatRings[index];
    return 0;
}
#pragma auto_inline(off)
float CDefendingCombatantInfo::CCombatWheel::GetRingRadius(long ring) const
{
    float radius = 0.0f;
    for (long i = ring; i >= 0; --i)
        radius += GetRing(i)->RingThickness;
    return radius;
}
C3DVector CDefendingCombatantInfo::CCombatRing::GetAttackerProngVector(const CThingCreatureBase& attacker) const
{
    const CProng* prong = GetAttackerProng(attacker);
    if (prong)
        return prong->GetProngVector(RotationAngle);
    return C3DVector(1.0f, 0.0f, 0.0f);
}
float CDefendingCombatantInfo::CCombatRing::GetProngSeparationHalfAngle() const
{
    long count = Prongs.size();
    return (1.0f / count) * 0.5f;
}
#pragma auto_inline(on)

bool CCombatManager::IsCreatureInAttackRingSector(const CThingCreatureBase& attacker) const
{
    const CCombatantInfo& attackerInfo = GetCombatantInfo(attacker);
    const CThingCreatureBase* attacked = attackerInfo.AttackerInfo.Attacked;
    float attackedRadius = attacked->GetPhysicsTC()->GetRadius();
    const CCombatantInfo& attackedInfo = GetCombatantInfo(*attacked);
    long ringIndex = attackerInfo.AttackerInfo.CombatWheelRingIndex;
    if (ringIndex < 0)
        return false;
    const CDefendingCombatantInfo::CCombatWheel& wheel = attackedInfo.DefenderInfo.CombatWheel;
    float outerRadius = wheel.GetRingRadius(ringIndex) + attackedRadius + 0.6f;
    float innerRadius = 0.0f;
    if (ringIndex > 0)
        innerRadius = wheel.GetRingRadius(ringIndex - 1) + attackedRadius - 0.6f;
    C3DVector toAttacker = attacker.GetPos() - attacked->GetPos();
    toAttacker.Z = 0.0f;
    float distanceSquared = toAttacker.X * toAttacker.X + toAttacker.Y * toAttacker.Y + toAttacker.Z * toAttacker.Z;
    if (innerRadius * innerRadius < distanceSquared && outerRadius * outerRadius > distanceSquared) {
        if (distanceSquared > DELTA_NOTIONAL_ZERO) {
            float invLength = 1.0f / (float)sqrt(distanceSquared);
            toAttacker.X *= invLength;
            toAttacker.Y *= invLength;
            toAttacker.Z *= invLength;
        }
        const CDefendingCombatantInfo::CCombatRing* ring = wheel.GetRing(ringIndex);
        C3DVector prongVector = ring->GetAttackerProngVector(attacker);
        if (prongVector.X * toAttacker.X + prongVector.Y * toAttacker.Y + prongVector.Z * toAttacker.Z > cos(ring->GetProngSeparationHalfAngle() * (double)6.2831855f))
            return true;
    }
    return false;
}
