// Behaviour test for CCombatManager::IsCreatureInAttackRingSector (0x006E94D0): the row body and its same-TU helpers (GetRing,
// GetRingRadius, GetProngSeparationHalfAngle) verbatim over models of the combatant map, the physics radius and the prongs. Kills:
// the negative ring index accepted, the inner radius taken for ring 0 or dropped, the proximity added / subtracted the wrong way or
// dropped on either radius, the defender radius dropped, either bound inclusive or swapped, Z not cleared, the line reversed
// (attacker / defender swapped), the line left unnormalised, the wrong ring's prong, the prong rotation ignored, the half angle
// doubled or the cosine compare inverted.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <map>
#include <vector>
static void check(bool v, int line) { if (!v) { printf("FAIL line %d\n", line); exit(1); } }
#define CHECK(c) check((c), __LINE__)
#ifdef _MSC_VER
typedef unsigned __int64 UID;
#else
typedef unsigned long long UID;
#endif
static const double TWO_PI = 6.2831854820251465;
static const float DELTA_NOTIONAL_ZERO = 1e-4f;
static float GFSqr(float n) { return n * n; }
static float GFRoot(float n) { return (float)sqrt(n); }
static double GFCos(float a) { return cos(a * TWO_PI); }
static double GFSin(float a) { return sin(a * TWO_PI); }
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetSquaredMagnitude() const { return X * X + Y * Y + Z * Z; }
    float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
    C3DVector& operator/=(float d) { X /= d; Y /= d; Z /= d; return *this; }
};
struct CTCPhysicsBase { float Radius; float GetRadius() const { return Radius; } };
struct CThingCreatureBase {
    UID UniqueID; C3DVector Pos; CTCPhysicsBase* Physics;
    UID GetUniqueID() const { return UniqueID; }
    const C3DVector& GetPos() const { return Pos; }
    CTCPhysicsBase* GetPhysicsTC() const { return Physics; }
};
class CDefendingCombatantInfo {
public:
    class CProng {
    public:
        const CThingCreatureBase* Occupier; C3DVector Prong;
        C3DVector GetProngVector(float rotationAngle) const {          // model of 0x006E7FD0: Prong turned about Z by the ring angle
            float c = (float)GFCos(rotationAngle), s = (float)GFSin(rotationAngle);
            return C3DVector(Prong.X * c - Prong.Y * s, Prong.X * s + Prong.Y * c, Prong.Z);
        }
    };
    class CCombatRing {
    public:
        float RotationAngle; float RingThickness; std::vector<CProng> Prongs;
        const CProng* GetAttackerProng(const CThingCreatureBase& attacker) const {
            for (unsigned long i = 0; i != Prongs.size(); ++i) if (Prongs[i].Occupier == &attacker) return &Prongs[i];
            return 0;
        }
        C3DVector GetAttackerProngVector(const CThingCreatureBase& attacker) const {
            const CProng* pprong = GetAttackerProng(attacker);
            if (pprong) return pprong->GetProngVector(RotationAngle);
            return C3DVector(1.0f, 0.0f, 0.0f);
        }
        float GetProngSeparationHalfAngle() const;
    };
    class CCombatWheel {
    public:
        std::vector<CCombatRing> CombatRings;
        const CCombatRing* GetRing(long index) const;
        float GetRingRadius(long index) const;
    };
    CCombatWheel CombatWheel;
};
struct CAttackingCombatantInfo { const CThingCreatureBase* Attacked; long CombatWheelRingIndex; };
struct CCombatantInfo { CAttackingCombatantInfo AttackerInfo; CDefendingCombatantInfo DefenderInfo; };
class CCombatManager {
public:
    std::map<UID, CCombatantInfo> Combatants;
    const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) const { return Combatants.find(creature.GetUniqueID())->second; }
    bool IsCreatureInAttackRingSector(const CThingCreatureBase& attacker) const;
};
// ---- row TU (verbatim) ----
static const float MOVE_TO_PRONG_NAV_PROXIMITY = 0.6f;
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
// ---- end row ----

static CCombatManager mgr;
static CTCPhysicsBase defender_phys;
static CThingCreatureBase defender, attacker, stranger;

