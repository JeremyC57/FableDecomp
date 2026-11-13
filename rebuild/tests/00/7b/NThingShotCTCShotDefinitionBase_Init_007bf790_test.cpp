#include <cstdio>
#include <cstring>
// Behaviour model of NThingShot::CTCShotDefinitionBase::Init (0x007BF790): parameter stores, KnockDown from the ability def,
// weapon augmentation copy when alive, mutual not-solid registration with a live shooter (adding CTCSolidToThing when missing).
struct Vec { float X, Y, Z; };
struct Thing {
    bool dead; bool hasAug; bool hasSolid; bool hasTrail; Vec pos;
    int augCopiedFrom; int notSolidCount; const Thing* notSolid[4]; int addTCCalls; int trailUpdates; int id;
    bool forceResend; int physicsResets;
};
struct Shot {
    Thing* parent; Thing* shooter; Thing* weapon; void* def; void* ability;
    float accuracy, damage, buildup; long startFrame; Vec target; long dieOnFrame;
    bool passed, knock, decap; Vec bestPos; int bestType; int effects, movement;
};
static long g_frame = 1234;
static void AddNotSolid(Thing* t, const Thing* other) { t->notSolid[t->notSolidCount++] = other; }
static void Init(Shot& s, void* def, Thing* shooter, Thing* weapon, const Vec& target, float acc, float dmg, float bt, void* ability)
{
    s.def = def; s.ability = ability; s.shooter = shooter; s.weapon = weapon;
    s.buildup = bt; s.damage = dmg; s.accuracy = acc;
    s.dieOnFrame = -1; s.passed = false; s.knock = false; s.decap = false;
    if (s.ability) s.knock = true;
    s.target = target;
    s.parent->physicsResets++;
    s.effects++; s.movement++;
    if (weapon && !weapon->dead && s.parent->hasAug) s.parent->augCopiedFrom = weapon->id;
    if (s.shooter && !s.shooter->dead) {
        if (s.shooter->hasSolid) AddNotSolid(s.shooter, s.parent);
        Thing* solid;
        if (s.parent->hasSolid) solid = s.parent;
        else { solid = 0; s.parent->addTCCalls++; s.parent->hasSolid = true; solid = s.parent; }
        AddNotSolid(solid, s.shooter);
    }
    s.parent->forceResend = true;
    s.bestPos = s.parent->pos; s.bestType = 0;
    if (s.parent->hasTrail) s.parent->trailUpdates++;
    s.startFrame = g_frame;
}
int main()
{
    Thing parent; std::memset(&parent, 0, sizeof parent); parent.id = 1; parent.pos.X = 5; parent.hasTrail = true;
    Thing shooter; std::memset(&shooter, 0, sizeof shooter); shooter.id = 2; shooter.hasSolid = true;
    Thing weapon; std::memset(&weapon, 0, sizeof weapon); weapon.id = 3;
    parent.hasAug = true;
    Shot s; std::memset(&s, 0, sizeof s); s.parent = &parent;
    Vec t = { 1, 2, 3 }; int abil = 0;
    Init(s, 0, &shooter, &weapon, t, 0.5f, 2.0f, 1.5f, &abil);
    bool ok = s.knock && !s.passed && !s.decap && s.dieOnFrame == -1 && s.accuracy == 0.5f && s.damage == 2.0f && s.buildup == 1.5f
        && s.target.Z == 3 && parent.augCopiedFrom == 3 && parent.addTCCalls == 1 && parent.notSolidCount == 1
        && parent.notSolid[0] == &shooter && shooter.notSolidCount == 1 && shooter.notSolid[0] == &parent && parent.forceResend
        && s.bestPos.X == 5 && parent.trailUpdates == 1 && s.startFrame == 1234 && s.effects == 1 && s.movement == 1;
    Thing p2; std::memset(&p2, 0, sizeof p2); Shot s2; std::memset(&s2, 0, sizeof s2); s2.parent = &p2;
    shooter.dead = true; weapon.dead = true;
    Init(s2, 0, &shooter, &weapon, t, 1, 1, 1, 0);
    ok = ok && !s2.knock && p2.addTCCalls == 0 && p2.augCopiedFrom == 0 && p2.trailUpdates == 0 && p2.notSolidCount == 0;
    if (!ok) { std::printf("007bf790_TEST FAIL\n"); return 1; }
    std::printf("007bf790_TEST PASS\n");
    return 0;
}