// behaviour test for 0x0089dbb0 CGameScriptInterface::EntitySetAsAbleToWalkThroughSolidObjects: let (or stop) a valid thing with a CTCCreatureNavigation (interface 0x07) move through solid objects
#include <cstdio>
struct Entry { int type; void* component; };
struct Thing {
    unsigned int bits[9];
    Entry entries[4];
    int count;
    unsigned char type;
    bool dead;
};
static Entry* LowerBound(Thing* t, int key)
{
    Entry* first = t->entries;
    int count = t->count;
    while (count > 0) {
        int half = count / 2;
        if (first[half].type < key) { first += half + 1; count -= half + 1; }
        else count = half;
    }
    return first;
}
template <class T> static bool GetInterface(Thing* t, T** out, int type)
{
    if (t->bits[type >> 5] & (1u << (type & 31))) {
        Entry* end = t->entries + t->count;
        Entry* it = LowerBound(t, type);
        if (it == end || type < it->type)
            it = end;
        *out = (T*)it->component;
        return true;
    }
    return false;
}
struct Component { int value; int calls; int extra; };
struct Script { bool valid; Thing* thing; };
static Thing* g_hero = 0;
static const int KIND = 0x7;

// the function under test, as reconstructed (form V)
static void Under(Script* s, int arg)
{
    (void)s; (void)arg;
    if (s->valid) {
        Component* tc;
        if (GetInterface(s->thing, &tc, KIND))
            tc->calls++, tc->extra = arg;
    }
}

static void Reset(Thing& t, Component& c, bool withTC)
{
    for (int i = 0; i < 9; ++i) t.bits[i] = 0;
    t.count = 3;
    t.entries[0].type = 1;    t.entries[0].component = 0;
    t.entries[1].type = KIND; t.entries[1].component = &c;
    t.entries[2].type = 0x200; t.entries[2].component = 0;
    t.type = 1; t.dead = false;
    if (withTC) t.bits[KIND >> 5] |= 1u << (KIND & 31);
    c.value = -7; c.calls = 0; c.extra = 0;
}
static int Fail() { std::printf("0089dbb0_TEST FAIL\n"); return 1; }
int main()
{
    Thing t; Component c; Script s;
    // 1: everything present -> the component is used
    Reset(t, c, true); s.valid = true; s.thing = &t; g_hero = &t;
    Under(&s, 5);
    if (!(c.calls == 1 && c.extra == 5)) return Fail();
    // 2: the interface bit is clear -> nothing happens
    Reset(t, c, false); s.valid = true; s.thing = &t; g_hero = &t;
    Under(&s, 5);
    if (!(c.calls == 0)) return Fail();
    // 3: the guard fails (invalid script thing) -> nothing happens
    Reset(t, c, true); s.valid = false; s.thing = &t;
    Under(&s, 5);
    if (!(c.calls == 0)) return Fail();
    std::printf("0089dbb0_TEST PASS\n");
    return 0;
}