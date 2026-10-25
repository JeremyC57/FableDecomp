// Behaviour test for 0x008648A0 NCreatureAI::IsWillingToAttackTarget(const CThingAICreature*, const CThing*)
// The model mirrors the source: the creature def is fetched and released once on every path; creature group 4 and
// non-village-members are always willing; a member of a village hostile through crime facing a creature target needs a
// hero-enemy opinion (state group 3 short-circuits to willing); scripted control, a summoned creature, no opinion of
// the hero or state group 3 mean willing; otherwise the target's opinion source (witnesses will not attack) or a deed
// log the opinion is unwilling to attack on says no.
#include <cstdio>

static int g_def_refs = 0;

struct Creature {
    bool has_def;
    unsigned long creature_group;
    bool village_member;            // TC 0x23
    bool has_village;
    bool village_enemy_by_crime;
    int  state_group;
    bool has_opinion_of_hero;       // TC 0x4F
    bool hero_opinion_enemy;
    bool willing_on_opinion;
    bool has_scripted;              // TC 0x1F
    bool scripted_locked;
    bool scripted_actions_empty;
    bool summonable;                // TC 0xA0
    bool summoned;
};
struct Target {
    int  thing_type;                // 2 = creature
    bool has_opinion_source;        // TC 0x50
    bool witnesses_will_not_attack;
    bool has_deed_log;              // TC 0x51
};

struct DefRef { const Creature* p; DefRef() : p(0) { } ~DefRef() { if (p) --g_def_refs; } };

static bool IsWillingToAttackTarget(const Creature* c, const Target* t)
{
    DefRef def;
    if (c->has_def) { def.p = c; ++g_def_refs; }
    if (def.p != 0 && c->creature_group == 4)
        return true;
    if (c->village_member) {
        if (t->thing_type == 2 && c->has_village && c->village_enemy_by_crime) {
            if (c->state_group != 3) {
                if (!(c->has_opinion_of_hero && c->hero_opinion_enemy))
                    return false;
            } else
                return true;
        }
        bool scripted = false;
        if (c->has_scripted)
            scripted = c->scripted_locked || !c->scripted_actions_empty;
        if (!scripted && !(c->summonable && c->summoned) && c->has_opinion_of_hero && c->state_group != 3) {
            if (t->has_opinion_source && t->witnesses_will_not_attack)
                return false;
            if (t->has_deed_log && !c->willing_on_opinion)
                return false;
        }
    }
    return true;
}

int main()
{
    int fail = 0;
    const Creature base = { true, 1, true, true, false, 0, true, false, true, false, false, true, false, false };
    const Target plain = { 1, false, false, false };
    const Target witness = { 1, true, true, false };
    const Target deed = { 1, false, false, true };
    const Target creature_target = { 2, false, false, false };
    Creature c = base;
    if (!IsWillingToAttackTarget(&c, &plain)) fail |= 1;
    if (IsWillingToAttackTarget(&c, &witness)) fail |= 2;                       // witnesses will not attack
    if (!IsWillingToAttackTarget(&c, &deed)) fail |= 4;                         // willing on opinion
    c.willing_on_opinion = false;
    if (IsWillingToAttackTarget(&c, &deed)) fail |= 8;
    c = base; c.creature_group = 4;
    if (!IsWillingToAttackTarget(&c, &witness)) fail |= 16;
    c = base; c.village_member = false;
    if (!IsWillingToAttackTarget(&c, &witness)) fail |= 32;
    c = base; c.has_scripted = true; c.scripted_actions_empty = false;
    if (!IsWillingToAttackTarget(&c, &witness)) fail |= 64;
    c = base; c.has_scripted = true;                                              // idle scripted control
    if (IsWillingToAttackTarget(&c, &witness)) fail |= 128;
    c = base; c.summonable = true; c.summoned = true;
    if (!IsWillingToAttackTarget(&c, &witness)) fail |= 256;
    c = base; c.has_opinion_of_hero = false;
    if (!IsWillingToAttackTarget(&c, &witness)) fail |= 512;
    c = base; c.village_enemy_by_crime = true;                                    // hostile village, creature target
    if (IsWillingToAttackTarget(&c, &creature_target)) fail |= 1024;            // no hero-enemy opinion
    c.hero_opinion_enemy = true;
    if (!IsWillingToAttackTarget(&c, &creature_target)) fail |= 2048;
    c.hero_opinion_enemy = false; c.state_group = 3;
    if (!IsWillingToAttackTarget(&c, &creature_target)) fail |= 4096;
    if (g_def_refs != 0) fail |= 8192;
    if (fail) { std::printf("008648a0_TEST FAIL\n"); return 1; }
    std::printf("008648a0_TEST PASS\n");
    return 0;
}