#include <cstdio>

struct SULongKeyedPair {
    unsigned long key;
    long          value;
    SULongKeyedPair() : key(0), value(0) { }
    SULongKeyedPair(const SULongKeyedPair& o) : key(o.key), value(o.value) { }
};
struct SULongKeyPairLess {
    bool operator()(const SULongKeyedPair& a, const SULongKeyedPair& b) const { return a.key < b.key; }
};
static void __unguarded_linear_insert(SULongKeyedPair* last, SULongKeyedPair val, SULongKeyPairLess comp)
{
    SULongKeyedPair* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
static void UnguardedInsertionSortAux(SULongKeyedPair* first, SULongKeyedPair* last, SULongKeyedPair*, SULongKeyPairLess comp)
{
    for (SULongKeyedPair* i = first; i != last; ++i)
        __unguarded_linear_insert(i, SULongKeyedPair(*i), comp);
}

int main()
{
    // slot 0 is the sentinel; 0x80000000 checks the unsigned order (a signed compare would sort it first)
    static const unsigned long keys[6] = { 0u, 0x80000000u, 7u, 3u, 7u, 1u };
    SULongKeyedPair a[6];
    for (int i = 0; i < 6; ++i) { a[i].key = keys[i]; a[i].value = i; }
    SULongKeyPairLess comp;
    UnguardedInsertionSortAux(a + 1, a + 6, 0, comp);
    static const unsigned long sorted[6] = { 0u, 1u, 3u, 7u, 7u, 0x80000000u };
    static const long origin[6] = { 0, 5, 3, 2, 4, 1 };
    bool ok = true;
    for (int i = 0; i < 6; ++i) if (a[i].key != sorted[i] || a[i].value != origin[i]) ok = false;
    UnguardedInsertionSortAux(a + 2, a + 2, 0, comp);
    if (a[2].key != 3u) ok = false;
    if (!ok) { std::printf("00471239_TEST FAIL\n"); return 1; }
    std::printf("00471239_TEST PASS\n");
    return 0;
}