#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#include "engine/C3DVector.h"
#include "engine/GFRandom.h"
#include <cstdio>
#include <cstring>

static unsigned failures;
#define CHECK(expr) do { if (!(expr)) { \
    if (failures < 20) std::printf("line %d: %s\n", __LINE__, #expr); ++failures; \
} } while (0)

long FABLE_FASTCALL GFROR13(unsigned long value)
{
    return (value >> 13) | (value << 19);
}

struct BattleDef {
    float ScreamInAirTime, ScaleDownTime, DeathScreamTime;
};
static unsigned heldDefs, releases;
template<class T> struct DefPointer {
    const T* value;
    DefPointer() : value(0) {}
    ~DefPointer() { CHECK(value && heldDefs == 1); --heldDefs; ++releases; }
    const T* operator->() const { return value; }
};
struct BaseDef {
    BattleDef battle;
    bool GetSubDef(DefPointer<const BattleDef>* out) const
    {
        CHECK(out->value == 0);
        out->value = &battle;
        ++heldDefs;
        return true;
    }
};

struct Thing {
    bool dead;
    unsigned kills;
    C3DVector position;
    Thing() : dead(false), kills(0) {}
    void Kill(bool now) { CHECK(now && !dead); dead = true; ++kills; }
    const C3DVector& GetPos() const { return position; }
};
struct Pointer {
    Thing* value;
    Thing* operator->() const { return value; }
};
static bool IS_THING_ALIVE(const Pointer& pointer) { return pointer.value && !pointer.value->dead; }

struct Physics {
    unsigned facings, teleports, enables;
    Physics() : facings(0), teleports(0), enables(0) {}
    void SetFacingAngleXY(float angle) { CHECK(angle == 0.0f); ++facings; }
    void EnableGravity(bool enabled) { CHECK(enabled); ++enables; }
    void ResetPosition(const C3DVector& pos)
    {
        CHECK(pos.X == 5.0f && pos.Y == -2.0f && pos.Z == 8.0f);
        ++teleports;
    }
};
struct Appearance {
    float scale;
    unsigned scaleReads, scaleWrites;
    Appearance() : scale(1.2f), scaleReads(0), scaleWrites(0) {}
    float GetScale() { ++scaleReads; return scale; }
    void SetScale(float value) { scale = value; ++scaleWrites; }
};
struct World {
    long frame;
    unsigned long seed;
    unsigned requests;
    World() : frame(100), seed(0), requests(0) {}
    long GetFrame() const { return frame; }
    unsigned long* GetWorldSeed() { ++requests; return &seed; }
};
struct CMainGameComponent {
    static long GetConstantFPS() { return 2; }
};

struct Boss {
    unsigned phases, readyChecks;
    int phase;
    long phaseFrame, delay;
    bool revokeAfterReady;
    World* world;
    bool readySeen;

    Boss() : phases(0), readyChecks(0), phase(0), phaseFrame(0), delay(0), revokeAfterReady(false),
        world(0), readySeen(false) {}
    void SetNextBossPhase(long value, bool immediate)
    {
        CHECK(!immediate && ((phases == 0 && value == 5) || (phases == 1 && value == 6)));
        ++phases;
        phase = value;
        phaseFrame = world->frame;
    }
    bool IsReadyForPhaseChange()
    {
        CHECK(phase == 5);
        ++readyChecks;
        if (revokeAfterReady && readySeen)
            return false;
        readySeen = world->frame - phaseFrame >= delay;
        return readySeen;
    }
};
struct Creature {
    BaseDef def;
    Physics physics;
    Appearance appearance;
    Boss* boss;
    unsigned bossLookups;

    Creature() : boss(0), bossLookups(0) {}
    bool GetTC(Boss** out) { ++bossLookups; *out = boss; return true; }
    const BaseDef& PeekBaseDef() const { return def; }
    Physics* GetPhysicsTC() { return &physics; }
    Appearance* GetTCGraphicAppearance() { return &appearance; }
};

struct Scream {
    Creature& creature;
    explicit Scream(Creature& who) : creature(who) {}
};
struct Fall {
    Creature& creature;
    explicit Fall(Creature& who) : creature(who) {}
};
struct DeathInto {
    Creature& creature;
    explicit DeathInto(Creature& who) : creature(who) {}
};
struct PlayAnimation {
    Creature& creature;
    PlayAnimation(Creature& who, const char* name, bool move) : creature(who)
    {
        CHECK(std::strcmp(name, "STUMBLE") == 0 && move);
    }
};

struct Stage {
    Pointer PSwordEmitter, PTargetThing, PArenaCentre;
    Creature creature;
    Boss boss;
    World world;
    unsigned stops, checks, idle, screams, falls, deaths, animations, failCheck;
    bool stopAtCheck;

