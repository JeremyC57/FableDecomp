#include <cstdio>
#include <cstring>
#include <math.h>
// Behaviour test for CCombatAction_KickThingOnGround::PerformAction @ 0x0087F3A0 (model of the matched source; shape of the landed
// sibling test CCombatActionKickThingOnGround_Initialise_0087e860_test.cpp). The kick lands only when the kicker stands within 0.2
// (flat) and 1.0 (height) of MoveToPos and the target is alive. Then: a CTCEntityEvents target is told it was kicked; a kickable
// def with a shot marks the kick as a launch (a creature target also records the attacker); otherwise a creature target is hit
// (unarmed melee damage 1.0) or a dead creature (type 13) plays RESPONSE_GET_KICKED_ON_GROUND with its own filter when it has the
// anim, else the no-group filter; a type-5 object resets its die-off timer. Without a kickable def the action ends. A launch adds
// the def's shot TC, scales the shot speed by the hero's strength and running speed toward the target (not for a child), inits
// the shot at target + facing * random * scale, adds creature mode 0x39, sets a CTCRotate spinning and spawns the shot's effect
// 0.3 above the target. Finally the kick's opinion deed is posted when the def names one and has no faction.
struct Vec {
    float X, Y, Z;
    Vec() : X(0.0f), Y(0.0f), Z(0.0f) { }
    Vec(float x, float y, float z) : X(x), Y(y), Z(z) { }
    Vec operator-(const Vec& o) const { return Vec(X - o.X, Y - o.Y, Z - o.Z); }
    Vec operator+(const Vec& o) const { return Vec(X + o.X, Y + o.Y, Z + o.Z); }
    Vec operator*(float f) const { return Vec(X * f, Y * f, Z * f); }
    float Dot(const Vec& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
    float GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    void Normalise() { float r = 1.0f / (float)sqrt(X * X + Y * Y + Z * Z); X *= r; Y *= r; Z *= r; }
};

struct KickableDef {
    float BaseDistanceToTravel, StrengthScaleRange, SpeedScaleRange, BaseSpeedScaleRange, RandomBase, RandomRange;
    long ShotToAttach, OpinionDeedType, OpinionDeedFaction, OpinionDeedTypeToEnemies;
};
struct ShotDef { long PrimaryEffect; const char* ClassName; };

struct Thing {
    int type;                 // CThing type byte +0x10
    int typeFlags;            // +0x6C, bit 8 = creature
    bool dead;
    Vec pos;
    Vec facing;               // physics GetFacingVector
    bool hasEntityEvents;     // TC 0x42
    KickableDef* kickable;    // GetSubDef<CKickableDef> result (0: lookup fails)
    bool hasAppearance;
    bool animExistsNoGroup;   // AnimExists(name, CAnimFilterNoGroup())
    bool animExistsOwn;       // AnimExists(name, GetAnimationFilter())
    bool hasAnimatingTC;      // TC 0x83
    bool hasModeManager;      // TC 0x31
    bool rotateAddFails;      // AddTC("CTCRotate") returns 0
    bool hasExperience;       // TC 0x68
    bool isChild;
    long statLevel[4], statMax[4];
    bool hasDeedLog;          // TC 0x51
};

struct Log {
    char calls[1024];
    int hitAttribute; float hitDamage; const Thing* hitBy;
    const Thing* attacker;
    int playedBank; int animFilterOwn;
    unsigned long* seedSeen;
    Vec initStart; float initAccuracy, initMult, initBuildup; const Thing* initShooter; long initDefId;
    long modeAdded; bool rotating; Vec rotAxis; float rotSpeed, rotCount;
    long effect; Vec effectPos;
    long deedType; long deedPostType; const Thing* deedTarget; Vec deedPos;
    void Reset() { std::memset(this, 0, sizeof(*this)); }
    void Add(const char* s) { std::strcat(calls, s); std::strcat(calls, ";"); }
};

struct World { unsigned long seed; ShotDef* shot; long shotId; };

static Log g_log;

// Stubs for the out-of-line callees (each records its call).
static float GFFloatRandom(float range, unsigned long* seed) { g_log.seedSeen = seed; return range * 0.25f; }   // 0x004978A0

struct Action {
    Thing* PCreature;
    Thing* PTarget;
    Vec MoveToPos;
    Vec Movement;
    float MaxRunningSpeed;
    World* world;

