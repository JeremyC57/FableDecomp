// behaviour test for 0x0068ADE0 CInputProcessInventoryMapScreen::Init: the component of interface type 0x1B is looked up only when its bit is set
// in the creature's interface bitset; the sorted map's lower bound gives it, a missing key gives the map's end entry
#include <cstdio>
struct Entry { long type; void* component; };
struct Creature {
    unsigned long bits[9];
    Entry* first;
    Entry* last;
};
static Entry* LowerBound(Creature* c, long key)
{
    Entry* it = c->first;
    while (it != c->last && it->type < key) ++it;
    return it;
}
static void* gInventory = 0;
static void Init(Creature* c, long type)
{
    if (c->bits[type >> 5] & (1UL << (type & 31))) {
        Entry* it = LowerBound(c, type);
        if (it == c->last || type < it->type)
            it = c->last;
        gInventory = it->component;
    }
}
int main()
{
    int fails = 0;
    int compA = 1, compB = 2, endMarker = 3;
    Entry entries[3] = { { 0x05, &compA }, { 0x1B, &compB }, { 0x7fff, &endMarker } };
    Creature c;
    for (int i = 0; i < 9; ++i) c.bits[i] = 0;
    c.first = entries; c.last = entries + 2;
    Init(&c, 0x1B);
    if (gInventory != 0) fails |= 1;                         // bit clear: untouched
    c.bits[0x1B >> 5] |= 1UL << (0x1B & 31);
    Init(&c, 0x1B);
    if (gInventory != &compB) fails |= 2;                    // found
    entries[1].type = 0x1B + 1;                        // bit set but no entry: the end entry's component
    Init(&c, 0x1B);
    if (gInventory != &endMarker) fails |= 4;
    if (fails) { std::printf("0068ade0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0068ade0_TEST PASS\n");
    return 0;
}