    Stage() : stops(0), checks(0), idle(0), screams(0), falls(0), deaths(0), animations(0), failCheck(0),
        stopAtCheck(false)
    {
        PSwordEmitter.value = PTargetThing.value = PArenaCentre.value = 0;
        boss.world = &world;
        creature.boss = &boss;
    }
    Creature& GetCreature() { return creature; }
    World& GetWorld() { return world; }
    bool IsStopping() { ++checks; stopAtCheck = stopAtCheck || (failCheck != 0 && checks == failCheck); return stopAtCheck; }
    void Terminate() { CHECK(heldDefs == 1); ++stops; }
    void AIAction_DoNothing() { ++idle; ++world.frame; CHECK(idle < 100); }
    void AIAction_PerformActionAndWait(const Scream& a, bool wait)
    {
        CHECK(&a.creature == &creature && !wait && creature.physics.facings == 1 && boss.phase == 0);
        ++screams;
    }
    void AIAction_PerformActionAndWait(const Fall& a, bool wait)
    {
        CHECK(&a.creature == &creature && !wait && creature.physics.enables == 1 && boss.phase == 0);
        ++falls;
    }
    void AIAction_PerformActionAndWait(const DeathInto& a, bool wait)
    {
        CHECK(&a.creature == &creature && !wait && creature.physics.teleports == 1 && boss.phase == 6);
        ++deaths;
    }
    void AIAction_PerformAction(const PlayAnimation& a) { CHECK(&a.creature == &creature && boss.phase == 5); ++animations; }
    bool IsStillRunnable();
    void Update();
};

#define CAIStateGroup_JackOfBladesBase Stage
#define CAIStateGroup_JackOfBladesStage3 Stage
#define CTCBoss Boss
#define CDefPointer DefPointer
#define CJackOfBladesBattleDef BattleDef
#define CCreatureAction_JackScream Scream
#define CCreatureAction_JackFall Fall
#define CCreatureAction_JackDeathInto DeathInto
#define CCreatureAction_PlayCombatAnimation PlayAnimation

// Original same-TU helper in FableWin 0x02BE5100 and retail 0x00971B00.
inline bool CAIStateGroup_JackOfBladesBase::IsStillRunnable()
{
    if (IS_THING_ALIVE(PTargetThing) && !IsStopping() && IS_THING_ALIVE(PArenaCentre))
        return true;
    return false;
}

void CAIStateGroup_JackOfBladesStage3::Update()
{
    CTCBoss* ptc_boss;
    GetCreature().GetTC(&ptc_boss);

    CDefPointer<const CJackOfBladesBattleDef> p_jack_def;
    GetCreature().PeekBaseDef().GetSubDef(&p_jack_def);

    if (IS_THING_ALIVE(PSwordEmitter))
        PSwordEmitter->Kill(true);

    GetCreature().GetPhysicsTC()->SetFacingAngleXY(0.0f);
    AIAction_PerformActionAndWait(CCreatureAction_JackScream(GetCreature()), false);

    long start_time = GetWorld().GetFrame();
    long timer;
    do {
        if (!IsStillRunnable()) {
            Terminate();
            return;
        }
        AIAction_DoNothing();
        timer = GetWorld().GetFrame() - start_time;
    } while (timer < p_jack_def->ScreamInAirTime * CMainGameComponent::GetConstantFPS());

    float current_scale = GetCreature().GetTCGraphicAppearance()->GetScale();
    float normal_scale = 1.2f;
    float scale_delta = (normal_scale - current_scale) /
        (CMainGameComponent::GetConstantFPS() * p_jack_def->ScaleDownTime);
    while (current_scale > normal_scale && IsStillRunnable()) {
        current_scale = GetCreature().GetTCGraphicAppearance()->GetScale();
        current_scale += scale_delta;
        GetCreature().GetTCGraphicAppearance()->SetScale(current_scale);
        AIAction_DoNothing();
    }
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }

    GetCreature().GetTCGraphicAppearance()->SetScale(normal_scale);
    GetCreature().GetPhysicsTC()->EnableGravity(true);
    AIAction_PerformActionAndWait(CCreatureAction_JackFall(GetCreature()), false);
    ptc_boss->SetNextBossPhase(5, false);

    if (!IsStillRunnable()) {
        Terminate();
        return;
    }

    start_time = GetWorld().GetFrame();
    while (IsStillRunnable() && !ptc_boss->IsReadyForPhaseChange()) {
        AIAction_DoNothing();
        timer = GetWorld().GetFrame() - start_time;
        if (timer / CMainGameComponent::GetConstantFPS() > 3) {
            if (GFRandom(100, GetWorld().GetWorldSeed()) < 33) {
                AIAction_PerformAction(CCreatureAction_PlayCombatAnimation(GetCreature(), "STUMBLE", true));
                start_time = GetWorld().GetFrame();
            }
        }
    }

    if (!IsStillRunnable()) {
        Terminate();
        return;
    }

    if (ptc_boss->IsReadyForPhaseChange()) {
        ptc_boss->SetNextBossPhase(6, false);
        if (!IsStillRunnable()) {
            Terminate();
            return;
        }

        GetCreature().GetPhysicsTC()->SetFacingAngleXY(0.0f);
        GetCreature().GetPhysicsTC()->ResetPosition(PArenaCentre->GetPos());
        AIAction_PerformActionAndWait(CCreatureAction_JackDeathInto(GetCreature()), false);
        if (!IsStillRunnable()) {
            Terminate();
            return;
        }

        start_time = GetWorld().GetFrame();
        do {
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
            AIAction_DoNothing();
            timer = GetWorld().GetFrame() - start_time;
        } while (timer < p_jack_def->DeathScreamTime * CMainGameComponent::GetConstantFPS());
    }
    Terminate();
}

