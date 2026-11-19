// Standalone C++03 fixture for CDefendingCombatantInfo::CCombatWheel::GetAttackerPlacementVector (0x006E89B0) and its
// inlined same-TU helper GetPreferredCombatDistanceForAttacker. Game types are reduced to the members the bodies read.
#include <cstdio>
#include <cmath>
#include <vector>

static const float DELTA_NOTIONAL_ZERO = 0.0001f;

struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    const C3DVector operator*(float scale) const { return C3DVector(X * scale, Y * scale, Z * scale); }
    const C3DVector operator/(float v) const { float reciprocal = 1.0f / v; return C3DVector(X * reciprocal, Y * reciprocal, Z * reciprocal); }
};

struct CTCPhysicsBase { float Radius; float GetRadius() const { return Radius; } };
struct CCombatTypeDef {
    float FrontMiddleLimit, FrontFarLimit, PreferredCombatDistance;
    long DefenderWheelOuterRingStartIndex;
};
struct CThingCreatureBase {
    CTCPhysicsBase Physics;
    CCombatTypeDef CombatDef;
    int ProngIndex;  // which prong the attacker holds on a ring (-1: none)
    const CTCPhysicsBase* GetPhysicsTC() const { return &Physics; }
    const CCombatTypeDef& GetCombatTypeDef() const { return CombatDef; }
};

static int g_assert_failures = 0;
#define FABLE_RETAIL_ASSERT(c) do { if (!(c)) ++g_assert_failures; } while (0)

struct CDefendingCombatantInfo {
    struct CProng {
        float Angle;
        C3DVector GetProngVector(float rotation) const { float a = Angle + rotation; return C3DVector(std::cos(a), std::sin(a), 0.0f); }
    };
    struct CCombatRing {
        float RotationAngle; long Index; float RingThickness; std::vector<CProng> Prongs;
        const CProng* GetAttackerProng(const CThingCreatureBase& attacker) const
        {
            if (attacker.ProngIndex < 0 || (unsigned long)attacker.ProngIndex >= Prongs.size()) return 0;
            return &Prongs[attacker.ProngIndex];
        }
        C3DVector GetAttackerProngVector(const CThingCreatureBase& attacker) const;
        bool IsOuterRing(const CCombatTypeDef& defender_combat_type_def) const;
    };
    struct CCombatWheel {
        std::vector<CCombatRing> CombatRings;
        const CCombatRing* GetRing(long index) const;
        float GetRingRadius(long index) const;
        C3DVector GetAttackerPlacementVector(const CThingCreatureBase& attacker, const CThingCreatureBase& defender, long ring_index) const;
    };
};

// Recovered bodies (same text as the row source).
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

static bool near(float a, float b) { return std::fabs(a - b) <= 1e-4f * (1.0f + std::fabs(b)); }
static bool same(const C3DVector& v, float x, float y, float z) { return near(v.X, x) && near(v.Y, y) && near(v.Z, z); }

int main()
{
    typedef CDefendingCombatantInfo D;
    D::CCombatWheel wheel;
    const float thick[3] = { 2.0f, 3.0f, 5.0f };
    const float rot[3] = { 0.0f, 0.5f, 1.0f };
    for (int r = 0; r < 3; ++r) {
        D::CCombatRing ring;
        ring.RotationAngle = rot[r]; ring.Index = r; ring.RingThickness = thick[r];
        for (int p = 0; p < 4; ++p) { D::CProng prong; prong.Angle = 1.5707964f * p; ring.Prongs.push_back(prong); }
        wheel.CombatRings.push_back(ring);
    }
    CThingCreatureBase defender = { { 1.5f }, { 0.0f, 0.0f, 0.0f, 2 }, -1 };
    CThingCreatureBase attacker = { { 0.5f }, { 4.0f, 8.0f, 3.0f, 0 }, 1 };
    bool ok = true;

    // Inner ring 0, preferred distance set: prong 1 of ring 0 (angle pi/2), scaled by defender radius + preferred distance.
    C3DVector v = wheel.GetAttackerPlacementVector(attacker, defender, 0);
    ok = ok && same(v, std::cos(1.5707964f) * 4.5f, std::sin(1.5707964f) * 4.5f, 0.0f);
    // Inner ring 1 (rotation 0.5), preferred distance below DELTA_NOTIONAL_ZERO: middle of the front band, (4 + 8) / 2 = 6.
    attacker.CombatDef.PreferredCombatDistance = 0.00005f;
    v = wheel.GetAttackerPlacementVector(attacker, defender, 1);
    ok = ok && same(v, std::cos(1.5707964f + 0.5f) * 7.5f, std::sin(1.5707964f + 0.5f) * 7.5f, 0.0f);
    // Exactly zero and negative preferred distances also take the band middle.
    attacker.CombatDef.PreferredCombatDistance = 0.0f;
    ok = ok && same(wheel.GetAttackerPlacementVector(attacker, defender, 1), std::cos(2.0707964f) * 7.5f, std::sin(2.0707964f) * 7.5f, 0.0f);
    attacker.CombatDef.PreferredCombatDistance = -2.0f;
    ok = ok && same(wheel.GetAttackerPlacementVector(attacker, defender, 0), std::cos(1.5707964f) * 7.5f, std::sin(1.5707964f) * 7.5f, 0.0f);
    // A preferred distance just above the bound is kept.
    attacker.CombatDef.PreferredCombatDistance = 0.0002f;
    ok = ok && same(wheel.GetAttackerPlacementVector(attacker, defender, 0), 0.0f, 1.5002f, 0.0f);
    // Outer ring 2 (index >= outer start 2): ring band middle, radius(1) = 5, radius(2) = 10, (5 + 10) / 2 = 7.5.
    attacker.ProngIndex = 2;
    v = wheel.GetAttackerPlacementVector(attacker, defender, 2);
    ok = ok && same(v, std::cos(3.1415927f + 1.0f) * 7.5f, std::sin(3.1415927f + 1.0f) * 7.5f, 0.0f);
    // Outer ring 0 (outer start 0): no inner ring, (0 + 2) / 2 = 1; attacker with no prong uses the (1,0,0) default.
    defender.CombatDef.DefenderWheelOuterRingStartIndex = 0;
    attacker.ProngIndex = -1;
    ok = ok && same(wheel.GetAttackerPlacementVector(attacker, defender, 0), 1.0f, 0.0f, 0.0f);
    // Same defender, ring 1 is outer now: (2 + 5) / 2 = 3.5 along (1,0,0).
    ok = ok && same(wheel.GetAttackerPlacementVector(attacker, defender, 1), 3.5f, 0.0f, 0.0f);
    // Outer start 3: every ring is inner; no-prong default scaled by 1.5 + 0.0002.
    defender.CombatDef.DefenderWheelOuterRingStartIndex = 3;
    ok = ok && same(wheel.GetAttackerPlacementVector(attacker, defender, 2), 1.5002f, 0.0f, 0.0f);
    ok = ok && g_assert_failures == 0;

    std::puts(ok ? "006e89b0_TEST PASS" : "006e89b0_TEST FAIL");
    return ok ? 0 : 1;
}