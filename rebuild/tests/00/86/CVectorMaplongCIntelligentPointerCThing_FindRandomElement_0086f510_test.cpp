#include <cstdio>
// Behaviour model of CVectorMap<long, CIntelligentPointer<CThing> >::FindRandomElement: the [lower, upper) range of a
// sorted key vector, indexed by one LCG + rotate-right-13 step of the caller's seed.
static unsigned int GFROR13(unsigned int v) { return (v >> 13) | (v << 19); }
struct Rng {
    unsigned int Seed;
    unsigned int Next() { Seed = Seed * 0x24a1 + 0x24df; Seed = GFROR13(Seed); return Seed; }
    unsigned int Random(unsigned int n) { unsigned int r = Next(); return n ? r % n : 0; }
};
struct Entry { long key; int thing; };
struct Map {
    Entry e[6]; int n;
    const Entry* LowerBound(long k) const { int i = 0; while (i < n && e[i].key < k) ++i; return e + i; }
    const Entry* UpperBound(long k) const { int i = 0; while (i < n && e[i].key <= k) ++i; return e + i; }
    int FindRandomElement(long key, Rng& r) const
    {
        const Entry* first = LowerBound(key);
        const Entry* last = UpperBound(key);
        if (first != last) { long count = (long)(last - first); return first[r.Random(count)].thing; }
        return 0;
    }
};
int main()
{
    Map m; m.n = 6;
    long keys[6] = { 1, 2, 2, 2, 5, 9 };
    for (int i = 0; i < 6; ++i) { m.e[i].key = keys[i]; m.e[i].thing = 10 + i; }
    Rng r; r.Seed = 12345;
    Rng copy = r; unsigned int expect = copy.Next() % 3;
    int got = m.FindRandomElement(2, r);
    if (got != 11 + (int)expect || r.Seed != copy.Seed) { std::printf("0086f510_TEST FAIL\n"); return 1; }
    unsigned int before = r.Seed;
    if (m.FindRandomElement(3, r) != 0 || r.Seed != before) { std::printf("0086f510_TEST FAIL\n"); return 1; }
    if (m.FindRandomElement(9, r) != 15) { std::printf("0086f510_TEST FAIL\n"); return 1; }
    std::printf("0086f510_TEST PASS\n");
    return 0;
}