static void Run(Stage& state, int swordState, float scale, long delay, unsigned long seed, bool readyRevoked, unsigned failedCheck,
                int targetStatus = 0, int arenaStatus = 0)
{
    Thing sword, target, arena;
    arena.position = C3DVector(5.0f,-2.0f,8.0f);
    sword.dead = swordState == 2;
    state.PSwordEmitter.value = swordState ? &sword : 0;
    target.dead = targetStatus == 2;
    arena.dead = arenaStatus == 2;
    state.PTargetThing.value = targetStatus == 1 ? 0 : &target;
    state.PArenaCentre.value = arenaStatus == 1 ? 0 : &arena;
    state.creature.appearance.scale = scale;
    state.creature.def.battle.ScreamInAirTime = 1.0f;
    state.creature.def.battle.ScaleDownTime = 2.0f;
    state.creature.def.battle.DeathScreamTime = 1.5f;
    state.boss.delay = delay;
    state.boss.revokeAfterReady = readyRevoked;
    state.world.seed = seed;
    state.failCheck = failedCheck;
    releases = 0;

    state.Update();

    CHECK(state.stops == 1 && state.creature.bossLookups == 1 && releases == 1 && heldDefs == 0);
    CHECK(sword.kills == (swordState == 1 ? 1U : 0U));
    CHECK(state.screams == 1 && state.falls <= 1 && state.deaths <= 1);
    CHECK(state.creature.physics.facings == 1 + state.deaths);
    CHECK(state.creature.physics.enables == state.falls && state.creature.physics.teleports == state.deaths);
    CHECK(state.creature.appearance.scaleReads == 0 || state.creature.appearance.scaleWrites <= state.creature.appearance.scaleReads);
    if (targetStatus || arenaStatus) {
        CHECK(state.idle == 0 && state.falls == 0 && state.boss.phases == 0);
    }
    else if (!failedCheck) {
        CHECK(state.falls == 1 && state.creature.appearance.scale == 1.2f);
        CHECK(state.creature.appearance.scaleReads >= 1 && state.creature.appearance.scaleWrites >= 1);
        CHECK(state.boss.phases == (readyRevoked ? 1U : 2U) && state.deaths == (readyRevoked ? 0U : 1U));
        CHECK(state.world.requests == (delay == 8 ? 1U : 0U));
        CHECK(state.animations == (delay == 8 && seed == 1 ? 1U : 0U));
    }
}

int main()
{
    unsigned executed = 0;
    for (unsigned scenario = 0; scenario != 48; ++scenario) {
        const int sword = scenario % 3;
        const unsigned flags = scenario / 3;
        const float scale = (flags & 1) ? 2.0f : 1.1f;
        const long delay = (flags & 2) ? 8 : 0;
        const unsigned long seed = (flags & 4) ? 1 : 0;
        const bool revoke = (flags & 8) != 0;
        Stage uninterrupted;
        Run(uninterrupted, sword, scale, delay, seed, revoke, 0);
        ++executed;
        for (unsigned i = 1; i <= uninterrupted.checks; ++i) {
            Stage interrupted;
            Run(interrupted, sword, scale, delay, seed, revoke, i);
            CHECK(interrupted.checks >= i && interrupted.idle <= uninterrupted.idle);
            ++executed;
        }
    }
    for (int target = 0; target != 3; ++target) {
        for (int arena = 0; arena != 3; ++arena) {
            Stage state;
            Run(state, 1, 2.0f, 0, 0, false, 0, target, arena);
            ++executed;
        }
    }
    CHECK(executed == 753);
    std::puts(failures ? "009756b0_TEST FAIL" : "009756b0_TEST PASS");
    return failures ? 1 : 0;
}