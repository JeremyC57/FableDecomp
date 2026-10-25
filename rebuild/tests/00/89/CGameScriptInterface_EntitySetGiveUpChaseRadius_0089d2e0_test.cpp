// behaviour test for 0x0089d2e0 CGameScriptInterface::EntitySetGiveUpChaseRadius: only a valid script thing whose thing carries interface
// type 0x34 gets CTCPerceiveThing::SetGiveUpChaseRadius(value); the sorted map's end entry stands in for a missing key
#include <cstdio>
struct Comp { int calls; float last; };
struct Entry { long type; Comp* comp; };
struct Thing {
    unsigned int bits[9];
    Entry* first;
    Entry* last;
};
struct ScriptThing { bool valid; Thing* thing; };
static void Set(const ScriptThing& st, float value)
{
    if (st.valid) {
        Thing* t = st.thing;
        const long type = 0x34;
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
    Entry entries[2] = { { 0x34, &mine }, { 0x7fff, &endc } };
    Thing t; for (int i = 0; i < 9; ++i) t.bits[i] = 0;
    t.first = entries; t.last = entries + 1;
    ScriptThing st; st.valid = true; st.thing = &t;
    Set(st, 1.5f);
    if (mine.calls != 0) fails |= 1;                         // interface bit clear: nothing
    t.bits[0x34 >> 5] |= 1u << (0x34 & 31);
    Set(st, 1.5f);
    if (mine.calls != 1 || !(mine.last == 1.5f)) fails |= 2;
    st.valid = false;
    Set(st, -2.0f);
    if (mine.calls != 1) fails |= 4;                         // invalid script thing: nothing
    st.valid = true; entries[0].type = 0x34 + 1;
    Set(st, -2.0f);
    if (endc.calls != 1 || !(endc.last == -2.0f)) fails |= 8; // missing key: the end entry's component
    if (fails) { std::printf("0089d2e0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0089d2e0_TEST PASS\n");
    return 0;
}