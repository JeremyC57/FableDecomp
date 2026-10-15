#include <cstdio>
// Host model of the script-thing plumbing: a thing with a component-presence bitset and a sorted component map
// searched by lower bound (the end entry when the kind is absent), a dead flag, and the intelligent pointer at +0x0c.
struct TC { long kind; TC() : kind(0) {} virtual ~TC() {} };
struct Entry { long kind; TC* tc; };
struct Thing {
    unsigned int mask[8];
    Entry entries[9];
    int count;
    bool dead;
    Thing() : count(0), dead(false) { for (int i = 0; i < 8; ++i) mask[i] = 0; entries[0].kind = 0; entries[0].tc = 0; }
    void Add(long kind, TC* tc)
    {
        int i = count;
        while (i > 0 && entries[i - 1].kind > kind) { entries[i] = entries[i - 1]; --i; }
        entries[i].kind = kind; entries[i].tc = tc; ++count;
        entries[count].kind = 0; entries[count].tc = 0;
        mask[kind >> 5] |= 1u << (kind & 31);
        tc->kind = kind;
    }
    unsigned int HasTC(long kind) const { return mask[kind >> 5] & (1u << (kind & 31)); }
    TC* GetTC(long kind) const
    {
        int i = 0; while (i < count && entries[i].kind < kind) ++i;
        const Entry* it = &entries[i]; const Entry* last = &entries[count];
        if (it == last || kind < it->kind) it = last;
        return it->tc;
    }
    template <class T> bool GetInterface(T** out, long kind) const { if (HasTC(kind)) { *out = (T*)GetTC(kind); return true; } return false; }
};
struct IntelligentPointer { Thing* item; Thing* GetPItem() const { return item; } };
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct Control : TC { long priority; Control() : priority(0) {} };
struct GameScriptThing {
    IntelligentPointer PThing;
    long GetCurrentScriptPriority() const
    {
        if (PThing.GetPItem() && !PThing.GetPItem()->dead) {
            const Control* c;
            if (PThing.GetPItem()->GetInterface(&c, 0x1f))
                return c->priority;
        }
        return 0;
    }
};
int main()
{
    Thing t; Control c; GameScriptThing g; g.PThing.item = 0;
    CHECK(g.GetCurrentScriptPriority() == 0);
    g.PThing.item = &t;
    CHECK(g.GetCurrentScriptPriority() == 0);
    t.Add(0x1f, &c); c.priority = 7;
    CHECK((t.mask[0] & 0x80000000u) != 0);
    CHECK(g.GetCurrentScriptPriority() == 7);
    t.dead = true;
    CHECK(g.GetCurrentScriptPriority() == 0);
    if (g_fail) { std::printf("008d0ac0_TEST FAIL\n"); return 1; }
    std::printf("008d0ac0_TEST PASS\n");
    return 0;
}
