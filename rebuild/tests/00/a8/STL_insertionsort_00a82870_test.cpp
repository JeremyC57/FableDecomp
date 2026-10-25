// Behaviour test for 0x00A82870 std::__insertion_sort over pair<unsigned __int64, long> (out-of-line __linear_insert).
#include <cstdio>

typedef unsigned long long U64;
struct SKeyPairU64Long {
    U64 first; long second;
    SKeyPairU64Long() : first(0), second(0) { }
    SKeyPairU64Long(U64 f, long s) : first(f), second(s) { }
    SKeyPairU64Long(const SKeyPairU64Long& o) : first(o.first), second(o.second) { }
};
struct CKeyPairCompareLess {
    void* m_context;
    bool operator()(const SKeyPairU64Long& a, const SKeyPairU64Long& b) const { return a.first < b.first; }
};

static int g_calls = 0;
static void __linear_insert(SKeyPairU64Long* first, SKeyPairU64Long* last, SKeyPairU64Long value, CKeyPairCompareLess comp)
{
    ++g_calls;
    if (comp(value, *first)) {
        for (SKeyPairU64Long* p = last; p != first; --p) *p = *(p - 1);
        *first = value;
    } else {
        SKeyPairU64Long* next = last - 1;
        while (comp(value, *next)) { *last = *next; last = next; --next; }
        *last = value;
    }
}
static void __insertion_sort(SKeyPairU64Long* first, SKeyPairU64Long* last, CKeyPairCompareLess comp)
{
    if (first == last)
        return;
    for (SKeyPairU64Long* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    SKeyPairU64Long a[5];
    a[0] = SKeyPairU64Long(0x100000000ull, 1); a[1] = SKeyPairU64Long(5, 2); a[2] = SKeyPairU64Long(0xFFFFFFFFull, 3);
    a[3] = SKeyPairU64Long(0x100000001ull, 4); a[4] = SKeyPairU64Long(1, 5);
    CKeyPairCompareLess comp = { 0 };
    __insertion_sort(a, a + 5, comp);
    long order[5] = { 5, 2, 3, 1, 4 };
    for (int k = 0; k < 5; ++k)
        if (a[k].second != order[k]) { std::printf("00a82870_TEST FAIL\n"); return 1; }
    if (g_calls != 4) { std::printf("00a82870_TEST FAIL\n"); return 1; }
    std::printf("00a82870_TEST PASS\n");
    return 0;
}