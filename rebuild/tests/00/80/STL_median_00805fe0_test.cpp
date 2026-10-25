// Behaviour test for 0x00805FE0 std::__median of thing pointers under CDoNotUseBinaryPredicate (each compare advances the world seed and answers "low bit clear").
#include <cstdio>

static int g_calls = 0;
static long GFROR13(unsigned long value)   // test stand-in for 0x00497890: rotate right by 13
{
    ++g_calls;
    return (long)((value >> 13) | (value << 19));
}
struct CRandomSeed {
    unsigned long Seed;
    unsigned long Random()
    {
        Seed = (Seed * 0x24a1 + 0x24df) & 0xffffffffUL;
        Seed = (unsigned long)GFROR13(Seed) & 0xffffffffUL;
        return Seed;
    }
};
struct CWorld { CRandomSeed RandomSeed; };
static CWorld g_world;
static CWorld* g_pWorld = &g_world;

struct CThing { int id; };
struct ThingPointer { CThing* item; };
struct CDoNotUseBinaryPredicate {
    bool operator()(const ThingPointer&, const ThingPointer&) const
    {
        if (g_pWorld->RandomSeed.Random() & 1)
            return false;
        return true;
    }
};
static const ThingPointer& __median(const ThingPointer& a, const ThingPointer& b, const ThingPointer& c, CDoNotUseBinaryPredicate comp)
{
    if (comp(a, b))
        if (comp(b, c))
            return b;
        else if (comp(a, c))
            return c;
        else
            return a;
    else if (comp(a, c))
        return a;
    else if (comp(b, c))
        return c;
    else
        return b;
}

// the same decision tree, driven by an explicit list of coin results (true = low bit clear)
static int expected(const bool* coin, int* used)
{
    *used = 2;
    if (coin[0]) { if (coin[1]) return 1; *used = 3; return coin[2] ? 2 : 0; }
    if (coin[1]) return 0;
    *used = 3;
    return coin[2] ? 2 : 1;
}

int main()
{
    CThing things[3] = { { 0 }, { 1 }, { 2 } };
    ThingPointer p[3] = { { &things[0] }, { &things[1] }, { &things[2] } };
    CDoNotUseBinaryPredicate comp;
    int seen[3] = { 0, 0, 0 };
    for (unsigned long s = 1; s < 200; ++s) {
        // predict the coins from a copy of the generator
        CRandomSeed probe; probe.Seed = s * 7919UL;
        bool coin[3];
        for (int k = 0; k < 3; ++k) coin[k] = (probe.Random() & 1) == 0;
        int used = 0;
        int want = expected(coin, &used);
        g_world.RandomSeed.Seed = s * 7919UL;
        g_calls = 0;
        const ThingPointer& r = __median(p[0], p[1], p[2], comp);
        if (&r != &p[want] || g_calls != used) { std::printf("00805fe0_TEST FAIL\n"); return 1; }   // two or three draws
        ++seen[want];
    }
    if (!seen[0] || !seen[1] || !seen[2]) { std::printf("00805fe0_TEST FAIL\n"); return 1; }
    std::printf("00805fe0_TEST PASS\n");
    return 0;
}