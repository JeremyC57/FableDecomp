// Behaviour test for 0x007A0D90 CTCWillLightning::GetTargets: a host model of the row's selection and beam set-up over stand-in things,
// beams and lists (the row's callees are out-of-line game functions); the predicates, the search and the random draws are scripted.
#include <cstdio>
#include <cstring>
#include <cmath>

struct Thing { float x, y; bool dead, creature, vulnerable, lightningable, underground, enemyOfOwner; int enemyOf; long type; };
struct Beam { Thing* home; Thing* target; float damage, decap, delay, act, deact; long onhit, frameCount; bool temporary; int def; };
struct World { unsigned int seed; };

static unsigned int Ror13(unsigned int v) { return (v >> 13) | (v << 19); }
static unsigned int GFRandom(unsigned int range, unsigned int* seed)
{
    *seed *= 0x24a1; *seed += 0x24df; *seed = Ror13(*seed);
    return range ? *seed % range : 0;
}
static float GFFloatRandom(float range, unsigned int* seed) { return range * (float)(GFRandom(1000, seed)) / 1000.0f; }
static bool Alive(const Thing* t) { return t != 0 && !t->dead; }

struct Lightning {
    Thing* parent; Thing* player; Thing* override_; Thing* targeted;
    float area, act, deact; long onhit; int def; World* world;
    Thing* all; int allCount;                         // the things the rough-area search can see

    bool GetTargets(Beam* beams, int& beamCount, long maxBeams, Thing* source, Thing** targets, int& targetCount,
                    float damage, float decap, float delay, bool retarget)
    {
        Thing* things[16]; int count = 0;
        Thing* target = 0;
        if (parent->type == 2 && source == player) {
            Thing* t = targeted;
            if (!Alive(t)) return false;
            if (t->creature && !t->vulnerable) return false;
            if (!t->lightningable || t->underground) return false;
            target = t;
        }
        if (source == player) {
            if (Alive(override_)) things[count++] = override_;
            else if (Alive(target)) things[count++] = target;
            else return false;
        } else {
            for (int i = 0; i < allCount; ++i) {
                Thing* t = &all[i];
                float dx = t->x - source->x, dy = t->y - source->y;
                if (t != source && t != player && !t->dead && dx * dx + dy * dy <= area * area) things[count++] = t;
            }
        }
        for (int i = 1; i < count; ++i)                   // nearest first (std::sort with CThingCompare_NearestToPos)
            for (int j = i; j > 0; --j) {
                float a = (things[j]->x - source->x) * (things[j]->x - source->x) + (things[j]->y - source->y) * (things[j]->y - source->y);
                float b = (things[j-1]->x - source->x) * (things[j-1]->x - source->x) + (things[j-1]->y - source->y) * (things[j-1]->y - source->y);
                if (a < b) { Thing* s = things[j]; things[j] = things[j-1]; things[j-1] = s; }
            }
        bool result = false;
        for (int n = 0; n < count; ++n) {
            Thing* thing = things[n];
            bool already = false;
            for (int k = 0; k < targetCount; ++k) if (targets[k] == thing) { already = true; break; }
            if (thing->underground) continue;
            if (!thing->lightningable) continue;
            if (thing != target && !thing->enemyOfOwner) continue;
            if (Alive(target) && target != thing && thing->enemyOf == 1) continue;
            if (already) continue;
            if (retarget) {
                for (int b = 0; b < beamCount; ++b) {
                    result = true;
                    beams[b].target = thing; beams[b].home = source; beams[b].damage = damage;
                    beams[b].decap = GFFloatRandom(1.0f, &world->seed) + decap;
                    beams[b].delay = delay; beams[b].act = act; beams[b].deact = deact; beams[b].def = def;
                    beams[b].onhit = onhit + (long)GFRandom((unsigned int)onhit, &world->seed);
                    beams[b].temporary = false; beams[b].frameCount = -1;
                    targets[targetCount++] = thing;
                }
            } else if ((unsigned long)beamCount < (unsigned long)maxBeams) {
                Beam& nb = beams[beamCount];
                std::memset(&nb, 0, sizeof nb); nb.home = source; nb.target = thing; nb.temporary = true; nb.frameCount = 7;
                nb.damage = damage;
                nb.decap = GFFloatRandom(1.0f, &world->seed) + decap;
                nb.delay = delay; nb.act = act; nb.deact = deact; nb.def = def;
                nb.onhit = (long)GFRandom((unsigned int)onhit, &world->seed) + onhit;
                ++beamCount;
                targets[targetCount++] = thing;
                result = true;
            }
        }
        return result;
    }
};

