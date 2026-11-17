// Behaviour test for CThingFilter_IsAICreatureMatchingTypeAndFactionNearby's constructor (0x00986ED0): MinCosine 0, RestrictAngle,
// OurPos = the creature's position, MaxSqDist = max_dist squared, OurCreatureType, PFaction from the creature's enemy TC; with
// restrict_angle, SideVec = the physics right-handed set's Forward ^ Up (negated for a HUMAN_CHILD) and MinCosine = cos(0.125 turn).
#include <cstdio>
#include <cmath>

struct V { float X, Y, Z; };
static V cross(const V& a, const V& b) { V r = { a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X }; return r; }
enum { HUMAN_ADULT = 0, HUMAN_CHILD = 1 };
struct Faction { int id; };
struct Creature { V pos; long type; const Faction* faction; V up, forward; };

struct Filter {
    bool RestrictAngle; V OurPos; float MaxSqDist; V SideVec; long OurCreatureType; const Faction* PFaction; float MinCosine;
    Filter(const Creature& creature, float max_dist, bool restrict_angle)
    {
        SideVec.X = SideVec.Y = SideVec.Z = 99.0f;            // untouched without restrict_angle
        MinCosine = 0.0f;
        RestrictAngle = restrict_angle;
        OurPos = creature.pos;
        MaxSqDist = max_dist * max_dist;
        OurCreatureType = creature.type;
        PFaction = creature.faction;
        if (restrict_angle) {
            SideVec = cross(creature.forward, creature.up);
            if (OurCreatureType == HUMAN_CHILD) { SideVec.X = -SideVec.X; SideVec.Y = -SideVec.Y; SideVec.Z = -SideVec.Z; }
            MinCosine = (float)std::cos(0.125f * (double)6.2831855f);
        }
    }
};

static int fail(int n) { std::printf("00986ed0_TEST FAIL %d\n", n); return 1; }

int main()
{
    Faction f = { 7 };
    Creature c = { { 1, 2, 3 }, HUMAN_ADULT, &f, { 0, 0, 1 }, { 0, 1, 0 } };
    Filter a(c, 4.0f, false);
    if (a.RestrictAngle || a.MinCosine != 0.0f || a.MaxSqDist != 16.0f || a.OurPos.Y != 2 || a.PFaction != &f || a.OurCreatureType != 0) return fail(1);
    if (a.SideVec.X != 99.0f) return fail(2);
    Filter b(c, 3.0f, true);                                  // forward (0,1,0) ^ up (0,0,1) = (1,0,0)
    if (!b.RestrictAngle || b.SideVec.X != 1.0f || b.SideVec.Y != 0.0f || b.SideVec.Z != 0.0f) return fail(3);
    if (std::fabs(b.MinCosine - 0.70710677f) > 1e-6f || b.MaxSqDist != 9.0f) return fail(4);
    Creature k = c; k.type = HUMAN_CHILD;
    Filter d(k, 3.0f, true);                                  // a child faces the other side
    if (d.SideVec.X != -1.0f || d.OurCreatureType != HUMAN_CHILD) return fail(5);
    Filter e(k, 3.0f, false);
    if (e.SideVec.X != 99.0f || e.MinCosine != 0.0f) return fail(6);
    std::printf("00986ed0_TEST PASS\n");
    return 0;
}
