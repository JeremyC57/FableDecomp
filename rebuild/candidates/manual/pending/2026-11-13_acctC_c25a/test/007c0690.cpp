// Behaviour test for 0x007C0690 NThingShot::CTCShotDefinitionBase::Init (target-thing overload): the target thing is remembered; the
// shot position is the shooter's targeted position when it has one AND (first-person targeting OR z-targeting with a live locked
// thing); otherwise the live, vulnerable target's targeting position; otherwise (player creature in third-person aiming mode) the shot's
// position plus the camera view direction times the maximum targeting distance (0 without a player targeting TC); otherwise 20 units
// along the shot's facing, whose pitch is moved onto the live shooter's normalised horizontal facing (rotation about (f.Y, -f.X, 0) by
// acos(clamped |facing2D|) in turns, signed by the facing's Z); a dead / absent shooter keeps the shot's facing. Then the position
// overload is called with all the other arguments unchanged.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; V() : X(0), Y(0), Z(0) {} V(float x, float y, float z) : X(x), Y(y), Z(z) {} };
static V add(const V& a, const V& b) { return V(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
static V mul(const V& a, float s) { return V(a.X * s, a.Y * s, a.Z * s); }
// Rodrigues rotation; the angle is in turns (GFArcCos returns acos / 2pi), as C3DVector::Rotate expects.
static V rotate(const V& v, const V& k, float turns) {
    float a = turns * 6.2831853f, c = (float)cos(a), s = (float)sin(a);
    V kxv(k.Y * v.Z - k.Z * v.Y, k.Z * v.X - k.X * v.Z, k.X * v.Y - k.Y * v.X); float d = k.X * v.X + k.Y * v.Y + k.Z * v.Z;
    return V(v.X * c + kxv.X * s + k.X * d * (1 - c), v.Y * c + kxv.Y * s + k.Y * d * (1 - c), v.Z * c + kxv.Z * s + k.Z * d * (1 - c));
}
struct Thing {
    bool exists, dead, hasTargeting, targetedOk, firstPerson, zTargeting, lockedAlive, playerTargeting, aiming, vulnerable; int type;
    V targetedPos, targetingPos, facing, physPos, pos; float maxDistance;
    Thing() : exists(true), dead(false), hasTargeting(false), targetedOk(false), firstPerson(false), zTargeting(false), lockedAlive(false),
              playerTargeting(false), aiming(false), vulnerable(false), type(0), maxDistance(0) {}
};
static bool alive(const Thing* t) { return t && t->exists && !t->dead; }
struct Shot {
    const Thing* targetThing; Thing* parent; V cameraDir; V initPos; int initCalls; Thing* initShooter; float initAcc;
    void InitPos(Thing* shooter, const V& p, float acc) { ++initCalls; initPos = p; initShooter = shooter; initAcc = acc; }
    void Init(Thing* shooter, const Thing* target, float acc) {
        targetThing = target;
        V target_pos; bool have = false;
        if (alive(shooter)) {
            if (shooter->hasTargeting && shooter->targetedOk) {
                target_pos = shooter->targetedPos;
                if (shooter->firstPerson) have = true;
                else if (shooter->zTargeting && shooter->lockedAlive) have = true;
            }
            if (!have) {
                if (alive(targetThing) && targetThing->vulnerable) { target_pos = targetThing->targetingPos; have = true; }
                else if (shooter->type == 2 && shooter->aiming) {
                    float distance = 0.0f; if (shooter->playerTargeting) distance = shooter->maxDistance;
                    target_pos = add(parent->pos, mul(cameraDir, distance)); have = true;
                }
            }
        }
        if (!have) {
            V fv = parent->facing, dir = fv;
            if (alive(shooter)) {
                V f = shooter->facing; f.Z = 0.0f;
                if (f.X * f.X + f.Y * f.Y > 1e-8f) {
                    float m = (float)sqrt(f.X * f.X + f.Y * f.Y); f = V(f.X / m, f.Y / m, 0.0f);
                    V axis(f.Y, -f.X, 0.0f); float sign = fv.Z > 0.0f ? 1.0f : -1.0f;
                    float c = (float)sqrt(dir.X * dir.X + dir.Y * dir.Y); if (c < -1.0f) c = -1.0f; else if (c > 1.0f) c = 1.0f;
                    float angle = (float)acos(c) * 0.15915494f; dir = rotate(f, axis, angle * sign);
                }
            }
            target_pos = add(parent->physPos, mul(dir, 20.0f));
        }
        InitPos(shooter, target_pos, acc);
    }
};
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
static bool near(const V& a, const V& b) { return fabs(a.X - b.X) < 1e-3f && fabs(a.Y - b.Y) < 1e-3f && fabs(a.Z - b.Z) < 1e-3f; }
static Shot run(Thing* shooter, const Thing* target, Thing& parent) {
    Shot s; s.parent = &parent; s.cameraDir = V(0, 1, 0); s.initCalls = 0; s.Init(shooter, target, 0.5f); return s;
}
int main() {
    Thing parent; parent.pos = V(1, 2, 3); parent.physPos = V(10, 0, 0); parent.facing = V(0.6f, 0.0f, 0.8f);
    Thing shooter; shooter.hasTargeting = true; shooter.targetedOk = true; shooter.targetedPos = V(7, 7, 7); shooter.firstPerson = true;
    Thing target; target.vulnerable = true; target.targetingPos = V(5, 5, 5);
    Shot s = run(&shooter, &target, parent);
    check(s.targetThing == &target && s.initCalls == 1 && near(s.initPos, V(7, 7, 7)) && s.initShooter == &shooter && s.initAcc == 0.5f, "first person");
    shooter.firstPerson = false; shooter.zTargeting = true; shooter.lockedAlive = true;
    check(near(run(&shooter, &target, parent).initPos, V(7, 7, 7)), "z-targeted locked");
    shooter.lockedAlive = false;
    check(near(run(&shooter, &target, parent).initPos, V(5, 5, 5)), "lock dead -> vulnerable target");
    shooter.zTargeting = false; shooter.lockedAlive = true;
    check(near(run(&shooter, &target, parent).initPos, V(5, 5, 5)), "locked but not z-targeting");
    target.vulnerable = false; shooter.type = 2; shooter.aiming = true; shooter.playerTargeting = true; shooter.maxDistance = 4.0f;
    check(near(run(&shooter, &target, parent).initPos, V(1, 6, 3)), "third-person aiming");
    shooter.playerTargeting = false;
    check(near(run(&shooter, &target, parent).initPos, V(1, 2, 3)), "aiming without player targeting");
    target.vulnerable = true; target.dead = true;
    check(near(run(&shooter, &target, parent).initPos, V(1, 2, 3)), "dead target skipped");
    shooter.aiming = false; shooter.facing = V(0.0f, 2.0f, 0.5f);
    // shot facing pitch 0.6 horizontal / 0.8 up moved onto the shooter's horizontal direction (0,1,0): (0, 0.6, 0.8) * 20 + (10,0,0)
    check(near(run(&shooter, &target, parent).initPos, V(10, 12, 16)), "pitched facing");
    parent.facing = V(0.6f, 0.0f, -0.8f);
    check(near(run(&shooter, &target, parent).initPos, V(10, 12, -16)), "pitched facing, downward sign");
    shooter.facing = V(0.0f, 0.0f, 1.0f);
    check(near(run(&shooter, &target, parent).initPos, V(22, 0, -16)), "vertical shooter facing keeps shot facing");
    shooter.dead = true; parent.facing = V(0.6f, 0.0f, 0.8f);
    check(near(run(&shooter, &target, parent).initPos, V(22, 0, 16)), "dead shooter: shot facing");
    check(near(run(0, 0, parent).initPos, V(22, 0, 16)) && run(0, 0, parent).targetThing == 0, "no shooter");
    if (fails == 0) std::printf("007c0690_TEST PASS\n");
    return fails;
}
