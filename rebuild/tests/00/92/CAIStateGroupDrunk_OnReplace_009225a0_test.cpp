// behaviour test for 0x009225a0 CAIStateGroup_Drunk::OnReplace(): the brain's creature's mode manager (interface type 0x31, found through
// the sorted interface map) gets RemoveMode(44) exactly once
#include <cstdio>
struct Modes { int calls; unsigned int mode; };
struct Entry { long type; Modes* comp; };
struct Creature { unsigned int bits[9]; Entry* first; Entry* last; };
struct Brain { Creature* creature; };
struct Group { Brain* brain; };
static void Run(Group& g)
{
    Creature* c = g.brain->creature;
    const long type = 0x31;
    Modes* m = 0;
    if (c->bits[type >> 5] & (1u << (type & 31))) {
        Entry* it = c->first;
        while (it != c->last && it->type < type) ++it;
        if (it == c->last || type < it->type) it = c->last;
        m = it->comp;
    }
    m->calls++; m->mode = 44;
}
int main()
{
    int fails = 0;
    Modes mine = { 0, 0 }, endm = { 0, 0 };
    Entry entries[3] = { { 0x10, &endm }, { 0x31, &mine }, { 0x7fff, &endm } };
    Creature c; for (int i = 0; i < 9; ++i) c.bits[i] = 0;
    c.bits[0x31 >> 5] |= 1u << (0x31 & 31);
    c.first = entries; c.last = entries + 2;
    Brain b; b.creature = &c;
    Group g; g.brain = &b;
    Run(g);
    if (mine.calls != 1 || mine.mode != 44u) fails |= 1;
    if (endm.calls != 0) fails |= 2;
    if (fails) { std::printf("009225a0_TEST FAIL %d\n", fails); return 1; }
    std::printf("009225a0_TEST PASS\n");
    return 0;
}