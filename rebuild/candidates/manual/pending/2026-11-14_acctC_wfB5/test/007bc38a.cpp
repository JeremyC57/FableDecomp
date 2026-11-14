// Behaviour test for 0x007BC38A NThingShot::CTCShotDefinitionThrowPredictive::InitMovement: the aim point is the shot's shoot-at
// position; with a live shooter it is pushed out to 5 units from the shooter when it is horizontally closer than that (and stored as
// the target position), else with a live target thing it is led by the target's last-frame motion * (distance / speed * 3) * accuracy
// percent (and stored). The shot then gets gravity and, when a solution exists, the velocity of a 45-degree lob:
// 1/v^2 = (d / sin * cos - h) * 2 sin^2 / (d^2 * 0.12), velocity = (horizontal direction * sin + up * cos) / sqrt(1/v^2).
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; V() : X(0), Y(0), Z(0) {} V(float x, float y, float z) : X(x), Y(y), Z(z) {} };
static V sub(const V& a, const V& b) { return V(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
static V add(const V& a, const V& b) { return V(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
static V mul(const V& a, float s) { return V(a.X * s, a.Y * s, a.Z * s); }
static float mag(const V& a) { return (float)sqrt(a.X * a.X + a.Y * a.Y + a.Z * a.Z); }
static float mag2d(const V& a) { return (float)sqrt(a.X * a.X + a.Y * a.Y); }
struct Thing { bool exists, dead; V pos, lastPos; Thing() : exists(true), dead(false) {} };
static bool alive(const Thing* t) { return t && t->exists && !t->dead; }
static const float ANGLE = 0.7853982f;
struct Shot {
    Thing* shooter; Thing* targetThing; V shootAt, targetPos, parentPos, velocity; float speed; int accuracy; bool gravity, velocitySet;
    void InitMovement() {
        V target = shootAt;
        if (alive(shooter)) {
            V from_shooter = sub(target, shooter->pos);
            if (mag2d(from_shooter) < 5.0f) {
                V offset = mul(from_shooter, 1.0f / mag(from_shooter));
                target = add(shooter->pos, mul(offset, 5.0f));
                targetPos = target;
            } else if (alive(targetThing)) {
                V motion = sub(targetThing->pos, targetThing->lastPos);
                float time = mag(sub(targetThing->pos, shooter->pos)) / speed * 3.0f;
                target = add(target, mul(mul(motion, time), accuracy * 0.01f));
                targetPos = target;
            }
        }
        V direction = sub(target, parentPos);
        float height = direction.Z; direction.Z = 0.0f;
        float distance = mag2d(direction);
        direction = mul(direction, 1.0f / distance);
        double s = sin((double)ANGLE), c = cos((double)ANGLE);
        float one_over = (float)((distance / s * c - height) * (2.0f * s * s) / (distance * distance * 0.12f));
        gravity = true;
        if (one_over > 0.0001f) {
            float v = 1.0f / (float)sqrt(one_over);
            velocity = mul(add(mul(direction, (float)s), mul(V(0, 0, 1), (float)c)), v);
            velocitySet = true;
        }
    }
};
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
static bool near(const V& a, const V& b) { return fabs(a.X - b.X) < 1e-3f && fabs(a.Y - b.Y) < 1e-3f && fabs(a.Z - b.Z) < 1e-3f; }
static Shot make(Thing* shooter, Thing* target, const V& shootAt) {
    Shot s; s.shooter = shooter; s.targetThing = target; s.shootAt = shootAt; s.targetPos = V(-1, -1, -1); s.parentPos = V(0, 0, 0);
    s.speed = 2.0f; s.accuracy = 50; s.gravity = false; s.velocitySet = false; return s;
}
// Simulate the lob under the game's gravity (0.06 per frame squared, from the 0.12 in the formula) and report where it crosses h.
static V land(const V& v, float h) {
    // z(t) = v.Z t - 0.06 t^2 = h, the later root
    float disc = v.Z * v.Z - 4.0f * 0.06f * h; float t = (v.Z + (float)sqrt(disc)) / (2.0f * 0.06f);
    return V(v.X * t, v.Y * t, v.Z * t - 0.06f * t * t);
}
int main() {
    Thing shooter; shooter.pos = V(0, 0, 0);
    Thing target; target.pos = V(30, 40, 0); target.lastPos = V(29, 40, 0);
    // no shooter: plain lob at the shoot-at position, target position untouched
    Shot s = make(0, &target, V(30, 40, 0)); s.InitMovement();
    check(s.gravity && s.velocitySet && near(s.targetPos, V(-1, -1, -1)), "no shooter: no lead, gravity on");
    check(near(land(s.velocity, 0.0f), V(30, 40, 0)), "lob lands on the target");
    check(fabs(s.velocity.Z - mag2d(s.velocity)) < 1e-3f, "45 degree launch");
    // dead shooter behaves the same
    shooter.dead = true; s = make(&shooter, &target, V(30, 40, 0)); s.InitMovement();
    check(near(s.targetPos, V(-1, -1, -1)), "dead shooter: no lead");
    shooter.dead = false;
    // close target: pushed out to 5 units from the shooter along the 3D offset
    s = make(&shooter, &target, V(3, 0, 4)); s.InitMovement();
    check(near(s.targetPos, V(3, 0, 4)), "close target moved to 5 units (already 5 in 3D, 3 in 2D)");
    s = make(&shooter, &target, V(1, 0, 0)); s.InitMovement();
    check(near(s.targetPos, V(5, 0, 0)), "close target pushed out");
    check(near(land(s.velocity, 0.0f), V(5, 0, 0)), "close lob lands on the pushed-out point");
    // far target with a live target thing: led by motion * (50 / 2 * 3) * 0.5 = (1,0,0) * 75 * 0.5
    s = make(&shooter, &target, V(30, 40, 0)); s.InitMovement();
    check(near(s.targetPos, V(67.5f, 40, 0)), "moving target led");
    check(near(land(s.velocity, 0.0f), V(67.5f, 40, 0)), "led lob lands on the led point");
    s = make(&shooter, &target, V(30, 40, 0)); s.accuracy = 0; s.InitMovement();
    check(near(s.targetPos, V(30, 40, 0)), "zero accuracy: no lead but stored");
    // far target, dead target thing: no lead, not stored
    target.dead = true; s = make(&shooter, &target, V(30, 40, 0)); s.InitMovement();
    check(near(s.targetPos, V(-1, -1, -1)) && s.velocitySet, "dead target thing: no lead");
    // raised target: still reached
    s = make(0, 0, V(30, 40, 10)); s.InitMovement();
    check(near(land(s.velocity, 10.0f), V(30, 40, 10)), "raised target reached");
    // target above the 45 degree line: no solution, gravity only
    s = make(0, 0, V(3, 4, 6)); s.InitMovement();
    check(s.gravity && !s.velocitySet, "no solution above the launch line");
    if (fails == 0) std::printf("007bc38a_TEST PASS\n");
    return fails;
}
