// behaviour test for 0x008a0f70 CGameScriptInterface::SetReadableObjectText: when the thing exists, is alive and has an action-use component (interface 0x0D), replace its readable text
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
static const int KIND = 0xD;

// the function under test, as reconstructed (form ND)
static void Under(Script* s, int arg)
{
    (void)s; (void)arg;
    Thing* t = s->thing;
    Component* tc;
    if (t && !t->dead && GetInterface(t, &tc, KIND))
        tc->value = arg;
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
static int Fail() { std::printf("008a0f70_TEST FAIL\n"); return 1; }
int main()
{
    Thing t; Component c; Script s;
    // 1: everything present -> the component is used
    Reset(t, c, true); s.valid = true; s.thing = &t; g_hero = &t;
    Under(&s, 5);
    if (!(c.value == 5)) return Fail();
    // 2: the interface bit is clear -> nothing happens
    Reset(t, c, false); s.valid = true; s.thing = &t; g_hero = &t;
    Under(&s, 5);
    if (!(c.value == -7)) return Fail();
    // 3: the guard fails (dead thing) -> nothing happens
    Reset(t, c, true); s.valid = true; s.thing = &t; t.dead = true;
    Under(&s, 5);
    if (!(c.value == -7)) return Fail();
    std::printf("008a0f70_TEST PASS\n");
    return 0;
}