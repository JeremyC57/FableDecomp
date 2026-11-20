// `anonymous namespace'::CGetBestAttackerForProng::operator()(std::pair<CThingCreatureBase*, CAttackingCombatantInfo*> attacker) -- score one attacker for the functor's prong: the defender-to-attacker line (or the attacker's preferred dolly vector) against the prong line scaled by the preferred distance (the ring radius on an outer ring), plus the weapon-preference value; remember the best attacker and its score
// Inline-budget: no flag, no pragma, no device: GetPreferredCombatDistanceForAttacker (TU-visible, __fastcall) is expanded at 0x006F2DE8..0x006F2E2D by VC7.1's own /O2 auto-inlining and still emitted out of line (retail keeps the unreferenced copy at 0x006E7EB0); IsOuterRing / GetRing are plain inline. Two same-TU bodies are visible for c2's side-effect knowledge, not for inlining: GetAttackerProngSelectionCompareValue 0x006E7F00 (Fidelity copy; declared only, every product / difference is loaded before any store to prong_line / my_line, whose addresses it takes: DIFFER(375v375)) and CCombatWheel::GetRingRadius 0x006E8850 (Fidelity copy; declared only, Ring is reloaded from [esi] after the call instead of kept in ebp: DIFFER(376v375)).
// Leaf: ??RCGetBestAttackerForProng@
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F2DE0, 375 bytes, thiscall `ret 8` (the pair by value), esp frame 0x2C, no EH frame. TU
//   combat_manager.obj (anonymous namespace ?A0xd8b789b9 in Ego_r / Ego_d); called by std::for_each 0x006F34B0 (landed) from
//   CCombatRing::AssignAttackerToProng 0x006EC1AC, which builds the functor inline (BestCompareVal -1.0f, PBestAttacker 0).
// Identity: Ego_d.pdb / FableWin.pdb ??RCGetBestAttackerForProng@?A0xd8b789b9@@QAEXU?$pair@PAVCThingCreatureBase@@PAVCAttackingCombatantInfo@@@std@@@Z.
//   Ego_d TPI 0x503E1: members Ring / Wheel / Prong / Defender / WorldMap are references (the constructor's const& parameter types),
//   BestCompareVal float +0x14, PBestAttacker +0x18; methods the constructor (world_map, wheel, ring, prong, defender),
//   operator(), GetPBestAttacker. Ego_d locals: attacker; prong_line, outer_ring, preferred_distance, prong_pos; block { my_line,
//   attacker_pos, compare_val }.
// Shape (Ego_d.pdb C13 lines 337..367, egod_lines/006f2de0.tsv, VC10 /Od sizes):
//   - 338 (4 B) outer_ring = false; 339 (17 B, fastcall + fstp) GetPreferredCombatDistanceForAttacker(Defender, *attacker.first);
//     340 (29 B) `if (Ring.IsOuterRing(Defender.GetCombatTypeDef()))`; 342 (4 B) outer_ring = true; 343 (25 B, `mov ecx,[eax] ;
//     call` + GetRingRadius) Wheel.GetRingRadius(Ring.GetIndex()).
//   - 345 (47 B) prong_line from Prong.GetProngVector(Ring.GetRotationAngle()) * preferred_distance, copy-initialised (an
//     assignment adds an 18-byte copy at /Od); 346 (26 B) `C3DVector prong_pos = Defender.GetPos() + prong_line;` (unused: only the
//     GetPos call 0x006F2E89 survives).
//   - 350 (17 B) attacker_pos; 351 / 352 my_line and its Z = 0; 353 (11 B) `if (attacker.second->PreferredAttackDollySet)`; 360
//     (17 B) `if (compare_val > BestCompareVal)`; 363 / 364 (9 B each) PBestAttacker then BestCompareVal, retail's store order.
// Callees: CThingCreatureBase::GetCombatTypeDef 0x00666D10, CCombatWheel::GetRingRadius 0x006E8850, CProng::GetProngVector
//   0x006E7FD0, CThing::GetPos 0x004C73D0, GetAttackerProngSelectionCompareValue 0x006E7F00 (fastcall: defender position in ecx,
//   attacker position in edx), GetAttackerProngSelectionCurrentWeaponBaseCompareValue 0x006E9F60 (fastcall, dl = outer_ring);
//   DELTA_NOTIONAL_ZERO 0x0129BA3C, 0.5f 0x0122F59C.
// Overlay (add-only opt-in): engine/CDefendingCombatantInfo.h FABLE_RETAIL_CCOMBATRING_GETTERS (GetIndex, GetRotationAngle; inferred
//   one-member bodies).
// Inferred: the getter bodies (see the helper's own line for its text); the functor's constructor and GetPBestAttacker bodies (the caller's inline stores at 0x006EC17C..0x006EC188).
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
#include "fable_compiler.h"
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
#include "engine/CThingCreatureBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CDefendingCombatantInfo.h"
#include "engine/CAttackingCombatantInfo.h"
class CWorldMap;
namespace {
// Inferred: GetPreferredCombatDistanceForAttacker is the landed 0x006E89B0 helper's text with Ego_d's `float distance` (type 0x40), Ego_d's
//   parameter order (defender, attacker) and the PDB's __fastcall (Ego_r / FableWin ?GetPreferredCombatDistanceForAttacker@?A0xd8b789b9@@YIMABVCThingCreatureBase@@0@Z).
//   `double distance` gives DIFFER(379v375) here; the landed 0x006E89B0 with float gives DIFFER(344v344) at 0x006E8A7A (pop esi / fmul st(1)).
//   Check: the object's out-of-line copy is byte-equal to retail's unreferenced copy at 0x006E7EB0 (57 B, SAME relocations only; with double
//   too; cdecl or the landed (attacker, defender) order: DIFF). No row or label holds 0x006E7EB0 yet.
float __fastcall GetPreferredCombatDistanceForAttacker(const CThingCreatureBase& defender, const CThingCreatureBase& attacker)
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
float __fastcall GetAttackerProngSelectionCompareValue(const C3DVector& attackerPos, const C3DVector& defenderPos,
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
float __fastcall GetAttackerProngSelectionCurrentWeaponBaseCompareValue(const CThing& attacker, bool wantProjectile);    // 0x006E9F60
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
namespace {
class CGetBestAttackerForProng {
public:
    CGetBestAttackerForProng(const CWorldMap& world_map, const CDefendingCombatantInfo::CCombatWheel& wheel,
                             const CDefendingCombatantInfo::CCombatRing& ring, const CDefendingCombatantInfo::CProng& prong,
                             const CThingCreatureBase& defender)
        : Ring(ring), Wheel(wheel), Prong(prong), Defender(defender), WorldMap(world_map), BestCompareVal(-1.0f), PBestAttacker(0) {}
    void operator()(std::pair<CThingCreatureBase*, CAttackingCombatantInfo*> attacker);
    CThingCreatureBase* GetPBestAttacker() const { return PBestAttacker; }
private:
    const CDefendingCombatantInfo::CCombatRing& Ring;
    const CDefendingCombatantInfo::CCombatWheel& Wheel;
    const CDefendingCombatantInfo::CProng& Prong;
    const CThingCreatureBase& Defender;
    const CWorldMap& WorldMap;
    float BestCompareVal;
    CThingCreatureBase* PBestAttacker;
};
void CGetBestAttackerForProng::operator()(std::pair<CThingCreatureBase*, CAttackingCombatantInfo*> attacker)
{
    bool outer_ring = false;
    float preferred_distance = GetPreferredCombatDistanceForAttacker(Defender, *attacker.first);
    if (Ring.IsOuterRing(Defender.GetCombatTypeDef())) {
        outer_ring = true;
        preferred_distance = Wheel.GetRingRadius(Ring.GetIndex());
    }
    C3DVector prong_line = Prong.GetProngVector(Ring.GetRotationAngle()) * preferred_distance;
    C3DVector prong_pos = Defender.GetPos() + prong_line;
    {
        C3DVector attacker_pos = attacker.first->GetPos();
        C3DVector my_line = attacker_pos - Defender.GetPos();
        my_line.Z = 0.0f;
        if (attacker.second->PreferredAttackDollySet)
            my_line = C3DVector(attacker.second->PreferredAttackDollyVector.X, attacker.second->PreferredAttackDollyVector.Y, 0.0f);
        float compare_val = GetAttackerProngSelectionCompareValue(Defender.GetPos(), attacker.first->GetPos(), prong_line, my_line);
        compare_val += GetAttackerProngSelectionCurrentWeaponBaseCompareValue(*attacker.first, outer_ring);
        if (compare_val > BestCompareVal) {
            PBestAttacker = attacker.first;
            BestCompareVal = compare_val;
        }
    }
}
}