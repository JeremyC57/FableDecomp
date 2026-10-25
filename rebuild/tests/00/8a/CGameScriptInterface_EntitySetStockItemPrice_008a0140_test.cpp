// behaviour test for 0x008a0140 CGameScriptInterface::EntitySetStockItemPrice: only a valid script thing whose thing carries interface
// type 0x54 gets CTCStockItem::SetPrice(value); the sorted map's end entry stands in for a missing key
#include <cstdio>
struct Comp { int calls; long last; };
struct Entry { long type; Comp* comp; };
struct Thing {
    unsigned int bits[9];
    Entry* first;
    Entry* last;
};
struct ScriptThing { bool valid; Thing* thing; };
static void Set(const ScriptThing& st, long value)
{
    if (st.valid) {
        Thing* t = st.thing;
        const long type = 0x54;
        if (t->bits[type >> 5] & (1u << (type & 31))) {
            Entry* it = t->first;
            while (it != t->last && it->type < type) ++it;
            if (it == t->last || type < it->type) it = t->last;
            it->comp->calls++; it->comp->last = value;
        }
    }
}
int main()
{
    int fails = 0;
    Comp mine = { 0, 0 }, endc = { 0, 0 };
    Entry entries[2] = { { 0x54, &mine }, { 0x7fff, &endc } };
    Thing t; for (int i = 0; i < 9; ++i) t.bits[i] = 0;
    t.first = entries; t.last = entries + 1;
    ScriptThing st; st.valid = true; st.thing = &t;
    Set(st, 250);
    if (mine.calls != 0) fails |= 1;                         // interface bit clear: nothing
    t.bits[0x54 >> 5] |= 1u << (0x54 & 31);
    Set(st, 250);
    if (mine.calls != 1 || !(mine.last == 250)) fails |= 2;
    st.valid = false;
    Set(st, -7);
    if (mine.calls != 1) fails |= 4;                         // invalid script thing: nothing
    st.valid = true; entries[0].type = 0x54 + 1;
    Set(st, -7);
    if (endc.calls != 1 || !(endc.last == -7)) fails |= 8; // missing key: the end entry's component
    if (fails) { std::printf("008a0140_TEST FAIL %d\n", fails); return 1; }
    std::printf("008a0140_TEST PASS\n");
    return 0;
}