    void PerformAction(int mutation)
    {
        Vec flat = PCreature->pos - MoveToPos;
        float d2 = mutation == 1 ? flat.X * flat.X + flat.Y * flat.Y + flat.Z * flat.Z : flat.X * flat.X + flat.Y * flat.Y;
        if (d2 > 0.04f)
            return;
        if (mutation != 2 && fabs(PCreature->pos.Z - MoveToPos.Z) > 1.0f)
            return;
        if (PTarget == 0 || (PTarget->dead && mutation != 3))
            return;
        if (PTarget->hasEntityEvents)
            g_log.Add("AddEvent(Kicked)");
        bool is_kickable = false;
        KickableDef* kickable_def = PTarget->kickable;
        if (kickable_def != 0 && (kickable_def->ShotToAttach != 0 || mutation == 4)) {
            is_kickable = true;
            if (PTarget->typeFlags & 8) { g_log.Add("SetAttackedByCreature"); g_log.attacker = PCreature; }
        } else if (PTarget->typeFlags & 8) {
            g_log.Add("CHitParameters(0,creature,0)");
            g_log.hitBy = PCreature;
            g_log.hitAttribute = mutation == 5 ? 0 : 1;
            g_log.hitDamage = 1.0f;
            g_log.Add("OnHit");
            g_log.Add("~CHitParameters");
        } else if (PTarget->type == (mutation == 6 ? 5 : 13)) {
            if (PTarget->hasAppearance && PTarget->animExistsNoGroup) {
                bool own = PTarget->animExistsOwn && mutation != 7;
                g_log.animFilterOwn = own ? 1 : 0;
                g_log.seedSeen = &world->seed;
                g_log.Add("GetRandomAnim(RESPONSE_GET_KICKED_ON_GROUND)");
                if (!PTarget->hasAnimatingTC)
                    g_log.Add("AddTC(CTCAnimatingObject,0)");
                g_log.playedBank = own ? 22 : 11;   // bank index of the picked entry
                g_log.Add("PlayAnimation(bank,0,0)");
                g_log.Add("SetAsRenderedThisFrame(2)");
            }
        }
        if (PTarget->type == 5 && mutation != 8)
            g_log.Add("ResetDieOffTimer");
        if (kickable_def == 0)
            return;
        if (is_kickable) {
            g_log.initDefId = kickable_def->ShotToAttach;
            ShotDef* shot_def = world->shot;
            g_log.Add("AddTC(shot,1)");
            Vec facing = PCreature->facing;
            float strength_scale = 0.0f, speed_scale = 0.0f, base_speed_scale = 0.0f;
            if (PCreature->hasExperience && (!PCreature->isChild || mutation == 15)) {
                strength_scale = (float)PCreature->statLevel[0] /
                                 ((float)PCreature->statMax[0] + (mutation == 11 ? 1.0f : 0.0f));
                Vec movement = Movement;
                Vec direction = PTarget->pos - PCreature->pos;
                direction.Normalise();
                movement = direction * direction.Dot(movement);
                base_speed_scale = speed_scale = movement.GetMagnitude() / MaxRunningSpeed;
                if (mutation != 10)
                    speed_scale *= ((float)PCreature->statLevel[3] + 1.0f) / ((float)PCreature->statMax[3] + 1.0f);
            }
            Vec velocity = facing * (GFFloatRandom(kickable_def->RandomRange, &world->seed) + kickable_def->RandomBase) *
                (kickable_def->BaseSpeedScaleRange * base_speed_scale + kickable_def->SpeedScaleRange * speed_scale +
                 kickable_def->StrengthScaleRange * strength_scale + kickable_def->BaseDistanceToTravel);
            g_log.initStart = velocity + PTarget->pos;
            g_log.initShooter = PCreature;
            g_log.initAccuracy = 50.0f; g_log.initMult = 1.0f; g_log.initBuildup = 0.0f;
            g_log.Add("Init");
            if (PTarget->hasModeManager || mutation == 12) { g_log.modeAdded = 0x39; g_log.Add("AddMode"); }
            g_log.Add("AddTC(CTCRotate,0)");
            if (!PTarget->rotateAddFails) {
                g_log.rotating = true;
                g_log.rotAxis = Vec(0.0f, 0.0f, 1.0f); g_log.rotSpeed = 0.15f; g_log.rotCount = 5.0f;
                g_log.Add("SetRotationInfo");
            }
            g_log.effect = shot_def->PrimaryEffect;
            g_log.effectPos = PTarget->pos + Vec(0.0f, 0.0f, mutation == 13 ? 0.0f : 0.3f);
            g_log.Add("CTCDParticleEmitter::Create");
        }
        bool any = mutation == 14 ? (kickable_def->OpinionDeedType != 0 && kickable_def->OpinionDeedTypeToEnemies != 0)
                                  : (kickable_def->OpinionDeedType != 0 || kickable_def->OpinionDeedTypeToEnemies != 0);
        if (any && PCreature->hasDeedLog) {
            if ((kickable_def->OpinionDeedFaction == 0 || mutation == 9) && kickable_def->OpinionDeedType != 0) {
                g_log.deedType = kickable_def->OpinionDeedType; g_log.deedPostType = 7;   // OPINION_POST_DEED_TYPE_ALL
                g_log.deedTarget = PTarget; g_log.deedPos = PCreature->pos;
                g_log.Add("PostDeed");
            }
        }
    }
};

static bool Near(float a, float b) { return fabs(a - b) < 1e-3; }
static bool NearV(const Vec& v, float x, float y, float z) { return Near(v.X, x) && Near(v.Y, y) && Near(v.Z, z); }
static bool Has(const char* s) { return std::strstr(g_log.calls, s) != 0; }

static void MakeThing(Thing& t, int type, int flags, float x, float y, float z)
{
    std::memset(&t, 0, sizeof(t));
    t.type = type; t.typeFlags = flags; t.pos = Vec(x, y, z); t.facing = Vec(1.0f, 0.0f, 0.0f);
}

static void MakeDef(KickableDef& d, long shot)
{
    d.BaseDistanceToTravel = 1.0f; d.StrengthScaleRange = 2.0f; d.SpeedScaleRange = 4.0f; d.BaseSpeedScaleRange = 8.0f;
    d.RandomBase = 0.5f; d.RandomRange = 1.0f; d.ShotToAttach = shot;
    d.OpinionDeedType = 0; d.OpinionDeedFaction = 0; d.OpinionDeedTypeToEnemies = 0;
}

// Runs every scenario on the model with the given mutation; returns 0 when all checks hold, else the failing check number.
static int Scenarios(int m)
{
    World world; ShotDef shot; shot.PrimaryEffect = 77; shot.ClassName = "NThingShot::CTCShotKickedThing";
    world.seed = 1234; world.shot = &shot; world.shotId = 9;
    Thing kicker, target; KickableDef def;
    Action a;
    a.PCreature = &kicker; a.PTarget = &target; a.world = &world;
    a.MoveToPos = Vec(0.1f, 0.1f, 0.5f); a.Movement = Vec(2.0f, 1.0f, 7.0f); a.MaxRunningSpeed = 4.0f;

    // 1. too far on the flat (0.3 > 0.2): nothing happens
    MakeThing(kicker, 1, 8, 0.4f, 0.1f, 0.5f); MakeThing(target, 13, 8, 3.0f, 4.0f, 0.0f); target.hasEntityEvents = true;
    g_log.Reset(); a.PerformAction(m);
    if (g_log.calls[0]) return 1;
    // 2. flat distance fine but a big height difference only: 3D distance would refuse, the flat test must accept
    MakeThing(kicker, 1, 8, 0.0f, 0.0f, 1.2f); MakeThing(target, 1, 8, 3.0f, 4.0f, 0.0f); target.hasEntityEvents = true;
    g_log.Reset(); a.PerformAction(m);
    if (!Has("AddEvent(Kicked)")) return 2;
    // 3. height difference 1.6 > 1.0: refused
    MakeThing(kicker, 1, 8, 0.0f, 0.0f, 2.1f); MakeThing(target, 1, 8, 3.0f, 4.0f, 0.0f); target.hasEntityEvents = true;
    g_log.Reset(); a.PerformAction(m);
    if (g_log.calls[0]) return 3;
    // 4. dead target and missing target: refused
    MakeThing(kicker, 1, 8, 0.0f, 0.0f, 0.0f); MakeThing(target, 1, 8, 3.0f, 4.0f, 0.0f); target.hasEntityEvents = true;
    target.dead = true;
    g_log.Reset(); a.PerformAction(m);
    if (g_log.calls[0]) return 4;
    a.PTarget = 0; g_log.Reset(); a.PerformAction(m); a.PTarget = &target;
    if (g_log.calls[0]) return 5;
    // 5. living creature, no kickable def: entity event, unarmed hit by the kicker, then the action ends (no deed)
    MakeThing(target, 1, 8, 3.0f, 4.0f, 0.0f); target.hasEntityEvents = true; kicker.hasDeedLog = true;
    g_log.Reset(); a.PerformAction(m);
    if (std::strcmp(g_log.calls, "AddEvent(Kicked);CHitParameters(0,creature,0);OnHit;~CHitParameters;")) return 6;
    if (g_log.hitAttribute != 1 || !Near(g_log.hitDamage, 1.0f) || g_log.hitBy != &kicker) return 7;
    // 6. kickable def without a shot on a creature: still the hit arm, then the deed is posted (faction 0)
    MakeDef(def, 0); def.OpinionDeedType = 12;
    MakeThing(target, 1, 8, 3.0f, 4.0f, 0.0f); target.kickable = &def;
    g_log.Reset(); a.PerformAction(m);
    if (std::strcmp(g_log.calls, "CHitParameters(0,creature,0);OnHit;~CHitParameters;PostDeed;")) return 8;
    if (g_log.deedType != 12 || g_log.deedPostType != 7 || g_log.deedTarget != &target || !NearV(g_log.deedPos, 0, 0, 0)) return 9;
    // 7. deed only to enemies: the || test passes but the deed type is 0, nothing posted; a faction blocks the post
    def.OpinionDeedType = 0; def.OpinionDeedTypeToEnemies = 5;
    g_log.Reset(); a.PerformAction(m);
    if (Has("PostDeed")) return 10;
    def.OpinionDeedType = 12; def.OpinionDeedFaction = 3;
    g_log.Reset(); a.PerformAction(m);
    if (Has("PostDeed")) return 11;
    def.OpinionDeedFaction = 0; kicker.hasDeedLog = false;
    g_log.Reset(); a.PerformAction(m);
    if (Has("PostDeed")) return 12;
    // 8. dead-creature corpse (type 13, not a creature): anim with its own filter, animating TC added
    MakeThing(target, 13, 0, 3.0f, 4.0f, 0.0f); target.hasAppearance = true; target.animExistsNoGroup = true;
    target.animExistsOwn = true;
    g_log.Reset(); a.PerformAction(m);
    if (std::strcmp(g_log.calls, "GetRandomAnim(RESPONSE_GET_KICKED_ON_GROUND);AddTC(CTCAnimatingObject,0);"
                                 "PlayAnimation(bank,0,0);SetAsRenderedThisFrame(2);")) return 13;
    if (g_log.animFilterOwn != 1 || g_log.playedBank != 22 || g_log.seedSeen != &world.seed) return 14;
    // 9. corpse without its own-filter anim and with an animating TC: no-group filter, no AddTC
    target.animExistsOwn = false; target.hasAnimatingTC = true;
    g_log.Reset(); a.PerformAction(m);
    if (std::strcmp(g_log.calls, "GetRandomAnim(RESPONSE_GET_KICKED_ON_GROUND);PlayAnimation(bank,0,0);SetAsRenderedThisFrame(2);"))
        return 15;
    if (g_log.animFilterOwn != 0 || g_log.playedBank != 11) return 16;
    // 10. corpse without the anim at all: nothing
    target.animExistsNoGroup = false;
    g_log.Reset(); a.PerformAction(m);
    if (g_log.calls[0]) return 17;
    // 11. type-5 object, no def: die-off timer reset only
    MakeThing(target, 5, 0, 3.0f, 4.0f, 0.0f);
    g_log.Reset(); a.PerformAction(m);
    if (std::strcmp(g_log.calls, "ResetDieOffTimer;")) return 18;
    // 12. launch: hero kicker (strength 2/4, speed skill 1/3) kicks a kickable creature with a mode manager
    MakeDef(def, 9); def.OpinionDeedType = 4;
    MakeThing(target, 1, 8, 3.0f, 4.0f, 0.0f); target.kickable = &def; target.hasModeManager = true; target.hasEntityEvents = true;
    kicker.hasExperience = true; kicker.statLevel[0] = 2; kicker.statMax[0] = 4; kicker.statLevel[3] = 1; kicker.statMax[3] = 3;
    kicker.hasDeedLog = true;
    g_log.Reset(); a.PerformAction(m);
    if (std::strcmp(g_log.calls, "AddEvent(Kicked);SetAttackedByCreature;AddTC(shot,1);Init;AddMode;AddTC(CTCRotate,0);"
                                 "SetRotationInfo;CTCDParticleEmitter::Create;PostDeed;")) return 19;
    if (g_log.attacker != &kicker || g_log.initShooter != &kicker || g_log.initDefId != 9) return 20;
    // direction (0.6, 0.8, 0), movement . direction = 2 -> base 0.5, speed 0.5 * (2/4) = 0.25, strength 0.5;
    // scale 8*0.5 + 4*0.25 + 2*0.5 + 1 = 7, random 0.25 + 0.5 -> velocity (5.25, 0, 0)
    if (!NearV(g_log.initStart, 8.25f, 4.0f, 0.0f)) return 21;
    if (!Near(g_log.initAccuracy, 50.0f) || !Near(g_log.initMult, 1.0f) || !Near(g_log.initBuildup, 0.0f)) return 22;
    if (g_log.modeAdded != 0x39 || !g_log.rotating || !NearV(g_log.rotAxis, 0, 0, 1) || !Near(g_log.rotSpeed, 0.15f) ||
        !Near(g_log.rotCount, 5.0f)) return 23;
    if (g_log.effect != 77 || !NearV(g_log.effectPos, 3.0f, 4.0f, 0.3f) || g_log.seedSeen != &world.seed) return 24;
    // 13. child kicker: no hero scaling; non-creature kickable (no attacker), no mode manager, CTCRotate refused
    kicker.isChild = true; target.typeFlags = 0; target.hasModeManager = false; target.rotateAddFails = true;
    target.hasEntityEvents = false; def.OpinionDeedType = 0;
    g_log.Reset(); a.PerformAction(m);
    if (std::strcmp(g_log.calls, "AddTC(shot,1);Init;AddTC(CTCRotate,0);CTCDParticleEmitter::Create;")) return 25;
    if (!NearV(g_log.initStart, 3.75f, 4.0f, 0.0f) || g_log.rotating) return 26;
    // 14. no experience TC: same as the child
    kicker.isChild = false; kicker.hasExperience = false;
    g_log.Reset(); a.PerformAction(m);
    if (!NearV(g_log.initStart, 3.75f, 4.0f, 0.0f)) return 27;
    // 15. kickable type-5 object: the launch and the die-off reset both run
    target.type = 5;
    g_log.Reset(); a.PerformAction(m);
    if (std::strcmp(g_log.calls, "ResetDieOffTimer;AddTC(shot,1);Init;AddTC(CTCRotate,0);CTCDParticleEmitter::Create;")) return 28;
    return 0;
}

int main()
{
    int r = Scenarios(0);
    if (r) { std::printf("0087f3a0_TEST FAIL %d\n", r); return 1; }
    // Mutation checks: each single-point change of the model must break at least one scenario.
    for (int m = 1; m <= 15; ++m) {
        if (Scenarios(m) == 0) { std::printf("0087f3a0_TEST FAIL mutation %d survived\n", m); return 1; }
    }
    std::printf("0087f3a0_TEST PASS\n");
    return 0;
}