static int fails = 0;
static void Check(bool ok, int line) { if (!ok) { std::printf("check failed at line %d\n", line); ++fails; } }
#define CHECK(x) Check((x), __LINE__)

int main()
{
    World world = { 12345 };
    Thing all[5];
    std::memset(all, 0, sizeof all);
    for (int i = 0; i < 5; ++i) { all[i].lightningable = true; all[i].enemyOfOwner = true; all[i].creature = true; all[i].vulnerable = true; }
    all[0].x = 0; all[0].y = 0;                       // the caster
    all[1].x = 3; all[1].y = 0;                       // far enemy
    all[2].x = 1; all[2].y = 1;                       // near enemy
    all[3].x = 2; all[3].y = 0; all[3].underground = true;
    all[4].x = 50; all[4].y = 0;                      // out of range
    Thing player; std::memset(&player, 0, sizeof player); player.type = 2;
    Lightning l; std::memset(&l, 0, sizeof l);
    l.parent = &all[0]; l.player = &player; l.area = 10.0f; l.act = 0.25f; l.deact = 0.5f; l.onhit = 4; l.def = 9;
    l.world = &world; l.all = all; l.allCount = 5;

    // 1. new beams from a non-player source: nearest first, the underground one skipped, at most two beams.
    {
        Beam beams[8]; int beamCount = 0; Thing* targets[16]; int targetCount = 0;
        CHECK(l.GetTargets(beams, beamCount, 2, &all[0], targets, targetCount, 5.0f, 1.0f, 0.1f, false));
        CHECK(beamCount == 2 && targetCount == 2);
        CHECK(beams[0].target == &all[2] && beams[1].target == &all[1]);
        CHECK(beams[0].home == &all[0] && beams[0].damage == 5.0f && beams[0].delay == 0.1f && beams[0].def == 9);
        CHECK(beams[0].decap >= 1.0f && beams[0].decap <= 2.0f && beams[0].onhit >= 4 && beams[0].onhit < 8);
        CHECK(beams[0].temporary && beams[0].frameCount == 7);
        // already targeted things are skipped
        CHECK(!l.GetTargets(beams, beamCount, 8, &all[0], targets, targetCount, 5.0f, 1.0f, 0.1f, false));
        CHECK(beamCount == 2);
    }
    // 2. retarget every beam of the list at each new target (each target pushed once per beam).
    {
        Beam beams[8]; std::memset(beams, 0, sizeof beams); int beamCount = 3; Thing* targets[16]; int targetCount = 0;
        CHECK(l.GetTargets(beams, beamCount, 0, &all[0], targets, targetCount, 2.0f, 0.5f, 0.2f, true));
        CHECK(targetCount == 6 && beams[0].target == &all[1] && beams[2].target == &all[1]);
        CHECK(!beams[1].temporary && beams[1].frameCount == -1 && beams[1].act == 0.25f && beams[1].deact == 0.5f);
        // an empty beam list sets nothing
        int none = 0; targetCount = 0;
        CHECK(!l.GetTargets(beams, none, 0, &all[0], targets, targetCount, 2.0f, 0.5f, 0.2f, true));
    }
    // 3. the player as the source: the override thing wins; with no override and no living targeted thing nothing is found.
    {
        l.parent = &player;
        Beam beams[8]; int beamCount = 0; Thing* targets[16]; int targetCount = 0;
        l.targeted = &all[1]; l.override_ = &all[2];
        CHECK(l.GetTargets(beams, beamCount, 4, &player, targets, targetCount, 1.0f, 1.0f, 0.0f, false));
        CHECK(beamCount == 1 && beams[0].target == &all[2]);
        l.override_ = 0; beamCount = 0; targetCount = 0;
        CHECK(l.GetTargets(beams, beamCount, 4, &player, targets, targetCount, 1.0f, 1.0f, 0.0f, false));
        CHECK(beamCount == 1 && beams[0].target == &all[1]);
        all[1].dead = true; beamCount = 0; targetCount = 0;
        CHECK(!l.GetTargets(beams, beamCount, 4, &player, targets, targetCount, 1.0f, 1.0f, 0.0f, false) && beamCount == 0);
        all[1].dead = false; all[1].vulnerable = false;
        CHECK(!l.GetTargets(beams, beamCount, 4, &player, targets, targetCount, 1.0f, 1.0f, 0.0f, false));
    }
    if (fails) { std::printf("007a0d90_TEST FAIL\n"); return 1; }
    std::printf("007a0d90_TEST PASS\n");
    return 0;
}
