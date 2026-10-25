// Behaviour test for 0x00861E60 NCreatureMode::GetRandomAnimationFromCreature(const CCharString&, const CThingCreatureBase&,
//   const CAnimFilterBase&, unsigned long*)
// The model mirrors the source: the creature's appearance def (when it has one) files entries under the name's CRC; every
// entry in that equal range is offered to the caller's filter and to the no-group filter; a random pick (the 0x24A1/0x24DF
// LCG, rotated right by 13, modulo the count) is made among the filter's matches, else among the no-group matches, and
// copied into a new entry; otherwise the result is null. The def reference taken by GetSubDef is always released.
#include <cstdio>
#include <cstring>

static int g_def_refs = 0;
static int g_entries_alive = 0;

struct Entry { unsigned long key; long anim; long group; };
struct EntryCopy { long anim; long group; EntryCopy(const Entry& e) : anim(e.anim), group(e.group) { ++g_entries_alive; } };

static unsigned long Ror13(unsigned long v) { v &= 0xffffffffUL; return ((v >> 13) | (v << 19)) & 0xffffffffUL; }
static unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = (*seed * 0x24a1 + 0x24df) & 0xffffffffUL;
    *seed = Ror13(*seed);
    return range ? *seed % range : 0;
}
static unsigned long Crc(const char* s) { unsigned long h = 0; while (*s) h = (h * 31 + (unsigned char)*s++) & 0xffffffffUL; return h; }

struct AppearanceDef { const Entry* entries; int count; };   // sorted by key
struct Creature { const AppearanceDef* def; };
struct Filter { long wanted_group; bool any; bool operator()(const Entry& e) const { return any || e.group == wanted_group; } };

static EntryCopy* GetRandomAnimationFromCreature(const char* name, const Creature& creature, const Filter& filter, unsigned long* seed)
{
    EntryCopy* result = 0;
    if (creature.def) {
        ++g_def_refs;
        const Entry* filtered[16]; int nf = 0;
        const Entry* no_group[16]; int nn = 0;
        unsigned long key = Crc(name);
        for (int i = 0; i < creature.def->count; ++i) {
            const Entry& e = creature.def->entries[i];
            if (e.key != key) continue;
            if (filter(e)) filtered[nf++] = &e;
            if (e.group == 0) no_group[nn++] = &e;
        }
        if (nf != 0) result = new EntryCopy(*filtered[GFRandom(nf, seed)]);
        else if (nn != 0) result = new EntryCopy(*no_group[GFRandom(nn, seed)]);
        --g_def_refs;
    }
    return result;
}

int main()
{
    int fail = 0;
    unsigned long k = Crc("STAND"), other = Crc("WALK");
    Entry e[6] = { { k, 1, 0 }, { k, 2, 5 }, { k, 3, 5 }, { k, 4, 0 }, { other, 9, 5 }, { other, 10, 0 } };
    AppearanceDef def = { e, 6 };
    Creature c = { &def };
    Filter group5 = { 5, false };
    unsigned long seed = 1234;
    unsigned long expect_seed = 1234;
    unsigned long pick = GFRandom(2, &expect_seed);
    EntryCopy* r = GetRandomAnimationFromCreature("STAND", c, group5, &seed);
    if (r == 0 || r->anim != (pick == 0 ? 2 : 3) || seed != expect_seed) fail |= 1;   // filtered pool: entries 2 and 3
    delete r; --g_entries_alive;
    Filter group7 = { 7, false };
    expect_seed = seed;
    pick = GFRandom(2, &expect_seed);
    r = GetRandomAnimationFromCreature("STAND", c, group7, &seed);
    if (r == 0 || r->anim != (pick == 0 ? 1 : 4) || seed != expect_seed) fail |= 2;   // falls back to the no-group pool
    delete r; --g_entries_alive;
    unsigned long before = seed;
    if (GetRandomAnimationFromCreature("RUN", c, group5, &seed) != 0 || seed != before) fail |= 4;   // no entries: null, seed untouched
    Creature bare = { 0 };
    if (GetRandomAnimationFromCreature("STAND", bare, group5, &seed) != 0) fail |= 8;                 // no appearance def
    Entry only_grouped[1] = { { k, 7, 3 } };
    AppearanceDef def2 = { only_grouped, 1 };
    Creature c2 = { &def2 };
    if (GetRandomAnimationFromCreature("STAND", c2, group5, &seed) != 0) fail |= 16;                  // neither pool qualifies
    Filter all = { 0, true };
    r = GetRandomAnimationFromCreature("STAND", c2, all, &seed);
    if (r == 0 || r->anim != 7) fail |= 32;                                                         // single match: index 0
    delete r; --g_entries_alive;
    if (g_def_refs != 0 || g_entries_alive != 0) fail |= 64;
    if (fail) { std::printf("00861e60_TEST FAIL %d\n", fail); return 1; }
    std::printf("00861e60_TEST PASS\n");
    return 0;
}