// Defender at (10, 10, 0), radius 0.5; ring 0 thickness 2 (radius 2), ring 1 thickness 3 (radius 5); four prongs per ring
// (half angle 1/8 turn: cos 0.7071). Ring 0: rotation 0, attacker on prong (1,0,0). Ring 1: rotation `rot1`, attacker on prong 2.
static void setup(long ring_index, float rot1, float defender_radius, bool on_prong)
{
    mgr.Combatants.clear();
    defender_phys.Radius = defender_radius;
    defender.UniqueID = 7; defender.Pos = C3DVector(10.0f, 10.0f, 0.0f); defender.Physics = &defender_phys;
    attacker.UniqueID = 0x100000003ULL; attacker.Physics = 0;
    stranger.UniqueID = 99;
    CCombatantInfo a; a.AttackerInfo.Attacked = &defender; a.AttackerInfo.CombatWheelRingIndex = ring_index;
    CCombatantInfo d; d.AttackerInfo.Attacked = 0; d.AttackerInfo.CombatWheelRingIndex = -1;
    for (int r = 0; r != 2; ++r) {
        CDefendingCombatantInfo::CCombatRing ring;
        ring.RotationAngle = r ? rot1 : 0.0f; ring.RingThickness = r ? 3.0f : 2.0f;
        for (int p = 0; p != 4; ++p) {
            CDefendingCombatantInfo::CProng prong;
            prong.Occupier = (on_prong && p == (r ? 2 : 0)) ? &attacker : &stranger;
            float ang = p * 0.25f;
            prong.Prong = C3DVector((float)GFCos(ang), (float)GFSin(ang), 0.0f);
            if (r == 1 && p == 2) prong.Prong = C3DVector(1.0f, 0.0f, 0.0f);   // ring 1's attacker prong points along +X before rotation
            ring.Prongs.push_back(prong);
        }
        d.DefenderInfo.CombatWheel.CombatRings.push_back(ring);
    }
    mgr.Combatants[attacker.UniqueID] = a;
    mgr.Combatants[defender.UniqueID] = d;
}
static bool at(float dx, float dy, float z)
{
    attacker.Pos = C3DVector(10.0f + dx, 10.0f + dy, z);
    return mgr.IsCreatureInAttackRingSector(attacker);
}

int main()
{
    long cases = 0;
    // Ring 1, no rotation: inner 2 + 0.5 - 0.6 = 1.9, outer 5 + 0.5 + 0.6 = 6.1.
    setup(1, 0.0f, 0.5f, true);
    CHECK(at(4.0f, 0.5f, 100.0f)); ++cases;     // inside; a large Z must be ignored
    CHECK(!at(1.8f, 0.0f, 0.0f)); ++cases;      // inside the inner radius
    CHECK(at(2.0f, 0.0f, 0.0f)); ++cases;       // just past 1.9 (an inner of 3.1 or 2.5 would reject)
    CHECK(at(6.0f, 0.0f, 0.0f)); ++cases;       // just inside 6.1 (an outer of 5.5 / 4.9 / 5.6 would reject)
    CHECK(!at(6.2f, 0.0f, 0.0f)); ++cases;      // outside the outer radius
    CHECK(!at(-4.0f, 0.0f, 0.0f)); ++cases;     // behind the prong (a reversed line would accept)
    CHECK(at(3.0f, 2.5f, 0.0f)); ++cases;       // 39.8 degrees off the prong: inside the 45-degree half sector
    CHECK(!at(3.0f, 3.5f, 0.0f)); ++cases;      // 49.4 degrees: outside (a full 90-degree sector would accept)
    CHECK(!at(1.0f, 4.0f, 0.0f)); ++cases;      // unnormalised dot 1.0 > 0.707 would accept; normalised 0.24 rejects
    // Ring 1 rotated a quarter turn: the prong points along +Y.
    setup(1, 0.25f, 0.5f, true);
    CHECK(at(0.0f, 4.0f, 0.0f)); ++cases;
    CHECK(!at(4.0f, 0.0f, 0.0f)); ++cases;
    // Defender radius 0: outer 5.6, inner 1.4.
    setup(1, 0.0f, 0.0f, true);
    CHECK(!at(5.8f, 0.0f, 0.0f)); ++cases;
    CHECK(at(1.5f, 0.0f, 0.0f)); ++cases;
    // Ring 0: no inner radius (0), outer 2 + 0.5 + 0.6 = 3.1; the ring-0 prong is (1,0,0).
    setup(0, 0.0f, 0.5f, true);
    CHECK(at(0.3f, 0.0f, 0.0f)); ++cases;
    CHECK(at(3.0f, 0.0f, 0.0f)); ++cases;
    CHECK(!at(3.2f, 0.0f, 0.0f)); ++cases;
    CHECK(!at(0.0f, 0.0f, 5.0f)); ++cases;      // on the defender: zero length fails the strict inner bound
    CHECK(at(0.05f, 0.0f, 0.0f)); ++cases;      // ring 0 has no inner radius (ring -1's 0 + 0.5 - 0.6 squared would reject)
    // No prong held: the unit X fallback.
    setup(1, 0.25f, 0.5f, false);
    CHECK(at(4.0f, 0.0f, 0.0f)); ++cases;
    CHECK(!at(0.0f, 4.0f, 0.0f)); ++cases;
    // Not in a ring.
    setup(-1, 0.0f, 0.5f, true);
    CHECK(!at(4.0f, 0.0f, 0.0f)); ++cases;
    CHECK(!at(0.5f, 0.0f, 0.0f)); ++cases;      // inside the would-be outer radius 1.1: still not in a ring
    CHECK(cases == 22);
    puts("006e94d0_TEST PASS");
    return 0;
}