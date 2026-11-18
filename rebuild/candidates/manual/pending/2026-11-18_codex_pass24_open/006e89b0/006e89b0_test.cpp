#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#include "engine/C3DVector.h"
#include <cassert>
#include <cstdio>
#include <vector>

struct CCombatTypeDef
{
    float PreferredCombatDistance, FrontMiddleLimit, FrontFarLimit;
    long DefenderWheelOuterRingStartIndex;
};

struct CTCPhysicsBase
{
    float Radius;
    float GetRadius() const { return Radius; }
};

struct CThingCreatureBase
{
    CTCPhysicsBase Physics;
    CCombatTypeDef Combat;
    const CTCPhysicsBase* GetPhysicsTC() const { return &Physics; }
    const CCombatTypeDef& GetCombatTypeDef() const { return Combat; }
};

struct CProng
{
    C3DVector Direction;
    C3DVector GetProngVector(float angle) const
    {
        assert(angle == 0.25f);
        return Direction;
    }
};

struct CDefendingCombatantInfo
{
    struct CCombatRing
    {
        long Index;
        float RingThickness, RotationAngle;
        const CProng* Prong;
        const CThingCreatureBase* ExpectedAttacker;
        const CProng* GetAttackerProng(const CThingCreatureBase& attacker) const
        {
            assert(ExpectedAttacker == &attacker);
            return Prong;
        }
        C3DVector GetAttackerProngVector(const CThingCreatureBase& attacker) const;
    };

    struct CCombatWheel
    {
        std::vector<CCombatRing> CombatRings;
        const CCombatRing* GetRing(long index) const;
        float GetRingRadius(long index) const;
        C3DVector GetAttackerPlacementVector(
            const CThingCreatureBase& attacker, const CThingCreatureBase& defender, long ring_index) const;
    };
};
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

int main()
{
    const CProng angled = { C3DVector(0.25f, -0.5f, 0.75f) };
    const float thresholds[] = { -1.0f, 0.0001f, 2.0f };
    const float midpoints[] = { 1.0f, 4.0f, 9.0f };
    CDefendingCombatantInfo::CCombatWheel wheel;
    wheel.CombatRings.resize(3);
    for (long i = 0; i < 3; ++i) {
        wheel.CombatRings[i].Index = i;
        wheel.CombatRings[i].RingThickness = float((i + 1) * 2);
        wheel.CombatRings[i].RotationAngle = 0.25f;
    }
    assert(!wheel.GetRing(-1) && !wheel.GetRing(3));
    for (long outer = 0; outer <= 3; ++outer) {
        for (long ring = 0; ring < 3; ++ring) {
            for (unsigned p = 0; p < 3; ++p) {
                for (unsigned with_prong = 0; with_prong < 2; ++with_prong) {
                    CThingCreatureBase attacker = { {0}, {thresholds[p], 4, 8, 0} };
                    CThingCreatureBase defender = { {3}, {0, 0, 0, outer} };
                    wheel.CombatRings[ring].Prong = with_prong ? &angled : 0;
                    wheel.CombatRings[ring].ExpectedAttacker = &attacker;
                    const float expected = ring >= outer ? midpoints[ring] :
                                           p == 0 ? 9.0f : 3.0f + thresholds[p];
                    const C3DVector actual = wheel.GetAttackerPlacementVector(attacker, defender, ring);
                    assert(actual.X == (with_prong ? expected * 0.25f : expected));
                    assert(actual.Y == (with_prong ? expected * -0.5f : 0));
                    assert(actual.Z == (with_prong ? expected * 0.75f : 0));
                }
            }
        }
    }
    std::puts("006e89b0_TEST PASS");
    return 0;
}
