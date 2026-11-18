// CDefendingCombatantInfo::CCombatWheel::GetAttackerPlacementVector(const CThingCreatureBase& attacker, const CThingCreatureBase& defender, long ring_index) -- where an attacker stands on the defender's combat wheel: its prong direction on the ring, scaled by the defender's radius plus the attacker's preferred distance on an inner ring, else by the middle of the ring band
// Grade: DIFFER(344v344) -- In the preferred-distance branch, retail multiplies prong.X by the distance at 0x006E8A7A before restoring esi at 0x006E8A7C. The selected compiler build restores esi first.
// Leaf: ?GetAttackerPlacementVector@CCombatWheel@CDefendingCombatantInfo@@QBE?AVC3DVector@@ABVCThingCreatureBase@@0J@Z
// Flags: none
// Retail: Fable.exe 0x006E89B0, 344 bytes, `ret 0x10`. Same-TU bodies in Ego_d combat_manager.obj order: GetPreferredCombatDistanceForAttacker (anon), CCombatRing::GetAttackerProngVector (0x006E8810), CCombatWheel::GetRingRadius (0x006E8850).
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETCOMBATTYPEDEF
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PRONG
#define FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_WHEEL
#define FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PLACEMENTVECTOR
#include "fable_compiler.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CDefendingCombatantInfo.h"
namespace {
float GetPreferredCombatDistanceForAttacker(const CThingCreatureBase& attacker, const CThingCreatureBase& defender)
{
    float separation = defender.GetPhysicsTC()->GetRadius();
    const CCombatTypeDef& combat_def = attacker.GetCombatTypeDef();
    float distance = combat_def.PreferredCombatDistance;
    if (distance < 0.0001f) {
        float far_range = combat_def.FrontFarLimit;
        float middle_range = combat_def.FrontMiddleLimit;
        distance = (middle_range + far_range) * 0.5f;
    }
    return separation + distance;
}
}
inline const CDefendingCombatantInfo::CCombatRing* CDefendingCombatantInfo::CCombatWheel::GetRing(long index) const
{
    if (index >= 0 && (unsigned long)index < CombatRings.size())
        return &CombatRings[index];
    return 0;
}
C3DVector CDefendingCombatantInfo::CCombatRing::GetAttackerProngVector(const CThingCreatureBase& attacker) const
{
    const CProng* pprong = GetAttackerProng(attacker);
    if (pprong)
        return pprong->GetProngVector(RotationAngle);
    return C3DVector(1.0f, 0.0f, 0.0f);
}
float CDefendingCombatantInfo::CCombatWheel::GetRingRadius(long index) const
{
    float dist = 0.0f;
    for (long c1 = index; c1 >= 0; --c1) {
        const CCombatRing* pringu = GetRing(c1);
        dist += pringu->RingThickness;
    }
    return dist;
}
C3DVector CDefendingCombatantInfo::CCombatWheel::GetAttackerPlacementVector(const CThingCreatureBase& attacker,
    const CThingCreatureBase& defender, long ring_index) const
{
    const CCombatRing* pring = GetRing(ring_index);
    C3DVector prong = pring->GetAttackerProngVector(attacker);
    if (pring->Index < defender.GetCombatTypeDef().DefenderWheelOuterRingStartIndex) {
        float pref_dist = GetPreferredCombatDistanceForAttacker(attacker, defender);
        return prong * pref_dist;
    }
    float inner_ring_rad = 0.0f;
    if (ring_index > 0)
        inner_ring_rad = GetRingRadius(ring_index - 1);
    float outer_ring_rad = GetRingRadius(ring_index);
    return prong * (inner_ring_rad + outer_ring_rad) * 0.5f;
}
