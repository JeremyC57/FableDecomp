// CCombatManager::IsCreatureInAttackRingSector(const CThingCreatureBase& attacker) const -- true when the attacker stands inside its assigned ring of the defender's combat wheel (between the inner and outer ring radius, each moved by the defender's radius and the prong proximity) and within its prong's angular sector
// Leaf: ?IsCreatureInAttackRingSector@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z
// Inline-budget: __declspec(noinline) through the header opt-in FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE -- retail calls the
//   map<unsigned __int64,CCombatantInfo> _M_find 0x006F0250 out of line at both GetCombatantInfo lookups (0x006E94F3 / 0x006E951F)
//   while the TU sees the template body, so VC7.1 knows the key is only read and the two key temporaries share one frame slot
//   ([esp+14h] both times, frame 0x1C). The declaration-only FABLE_RETAIL_STLP_TREE_MFIND_DECL gives the second key its own slot
//   (frame 0x24, every local 8 higher: 426 v 426, 36 bytes differ).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E94D0, 426 bytes, thiscall `ret 4`, bool in al.
// Name: Ego_r.pdb (0x007127E0) + FableWin.pdb (0x024167F0); the catalog row "Combat_CheckRingRangeToTarget" was descriptive.
// Source: FableWin.pdb debug body 0x024167F0 (combat_manager.obj) with Ego_d.pdb's locals (attacker; defender_radius, ring_index,
//   defender_info, combatant_info, pdefender; block: line_out, inner_radius, sqr_mag, outer_rad; inner block: ring_vector, pring,
//   cos_theta). The two debug ASSERTs (IsAttacker, IS_THING_ALIVE(pdefender), pring) emit nothing in release. The 0.6 is the TU-static
//   const float MOVE_TO_PRONG_NAV_PROXIMITY (FableWin S_LDATA32 0x0420A828; retail 0x01261D68).
#include "fable_compiler.h"
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_MATH_TRIG_TWO_PI
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
#define FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE
#include <math.h>
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/GFCos.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCombatManager.h"

static const float MOVE_TO_PRONG_NAV_PROXIMITY = 0.6f;

// Same-TU bodies (FableWin.pdb puts all four in combat_manager.obj before IsCreatureInAttackRingSector; retail 0x006E8520 /
// 0x006E8850 / 0x006E8810 / 0x006E8460, landed as their own rows). Retail calls them out of line here; seeing their bodies is
// what lets VC7.1 skip the ecx reload before the second GetRingRadius (0x006E955B) and keep the ring in esi across
// GetProngSeparationHalfAngle (0x006E964C). Bodies from the FableWin debug copies (GetRing 0x024111E0, GetRingRadius 0x02410550,
// GetAttackerProngVector 0x02410110, GetProngSeparationHalfAngle 0x02410280) with Ego_d.pdb's locals; their debug ASSERTs emit
// nothing in release.
const CDefendingCombatantInfo::CCombatRing* CDefendingCombatantInfo::CCombatWheel::GetRing(long index) const
{
    if (index >= 0 && (unsigned long)index < CombatRings.size())
        return &CombatRings[index];
    return 0;
}

float CDefendingCombatantInfo::CCombatWheel::GetRingRadius(long index) const
{
    float dist = 0.0f;
    for (long c1 = index; c1 >= 0; c1--) {
        const CCombatRing* pringu = GetRing(c1);
        dist += pringu->RingThickness;
    }
    return dist;
}

C3DVector CDefendingCombatantInfo::CCombatRing::GetAttackerProngVector(const CThingCreatureBase& attacker) const
{
    const CProng* pprong = GetAttackerProng(attacker);
    if (pprong)
        return pprong->GetProngVector(RotationAngle);
    return C3DVector(1.0f, 0.0f, 0.0f);
}

float CDefendingCombatantInfo::CCombatRing::GetProngSeparationHalfAngle() const
{
    long num_prongs = Prongs.size();
    float prong_sep = 1.0f / num_prongs;
    return prong_sep / 2.0;
}


bool CCombatManager::IsCreatureInAttackRingSector(const CThingCreatureBase& attacker) const
{
    const CCombatantInfo& combatant_info = GetCombatantInfo(attacker);
    const CThingCreatureBase* pdefender = combatant_info.AttackerInfo.Attacked;
    float defender_radius = pdefender->GetPhysicsTC()->GetRadius();
    const CCombatantInfo& defender_info = GetCombatantInfo(*pdefender);
    long ring_index = combatant_info.AttackerInfo.CombatWheelRingIndex;
    if (ring_index >= 0) {
        float outer_rad = defender_info.DefenderInfo.CombatWheel.GetRingRadius(ring_index) + defender_radius + MOVE_TO_PRONG_NAV_PROXIMITY;
        float inner_radius = 0.0f;
        if (ring_index > 0)
            inner_radius = defender_info.DefenderInfo.CombatWheel.GetRingRadius(ring_index - 1) + defender_radius - MOVE_TO_PRONG_NAV_PROXIMITY;
        C3DVector line_out = attacker.GetPos() - pdefender->GetPos();
        line_out.Z = 0.0f;
        float sqr_mag = line_out.GetSquaredMagnitude();
        if (sqr_mag > GFSqr(inner_radius) && sqr_mag < GFSqr(outer_rad)) {
            if (sqr_mag > DELTA_NOTIONAL_ZERO)
                line_out /= GFRoot(sqr_mag);
            const CDefendingCombatantInfo::CCombatRing* pring = defender_info.DefenderInfo.CombatWheel.GetRing(ring_index);
            C3DVector ring_vector = pring->GetAttackerProngVector(attacker);
            float cos_theta = ring_vector.Dot(line_out);
            if (cos_theta > GFCos(pring->GetProngSeparationHalfAngle()))
                return true;
        }
    }
    return false;
}