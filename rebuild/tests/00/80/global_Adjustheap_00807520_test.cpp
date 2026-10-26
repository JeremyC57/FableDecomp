// behaviour test for 00807520: _Adjust_heap over intelligent thing pointers under the coin-flip
// CDoNotUseBinaryPredicate: each comparison advances the world seed (ROR13(seed*0x24a1+0x24df))
// and answers "low bit clear"; the sift must keep every element exactly once.
#include <cstdio>
class CThing { public: int Id; };
struct ThingPointer {
    CThing* Item;
    ThingPointer() : Item(0) { }
    ThingPointer(const ThingPointer& p) : Item(p.Item) { }
    ThingPointer& operator=(const ThingPointer& p) { Item = p.Item; return *this; }
};
static unsigned int GFROR13(unsigned int value) { return (value >> 13) | (value << 19); }
struct CRandomSeed {
    unsigned int Seed;
    unsigned int Random()
    {
        Seed = (Seed * 0x24a1 + 0x24df);
        Seed = GFROR13(Seed);
        return Seed;
    }
};
static CRandomSeed g_seed;
static int g_compares = 0;
struct CDoNotUseBinaryPredicate {
    char m_tag;
    bool operator()(const ThingPointer&, const ThingPointer&) const
    {
        ++g_compares;
        const unsigned int& bit = g_seed.Random() & 1;
        return bit == 0;
    }
};
static void PushHeap(ThingPointer* first, long hole, long top, ThingPointer value, CDoNotUseBinaryPredicate pred)
{
    long parent = (hole - 1) / 2;
    while (top < hole && pred(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
static void AdjustHeap(ThingPointer* first, long hole, long bottom, ThingPointer value, CDoNotUseBinaryPredicate pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    PushHeap(first, hole, top, value, pred);
}
int main()
{
    CThing things[12];
    ThingPointer a[12];
    long i = 0;
    long round = 0;
    int ok = 1;
    g_seed.Seed = 12345;
    for (i = 0; i < 12; ++i) { things[i].Id = (int)i; a[i].Item = &things[i]; }
    CDoNotUseBinaryPredicate pred;
    pred.m_tag = 0;
    unsigned int expect = 12345;
    for (round = 0; round < 20; ++round) {
        ThingPointer value = a[11];
        a[11] = a[0];
        g_compares = 0;
        AdjustHeap(a, 0, 11, value, pred);
        for (i = 0; i < g_compares; ++i) expect = GFROR13((expect * 0x24a1 + 0x24df));
        if (g_seed.Seed != expect || g_compares < 2) ok = 0;
        int seen[12];
        long j = 0;
        for (j = 0; j < 12; ++j) seen[j] = 0;
        for (j = 0; j < 12; ++j) if (a[j].Item) ++seen[a[j].Item->Id];
        for (j = 0; j < 12; ++j) if (seen[j] != 1) ok = 0;
    }
    if (GFROR13(0x00002000U) != 1U) ok = 0;
    if (!ok) { std::printf("00807520_TEST FAIL\n"); return 1; }
    std::printf("00807520_TEST PASS\n");
    return 0;
}