// Behaviour test for 0x008DF510 NCreatureMode::CModeScorpionKingTailPlunge::Recover()
// The model mirrors the source: a live tail emitter is killed; the special-effects sub-def (when present) spawns
// TAIL_DUST_EFFECT at the tail position when the name resolves and is released at the end; the creature's dependent things
// are killed (the 0xAD component is fetched or null; the creature always carries it); the withdraw action is set with the
// target and the temporary action destroyed.
#include <cstdio>
#include <cstring>

static int g_def_refs = 0;
static int g_effects_created = 0;
static float g_effect_pos[3];
static int g_dependents_killed = 0;
static int g_actions_set = 0;
static long g_effect_type = -1;
static int g_actions_alive = 0;
static const void* g_last_target = 0;

struct Thing { bool exists; bool dead; bool killed; };
struct EffectsDef { bool has_tail_dust; long type; };
struct Creature { bool has_effects_def; EffectsDef effects; int dependents; };

struct Action {
    const void* target;
    Action(const void* t) : target(t) { ++g_actions_alive; }
    ~Action() { --g_actions_alive; }
};
static void SetCurrentAction(const Action& a) { ++g_actions_set; g_last_target = a.target; }

struct DefRef { const EffectsDef* p; DefRef() : p(0) { } ~DefRef() { if (p) --g_def_refs; } };

struct Mode {
    Creature* creature;
    float tail_pos[3];
    Thing* target;
    Thing* tailEmitter;

    void Recover()
    {
        if (tailEmitter->exists && !tailEmitter->dead)
            tailEmitter->killed = true;
        DefRef effects;
        long effect_type = 0;
        if (creature->has_effects_def) { effects.p = &creature->effects; ++g_def_refs; }
        if (effects.p) {
            if (effects.p->has_tail_dust) {
                effect_type = effects.p->type;
                ++g_effects_created;
                g_effect_type = effect_type;
                std::memcpy(g_effect_pos, tail_pos, sizeof g_effect_pos);
            }
        }
        g_dependents_killed += creature->dependents;
        creature->dependents = 0;
        SetCurrentAction(Action(target->exists ? target : 0));
    }
};

int main()
{
    int fail = 0;
    Creature c = { true, { true, 12 }, 3 };
    Thing target = { true, false, false };
    Thing emitter = { true, false, false };
    Mode m = { &c, { 1.0f, 2.0f, 3.0f }, &target, &emitter };
    m.Recover();
    if (!emitter.killed) fail |= 1;
    if (g_effects_created != 1 || g_effect_type != 12 || g_effect_pos[0] != 1.0f || g_effect_pos[2] != 3.0f) fail |= 2;
    if (g_dependents_killed != 3 || c.dependents != 0) fail |= 4;
    if (g_actions_set != 1 || g_last_target != &target || g_actions_alive != 0) fail |= 8;
    Thing deadEmitter = { true, true, false };
    m.tailEmitter = &deadEmitter;
    c.effects.has_tail_dust = false;
    m.Recover();
    if (deadEmitter.killed) fail |= 16;                 // already dead: not killed again
    if (g_effects_created != 1) fail |= 32;              // name not in the def: no effect
    if (g_actions_set != 2) fail |= 64;
    c.has_effects_def = false;
    Thing no_target = { false, false, false };
    m.target = &no_target;
    m.Recover();
    if (g_effects_created != 1 || g_actions_set != 3 || g_last_target != 0) fail |= 128;
    if (g_def_refs != 0 || g_actions_alive != 0) fail |= 256;
    if (fail) { std::printf("008df510_TEST FAIL %d\n", fail); return 1; }
    std::printf("008df510_TEST PASS\n");
    return 0;
}