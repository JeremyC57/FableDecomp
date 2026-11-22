// 006f2c00 CGetBestFreeProngForAttacker::operator()
// Leaf: ??RCGetBestFreeProngForAttacker@?A0xd8b789b9@@QAEXAAVCProng@CDefendingCombatantInfo@@@Z
// Flags: none
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
#define FABLE_RETAIL_CCOMBATRING_ISOUTERRING
#define FABLE_RETAIL_CCOMBATRING_GETTERS
#define FABLE_RETAIL_FREE_PRONG_PASS26
#define FABLE_RETAIL_COMBAT_PRONG_OCCUPIER_PASS26
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#include "fable_compiler.h"
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(sqrt)
#endif
#include "engine/CThingCreatureBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CDefendingCombatantInfo.h"
#include "engine/CAttackingCombatantInfo.h"
class CWorldMap;
namespace {
// Fidelity: same-TU strong definition of 0x006E7EB0 (retail TU-mate)
//   GetPreferredCombatDistanceForAttacker verbatim from the landed 0x006E7EB0 row: Ego_d `float distance` (type 0x40), Ego_d parameter order
//   (defender, attacker), the PDB FABLE_FASTCALL (?GetPreferredCombatDistanceForAttacker@?A0xd8b789b9@@YIMABVCThingCreatureBase@@0@Z).
//   `double distance` gives DIFFER(379v375) here; the landed 0x006E89B0 with float gives DIFFER(344v344) at 0x006E8A7A (re-port debt, FINDINGS).
//   The object's out-of-line copy is byte-equal to retail's unreferenced copy at 0x006E7EB0 (57 B, SAME relocations only).
float FABLE_FASTCALL GetPreferredCombatDistanceForAttacker(const CThingCreatureBase& defender, const CThingCreatureBase& attacker)
{
    float separation = defender.GetPhysicsTC()->GetRadius();
    const CCombatTypeDef& combat_def = attacker.GetCombatTypeDef();
    float distance = combat_def.PreferredCombatDistance;
    if (distance < DELTA_NOTIONAL_ZERO) {
        float far_range = combat_def.FrontFarLimit;
        float middle_range = combat_def.FrontMiddleLimit;
        distance = (middle_range + far_range) / 2.0f;
    }
    separation += distance;
    return separation;
}
// Fidelity: same-TU strong definition of 0x006E7F00 (retail TU-mate)
float FABLE_FASTCALL GetAttackerProngSelectionCompareValue(const C3DVector& attackerPos, const C3DVector& defenderPos,
                                                       const C3DVector& prongVector, const C3DVector& placementVector)
{
    C3DVector flatPlacement(placementVector.X, placementVector.Y, 0.0f);
    C3DVector flatProng(prongVector.X, prongVector.Y, 0.0f);
    float lengthSquared = flatPlacement.X * flatPlacement.X + flatPlacement.Y * flatPlacement.Y + flatPlacement.Z * flatPlacement.Z;
    if (lengthSquared < 0.0001f)
        return 0.0001f;
    float inverse = 1.0f / (float)sqrt(lengthSquared);
    flatPlacement.X = inverse * flatPlacement.X;
    flatPlacement.Y = inverse * flatPlacement.Y;
    flatPlacement.Z = inverse * flatPlacement.Z;
    lengthSquared = flatProng.X * flatProng.X + flatProng.Y * flatProng.Y + flatProng.Z * flatProng.Z;
    if (lengthSquared < 0.0001f)
        return 0.0001f;
    inverse = 1.0f / (float)sqrt(lengthSquared);
    flatProng.X = inverse * flatProng.X;
    flatProng.Y = inverse * flatProng.Y;
    flatProng.Z = inverse * flatProng.Z;
    return flatProng.Z * flatPlacement.Z + (flatProng.X * flatPlacement.X + flatProng.Y * flatPlacement.Y) + 1.0f;
}
float FABLE_FASTCALL GetAttackerProngSelectionCurrentWeaponBaseCompareValue(const CThing& attacker, bool wantProjectile);    // 0x006E9F60
}
inline bool CDefendingCombatantInfo::CCombatRing::IsOuterRing(const CCombatTypeDef& defender_combat_type_def) const
{
    return Index >= defender_combat_type_def.DefenderWheelOuterRingStartIndex;
}
inline const CDefendingCombatantInfo::CCombatRing* CDefendingCombatantInfo::CCombatWheel::GetRing(long index) const
{
    if (index >= 0 && (unsigned long)index < CombatRings.size())
        return &CombatRings[index];
    return 0;
}
// Fidelity: same-TU strong definition of 0x006E8850 (retail TU-mate)
float CDefendingCombatantInfo::CCombatWheel::GetRingRadius(long ring) const
{
    float radius = 0.0f;
    for (long i = ring; i >= 0; --i)
        radius += GetRing(i)->RingThickness;
    return radius;
}
#include "engine/CGetBestFreeProngForAttacker.h"
#include "engine/ThingAlive.h"
#include "engine/GFCos.h"
namespace {
// The debug definitions at 0240E0A0 and 0240E290 return constants.
// The native caller folds both calls.
bool FABLE_FASTCALL IsPosValid(const CWorldMap&, const CThing&, const CThing&, const C3DVector&)
{
    return true;
}
float FABLE_FASTCALL GetAttackerProngSelectionNavigtionBasedCompareValue(
    const CThing&, const CThing&, const C3DVector&)
{
    return 0.0f;
}
void CGetBestFreeProngForAttacker::operator()(CDefendingCombatantInfo::CProng& prong)
{
    if (IS_THING_ALIVE(prong.GetPOccupyingAttacker()))
        return;
    bool outer_ring = false;
    float preferred_distance = GetPreferredCombatDistanceForAttacker(Defender, Attacker);
    if (Ring.IsOuterRing(Defender.GetCombatTypeDef())) {
        outer_ring = true;
        preferred_distance = Wheel.GetRingRadius(Ring.GetIndex());
    }
    C3DVector my_line = Attacker.GetPos() - Defender.GetPos();
    my_line.Z = 0.0f;
    float separation = my_line.GetAccurateMagnitude();
    C3DVector prong_line = prong.GetProngVector(Ring.GetRotationAngle()) * preferred_distance;
    prong_line.Z = 0.0f;
    C3DVector prong_pos = Defender.GetPos() + prong_line;
    bool my_prong = false;
    if (!AttackerCombatInfo.PreferredAttackDollySet &&
        prong_line.Dot(my_line) / (separation * preferred_distance) > GFCos(0.25f / 360.0f) &&
        separation < preferred_distance)
        my_prong = true;
    if (IsPosValid(WorldMap, Attacker, Defender, prong_pos) || my_prong) {
        if (my_prong) {
            BestCompareVal = 1.0e10f;
            PBestProng = &prong;
        } else {
            if (AttackerCombatInfo.PreferredAttackDollySet) {
                my_line.X = AttackerCombatInfo.PreferredAttackDollyVector.X;
                my_line.Y = AttackerCombatInfo.PreferredAttackDollyVector.Y;
            }
            float compare_val = 0.0f;
            compare_val += GetAttackerProngSelectionCompareValue(Defender.GetPos(), Attacker.GetPos(), prong_line, my_line);
            compare_val += GetAttackerProngSelectionNavigtionBasedCompareValue(Attacker, Defender, prong_pos);
            compare_val += GetAttackerProngSelectionCurrentWeaponBaseCompareValue(Attacker, outer_ring);
            if (compare_val > BestCompareVal) {
                BestCompareVal = compare_val;
                PBestProng = &prong;
            }
        }
    }
}
}