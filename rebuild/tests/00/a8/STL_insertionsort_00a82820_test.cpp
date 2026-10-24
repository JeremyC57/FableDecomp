// Behaviour test for 0x00A82820 _STL::__insertion_sort over pair<unsigned __int64, bool> (out-of-line __linear_insert).
#include <cstdio>

typedef unsigned long long U64;
struct SKeyPairU64Bool {
    U64 first; bool second;
    SKeyPairU64Bool() : first(0), second(false) { }
    SKeyPairU64Bool(U64 f, bool s) : first(f), second(s) { }
    SKeyPairU64Bool(const SKeyPairU64Bool& o) : first(o.first), second(o.second) { }
};
struct CKeyPairCompareLess {
    void* m_context;
    bool operator()(const SKeyPairU64Bool& a, const SKeyPairU64Bool& b) const { return a.first < b.first; }
};

static int g_calls = 0;
static void __linear_insert(SKeyPairU64Bool* first, SKeyPairU64Bool* last, SKeyPairU64Bool value, CKeyPairCompareLess comp)
{
    ++g_calls;
    if (comp(value, *first)) {
        for (SKeyPairU64Bool* p = last; p != first; --p) *p = *(p - 1);
        *first = value;
    } else {
        SKeyPairU64Bool* next = last - 1;
        while (comp(value, *next)) { *last = *next; last = next; --next; }
        *last = value;
    }
}
static void __insertion_sort(SKeyPairU64Bool* first, SKeyPairU64Bool* last, CKeyPairCompareLess comp)
{
    if (first == last)
        return;
    for (SKeyPairU64Bool* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    SKeyPairU64Bool a[5];
    a[0] = SKeyPairU64Bool(0x100000000ull, true); a[1] = SKeyPairU64Bool(5, false); a[2] = SKeyPairU64Bool(0xFFFFFFFFull, true);
    a[3] = SKeyPairU64Bool(0x100000001ull, false); a[4] = SKeyPairU64Bool(1, true);
    CKeyPairCompareLess comp = { 0 };
    __insertion_sort(a, a + 5, comp);
    U64 keys[5] = { 1, 5, 0xFFFFFFFFull, 0x100000000ull, 0x100000001ull };
    bool flags[5] = { true, false, true, true, false };
    for (int k = 0; k < 5; ++k)
        if (a[k].first != keys[k] || a[k].second != flags[k]) { std::printf("00a82820_TEST FAIL\n"); return 1; }
    if (g_calls != 4) { std::printf("00a82820_TEST FAIL\n"); return 1; }
    std::printf("00a82820_TEST PASS\n");
    return 0;
}