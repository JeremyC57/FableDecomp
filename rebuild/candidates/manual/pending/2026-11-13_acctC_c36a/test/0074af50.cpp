// Behaviour test for CTCCreatureGenerator::CreateCreaturesUndead (retail 0x0074AF50): the function's decisions restated over a small
// model world (the engine callees are the model's hooks): the empty-family / creation-limit early outs, the random family pick
// (seed = ROR13(seed * 0x24A1 + 0x24DF), value % count), the per-creature def lookups and generation-group gate, the two position
// sources (generation point near the triggerer v the generator's own position outside the exclusion area), and the fact that every
// failure inside the loop ends the WHOLE loop (break), while a missing def only skips that creature.
#include <cstdio>
#include <vector>
struct Creature { long def; float x; bool underground; bool hasGenerator; };
struct Model {
    std::vector<std::vector<long> > families;      // family -> creature def indices
    unsigned long seed;
    long created, activeLimit;                      // HasHitCreationLimit: created >= activeLimit (activeLimit < 0: never)
    bool undeadTriggered;
    std::vector<long> missingThingDef, missingCreatureDef, disabledGroupDefs, deadOnCreate;
    bool hasTriggerer, triggererAlive, pointFound; float pointX, ownX; bool hasExclusion, ownPosExcluded;
    std::vector<Creature> out; long globalCount;
};
static bool In(const std::vector<long>& v, long x) { for (unsigned i = 0; i < v.size(); ++i) if (v[i] == x) return true; return false; }
static unsigned long ROR13(unsigned long v) { return (v >> 13) | (v << 19); }
static bool HasHitCreationLimit(const Model& m) { return m.activeLimit >= 0 && m.created >= m.activeLimit; }
static void CreateCreaturesUndead(Model& m)
{
    if (m.families.size() <= 0 || HasHitCreationLimit(m))
        return;
    m.undeadTriggered = true;
    m.seed = m.seed * 0x24a1 + 0x24df;
    m.seed = ROR13(m.seed);
    unsigned long count = (unsigned long)m.families.size();
    unsigned long index = count ? m.seed % count : 0;
    const std::vector<long>& family = m.families.at(index);
    for (std::vector<long>::const_iterator it = family.begin(); it != family.end(); ++it) {
        if (In(m.missingThingDef, *it))
            continue;
        if (In(m.missingCreatureDef, *it) || In(m.disabledGroupDefs, *it))
            continue;
        float x;
        if (m.hasTriggerer) {
            if (!m.pointFound)
                break;
            x = m.pointX;
        } else {
            x = m.ownX;
            if (m.hasExclusion && m.ownPosExcluded)
                break;
        }
        if (In(m.deadOnCreate, *it))
            break;
        Creature c; c.def = *it; c.x = x; c.hasGenerator = true;
        c.underground = m.hasTriggerer && m.triggererAlive;
        m.out.push_back(c);
        ++m.globalCount;
        ++m.created;
        if (HasHitCreationLimit(m))
            break;
    }
}
static int fails = 0;
static void check(bool c, const char* what) { if (!c) { ++fails; std::printf("FAIL %s\n", what); } }
static Model Base()
{
    Model m; m.seed = 1; m.created = 0; m.activeLimit = -1; m.undeadTriggered = false; m.hasTriggerer = true; m.triggererAlive = true;
    m.pointFound = true; m.pointX = 5.0f; m.ownX = 9.0f; m.hasExclusion = false; m.ownPosExcluded = false; m.globalCount = 0;
    std::vector<long> a; a.push_back(10); a.push_back(11); a.push_back(12); m.families.push_back(a);
    return m;
}
int main()
{
    Model m = Base(); m.families.clear();
    CreateCreaturesUndead(m);
    check(!m.undeadTriggered && m.out.empty() && m.seed == 1, "no families: nothing, seed untouched");
    m = Base(); m.activeLimit = 0;
    CreateCreaturesUndead(m);
    check(!m.undeadTriggered && m.out.empty(), "limit already hit: nothing");
    m = Base();
    CreateCreaturesUndead(m);
    check(m.undeadTriggered && m.out.size() == 3 && m.out[0].def == 10 && m.out[2].def == 12 && m.out[1].x == 5.0f && m.out[0].underground
          && m.out[0].hasGenerator && m.globalCount == 3 && m.created == 3, "whole family made at the generation point, underground");
    check(m.seed == ROR13(1UL * 0x24a1 + 0x24df), "seed stepped once");
    m = Base(); { std::vector<long> b; b.push_back(20); m.families.push_back(b); std::vector<long> c; c.push_back(30); m.families.push_back(c); }
    unsigned long pick = ROR13(1UL * 0x24a1 + 0x24df) % 3;
    CreateCreaturesUndead(m);
    check(m.out.size() == (pick == 0 ? 3u : 1u) && m.out[0].def == (pick == 0 ? 10 : pick == 1 ? 20 : 30), "random family pick");
    m = Base(); m.missingThingDef.push_back(10); m.disabledGroupDefs.push_back(12);
    CreateCreaturesUndead(m);
    check(m.out.size() == 1 && m.out[0].def == 11, "missing def / disabled group skip only that creature");
    m = Base(); m.missingCreatureDef.push_back(11);
    CreateCreaturesUndead(m);
    check(m.out.size() == 2 && m.out[1].def == 12, "missing creature sub-def skips");
    m = Base(); m.pointFound = false;
    CreateCreaturesUndead(m);
    check(m.undeadTriggered && m.out.empty(), "no generation point: loop ends");
    m = Base(); m.hasTriggerer = false;
    CreateCreaturesUndead(m);
    check(m.out.size() == 3 && m.out[0].x == 9.0f && !m.out[0].underground, "no triggerer: own position, no underground action");
    m = Base(); m.hasTriggerer = false; m.hasExclusion = true; m.ownPosExcluded = true;
    CreateCreaturesUndead(m);
    check(m.out.empty(), "own position inside the exclusion area: loop ends");
    m = Base(); m.hasTriggerer = false; m.hasExclusion = true;
    CreateCreaturesUndead(m);
    check(m.out.size() == 3, "exclusion area that does not hold the position");
    m = Base(); m.triggererAlive = false;
    CreateCreaturesUndead(m);
    check(m.out.size() == 3 && !m.out[0].underground && m.out[0].x == 5.0f, "dead triggerer: point used, no underground action");
    m = Base(); m.deadOnCreate.push_back(11);
    CreateCreaturesUndead(m);
    check(m.out.size() == 1 && m.out[0].def == 10 && m.created == 1, "failed creation ends the loop (not a skip)");
    m = Base(); m.activeLimit = 2;
    CreateCreaturesUndead(m);
    check(m.out.size() == 2 && m.created == 2 && m.globalCount == 2, "limit reached mid-family ends the loop");
    std::printf(fails ? "0074af50_TEST FAIL\n" : "0074af50_TEST PASS\n");
    return fails;
}
