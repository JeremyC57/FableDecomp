#include <cstdio>
#include <cstring>
#include <math.h>
// Behaviour test for CCombatAction_KickThingOnGround::Initialise @ 0x0087E860: a living type-13 target gets the stealth
// kick (aim 1.0 forward, 0.15 aside), anything else the hard low kick (0.0, 0.1); a living target is suspended when it
// is type 1, made mutually not solid with the kicker, and aimed at through its hit location's closest point when it has
// one; MoveToPos keeps the kicker's height and, when the kicker's flat back-facing direction is not degenerate, is moved
// along it and beside it; the base Initialise always runs.
struct Vec {
    float X, Y, Z;
    Vec() { }
    Vec(float x, float y, float z) : X(x), Y(y), Z(z) { }
    Vec(const Vec& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    Vec operator-() const { return Vec(-X, -Y, -Z); }
    Vec operator+(const Vec& o) const { return Vec(X + o.X, Y + o.Y, Z + o.Z); }
    Vec operator*(float f) const { return Vec(X * f, Y * f, Z * f); }
    void Normalise() { float r = 1.0f / (float)sqrt(X * X + Y * Y + Z * Z); X *= r; Y *= r; Z *= r; }
};
static const float g_epsilon = 0.0001f;

struct Thing {
    int type;
    bool dead;
    Vec position;
    Vec facing;
    bool hasHitLocation;
    Vec closestPoint;
    int suspended;
    Thing* notSolidTo;
};

struct Action {
    Thing* creature;
    Thing* target;
    char anim[48];
    Vec MoveToPos;
    int baseCalls;
    Vec lastClosestQuery;
    static bool Alive(const Thing* t) { return t && !t->dead; }
    void Initialise()
    {
        float forward;
        float side;
        if (Alive(target) && target->type == 13) {
            std::strcpy(anim, "STRIKE_STEALTH_KICK_THING_ON_GROUND");
            forward = 1.0f;
            side = 0.15f;
        } else {
            std::strcpy(anim, "STRIKE_HARD_LOW_KICK");
            forward = 0.0f;
            side = 0.1f;
        }
        Vec position = creature->position;
        Vec facing = creature->facing;
        Vec direction = -facing;
        if (Alive(target)) {
            if (target->type == 1)
                ++target->suspended;
            target->notSolidTo = creature;
            creature->notSolidTo = target;
            position = target->position;
            if (target->hasHitLocation) {
                lastClosestQuery = creature->position;
                position = target->closestPoint;
            }
        }
        position.Z = creature->position.Z;
        MoveToPos = position;
        direction.Z = 0.0f;
        if (direction.X * direction.X + direction.Y * direction.Y > g_epsilon * g_epsilon) {
            direction.Normalise();
            MoveToPos = direction * forward + position + Vec(direction.Y, -direction.X, 0.0f) * side;
        }
        ++baseCalls;
    }
};

static bool Near(const Vec& v, float x, float y, float z)
{
    return fabs(v.X - x) < 1e-4 && fabs(v.Y - y) < 1e-4 && fabs(v.Z - z) < 1e-4;
}
static int Fail(int n) { std::printf("0087e860_TEST FAIL %d\n", n); return 1; }

static void Make(Thing& t, int type, bool dead, float x, float y, float z)
{
    t.type = type; t.dead = dead; t.position = Vec(x, y, z); t.facing = Vec(0.0f, -1.0f, 0.0f);
    t.hasHitLocation = false; t.closestPoint = Vec(0.0f, 0.0f, 0.0f); t.suspended = 0; t.notSolidTo = 0;
}

int main()
{
    Thing kicker, target;
    Action a;
    // stealth kick on a living type-13 target; kicker faces -Y so the aim direction is +Y and the side vector +X
    Make(kicker, 1, false, 1.0f, 2.0f, 3.0f);
    Make(target, 13, false, 10.0f, 20.0f, 5.0f);
    a.creature = &kicker; a.target = &target; a.baseCalls = 0;
    a.Initialise();
    if (std::strcmp(a.anim, "STRIKE_STEALTH_KICK_THING_ON_GROUND") || a.baseCalls != 1) return Fail(1);
    if (target.notSolidTo != &kicker || kicker.notSolidTo != &target || target.suspended) return Fail(2);
    if (!Near(a.MoveToPos, 10.15f, 21.0f, 3.0f)) return Fail(3);
    // hit location: aim at the closest point, the query is the kicker's position
    target.hasHitLocation = true; target.closestPoint = Vec(7.0f, 8.0f, 9.0f);
    a.Initialise();
    if (!Near(a.MoveToPos, 7.15f, 9.0f, 3.0f) || !Near(a.lastClosestQuery, 1.0f, 2.0f, 3.0f)) return Fail(4);
    // living type-1 target: hard low kick, suspended, mutual non-solidity, no forward push
    Make(target, 1, false, 4.0f, 6.0f, 8.0f);
    kicker.notSolidTo = 0;
    a.Initialise();
    if (std::strcmp(a.anim, "STRIKE_HARD_LOW_KICK") || target.suspended != 1 || kicker.notSolidTo != &target) return Fail(5);
    if (!Near(a.MoveToPos, 4.1f, 6.0f, 3.0f)) return Fail(6);
    // dead target: hard kick from the kicker's own position, no component changes
    Make(target, 13, true, 4.0f, 6.0f, 8.0f);
    kicker.notSolidTo = 0;
    a.Initialise();
    if (std::strcmp(a.anim, "STRIKE_HARD_LOW_KICK") || target.notSolidTo || kicker.notSolidTo) return Fail(7);
    if (!Near(a.MoveToPos, 1.1f, 2.0f, 3.0f)) return Fail(8);
    // no target and a vertical facing: the flat direction is degenerate, MoveToPos is just the position
    kicker.facing = Vec(0.0f, 0.0f, 1.0f);
    a.target = 0;
    a.Initialise();
    if (!Near(a.MoveToPos, 1.0f, 2.0f, 3.0f) || a.baseCalls != 5) return Fail(9);
    // a non-unit flat facing is normalised before the push: facing (-3,0,0) -> direction (1,0,0), side vector (0,-1,0)
    kicker.facing = Vec(-3.0f, 0.0f, 0.0f);
    Make(target, 13, false, 0.0f, 0.0f, 0.0f);
    a.target = &target;
    a.Initialise();
    if (!Near(a.MoveToPos, 1.0f, -0.15f, 3.0f)) return Fail(10);
    std::printf("0087e860_TEST PASS\n");
    return 0;
}