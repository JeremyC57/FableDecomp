// Behaviour test for 0x00864B30 NCreatureAI::IsOverridingFactionEnemyOnTarget(const CThingAICreature*, const CThing*)
// The model mirrors the source: the creature def is fetched and released exactly once on every path; creature group 4
// never overrides; only village members qualify, and not while scripted control is locked or has queued actions; the
// creature needs an opinion of the hero and must not be in state group 3; then the target's opinion source (faction
// override for this witness) or a target deed log with the creature willing to attack on opinion overrides.
#include <cstdio>

static int g_def_refs = 0;
static int g_def_fetches = 0;

struct CreatureDef { unsigned long creature_group; };

struct Creature {
    bool has_def;
    CreatureDef def;
    bool village_member;            // TC 0x23
    bool has_scripted;              // TC 0x1F
    bool scripted_locked;
    bool scripted_actions_empty;
    bool has_opinion_of_hero;       // TC 0x4F
    bool willing_on_opinion;
    int  state_group;
};
struct Target {
    bool has_opinion_source;        // TC 0x50
    bool overrides_faction_for_witness;
    bool has_deed_log;              // TC 0x51
};

struct DefRef {                     // CDefPointer<const CCreatureDef>
    const CreatureDef* p;
    DefRef() : p(0) { }
    ~DefRef() { if (p) --g_def_refs; }
};
static bool GetSubDef(const Creature& c, DefRef* out)
{
    ++g_def_fetches;
    if (!c.has_def) return false;
    out->p = &c.def; ++g_def_refs;
    return true;
}

static bool IsOverridingFactionEnemyOnTarget(const Creature* creature, const Target* target)
{
    DefRef def;
    if (GetSubDef(*creature, &def) && def.p != 0 && def.p->creature_group == 4)
        return false;
    if (creature->village_member) {
        bool scripted = false;
        if (creature->has_scripted)
            scripted = creature->scripted_locked || !creature->scripted_actions_empty;
        if (!scripted && creature->has_opinion_of_hero && creature->state_group != 3) {
            if (target->has_opinion_source && target->overrides_faction_for_witness)
                return true;
            if (target->has_deed_log && creature->willing_on_opinion)
                return true;
        }
    }
    return false;
}

int main()
{
    int fail = 0;
    Creature base = { true, { 1 }, true, false, false, true, true, false, 0 };
    Target t0 = { false, false, false };
    Target src = { true, true, false };
    Target deed = { false, false, true };

    Creature c = base;
    if (IsOverridingFactionEnemyOnTarget(&c, &t0)) fail |= 1;            // nothing overrides
    if (!IsOverridingFactionEnemyOnTarget(&c, &src)) fail |= 2;          // opinion source overrides
    if (IsOverridingFactionEnemyOnTarget(&c, &deed)) fail |= 4;          // deed log but not willing
    c.willing_on_opinion = true;
    if (!IsOverridingFactionEnemyOnTarget(&c, &deed)) fail |= 8;
    c.def.creature_group = 4;
    if (IsOverridingFactionEnemyOnTarget(&c, &src)) fail |= 16;          // group 4 never
    c = base; c.has_def = false;
    if (!IsOverridingFactionEnemyOnTarget(&c, &src)) fail |= 32;         // no def: carry on
    c = base; c.village_member = false;
    if (IsOverridingFactionEnemyOnTarget(&c, &src)) fail |= 64;
    c = base; c.has_scripted = true; c.scripted_locked = true;
    if (IsOverridingFactionEnemyOnTarget(&c, &src)) fail |= 128;
    c = base; c.has_scripted = true; c.scripted_actions_empty = false;
    if (IsOverridingFactionEnemyOnTarget(&c, &src)) fail |= 256;
    c = base; c.has_scripted = true;                                       // idle scripted control is fine
    if (!IsOverridingFactionEnemyOnTarget(&c, &src)) fail |= 512;
    c = base; c.has_opinion_of_hero = false;
    if (IsOverridingFactionEnemyOnTarget(&c, &src)) fail |= 1024;
    c = base; c.state_group = 3;
    if (IsOverridingFactionEnemyOnTarget(&c, &src)) fail |= 2048;
    Target src_no = { true, false, false };
    c = base;
    if (IsOverridingFactionEnemyOnTarget(&c, &src_no)) fail |= 4096;
    if (g_def_refs != 0) fail |= 8192;                                     // every path released the def
    if (g_def_fetches != 13) fail |= 16384;
    if (fail) { std::printf("00864b30_TEST FAIL\n"); return 1; }
    std::printf("00864b30_TEST PASS\n");
    return 0;
}