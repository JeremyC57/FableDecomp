#include <cstdio>

// Behaviour model of CVtable_012b2f88::Compare (0x00C19E90): the item's two key words, second fetched first,
// handed to the comparer as (first, second).
static int g_order[4]; static int g_n = 0;
struct Pair {
    int a, b;
    virtual int* GetFirst() { g_order[g_n++] = 1; return &a; }
    virtual int* GetSecond() { g_order[g_n++] = 2; return &b; }
    virtual ~Pair() { }
};
struct Comparer {
    virtual int Compare(int first, int second) { return first * 100 + second; }
    virtual ~Comparer() { }
};
struct Adaptor {
    Comparer* m_pComparer;
    int Compare(Pair* item) { int second = *item->GetSecond(); return m_pComparer->Compare(*item->GetFirst(), second); }
};

int main()
{

    Pair p; p.a = 4; p.b = 7; Comparer c; Adaptor ad; ad.m_pComparer = &c;
    if (ad.Compare(&p) != 407) { std::printf("00c19e90_TEST FAIL\n"); return 1; }
    if (g_n != 2 || g_order[0] != 2 || g_order[1] != 1) { std::printf("00c19e90_TEST FAIL\n"); return 1; }
    std::printf("00c19e90_TEST PASS\n");
    return 0;
}