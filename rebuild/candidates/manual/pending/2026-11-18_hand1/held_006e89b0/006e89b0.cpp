// CDefendingCombatantInfo::CCombatWheel::GetAttackerPlacementVector(const CThingCreatureBase& attacker, const CThingCreatureBase& defender, long ring_index) -- where an attacker stands on the defender's combat wheel: its prong direction on the ring, scaled by the defender's radius plus the attacker's preferred distance on an inner ring, else by the middle of the ring band
// Grade: RELOCATION_MATCH (344v344) -- closed by the inlined helper's `double distance` (Inferred, see below); measured: identical c2 DAG, fmul picked a cycle earlier (inferred: double-typed join
//   add latency), so prong.X is multiplied at 0x006E8A7A before `pop esi` as in retail (0x006E8A6D `fadd dword [esp+38h]`).
// Leaf: ?GetAttackerPlacementVector@CCombatWheel@CDefendingCombatantInfo@@QBE?AVC3DVector@@ABVCThingCreatureBase@@0J@Z
// Flags: none
// Retail: Fable.exe 0x006E89B0, 344 bytes, `ret 0x10`. Same-TU bodies in Ego_d combat_manager.obj order: GetPreferredCombatDistanceForAttacker (anon), CCombatRing::GetAttackerProngVector (0x006E8810), CCombatWheel::GetRingRadius (0x006E8850).
// Structure and call targets from the FableWin.exe debug copies (combat_manager.obj, Dec 2012 revision, VC10 /Od), each checked
//   against retail's bytes:
//   - GetAttackerPlacementVector 0x024107B0: GetRing, ASSERT(pring) (dead in release), GetAttackerProngVector into the local
//     prong, `if (!pring->IsOuterRing(defender.GetCombatTypeDef()))`; arm 1 is a named float pref_dist and
//     `return prong * pref_dist` (C3DVector::operator* 0x018C0AF0 into the return slot); arm 2 is
//     `prong * (inner + outer) / 2.0f` (operator* into a temporary, then C3DVector::operator/ 0x01DB57B0 with a dword 2.0f).
//   - GetPreferredCombatDistanceForAttacker 0x0240E0C0 (fastcall): separation (GetPhysicsTC()->GetRadius()), combat_def,
//     distance = PreferredCombatDistance, `distance < DELTA_NOTIONAL_ZERO` (dword global), (middle + far) / 2.0,
//     `separation += distance; return separation;`.
// Inferred: retail differs from FableWin in two literal/type spots. Retail's halving is `fmul dword [0.5]` (0x006E8A67), so the
//   helper's divisor is the float 2.0f, not FableWin's double 2.0 (VC7.1 keeps a double literal as a qword load). And `distance`
//   is a double: a float distance (FableWin's type) leaves the arm-1 residue above (pop esi before the fmul), a double one that
//   never spills (no qword operand anywhere in retail) matches. Retail compares it against the dword DELTA_NOTIONAL_ZERO with
//   `fcom dword` (0x006E8A52), which a double compare does not fold, hence `(float)distance` in the comparison. Text-level fit; no donor
//   shows the double.
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
#define FABLE_RETAIL_CCOMBATRING_ISOUTERRING
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_compiler.h"
#include "fable_assert.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CDefendingCombatantInfo.h"
namespace {
float GetPreferredCombatDistanceForAttacker(const CThingCreatureBase& attacker, const CThingCreatureBase& defender)
{
    float separation = defender.GetPhysicsTC()->GetRadius();
    const CCombatTypeDef& combat_def = attacker.GetCombatTypeDef();
    double distance = combat_def.PreferredCombatDistance;
    if ((float)distance < DELTA_NOTIONAL_ZERO) {
        float far_range = combat_def.FrontFarLimit;
        float middle_range = combat_def.FrontMiddleLimit;
        distance = (middle_range + far_range) / 2.0f;
    }
    separation += distance;
    return separation;
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
inline bool CDefendingCombatantInfo::CCombatRing::IsOuterRing(const CCombatTypeDef& defender_combat_type_def) const
{
    return Index >= defender_combat_type_def.DefenderWheelOuterRingStartIndex;
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
    FABLE_RETAIL_ASSERT(pring);
    C3DVector prong = pring->GetAttackerProngVector(attacker);
    if (!pring->IsOuterRing(defender.GetCombatTypeDef())) {
        float pref_dist = GetPreferredCombatDistanceForAttacker(attacker, defender);
        return prong * pref_dist;
    }
    float inner_ring_rad = 0.0f;
    if (ring_index > 0)
        inner_ring_rad = GetRingRadius(ring_index - 1);
    float outer_ring_rad = GetRingRadius(ring_index);
    return prong * (inner_ring_rad + outer_ring_rad) / 2.0f;
}
