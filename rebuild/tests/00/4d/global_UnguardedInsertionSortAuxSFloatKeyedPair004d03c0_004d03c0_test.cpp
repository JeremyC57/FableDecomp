#include <cstdio>

struct SFloatKeyedPair {
    float key;
    long  value;
    SFloatKeyedPair() : key(0), value(0) { }
    SFloatKeyedPair(const SFloatKeyedPair& o) : key(o.key), value(o.value) { }
};
struct SFloatKeyPairLess {
    bool operator()(const SFloatKeyedPair& a, const SFloatKeyedPair& b) const { return a.key < b.key; }
};
static void __unguarded_linear_insert(SFloatKeyedPair* last, SFloatKeyedPair val, SFloatKeyPairLess comp)
{
    SFloatKeyedPair* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
static void UnguardedInsertionSortAux(SFloatKeyedPair* first, SFloatKeyedPair* last, SFloatKeyedPair*, SFloatKeyPairLess comp)
{
    for (SFloatKeyedPair* i = first; i != last; ++i)
        __unguarded_linear_insert(i, SFloatKeyedPair(*i), comp);
}

int main()
{
    // slot 0 is the sentinel (smallest key) the unguarded form relies on: the guarded __insertion_sort sorted it first
    static const float keys[6] = { -100.0f, 3.5f, 1.0f, 2.25f, 1.0f, 0.5f };
    SFloatKeyedPair a[6];
    for (int i = 0; i < 6; ++i) { a[i].key = keys[i]; a[i].value = i; }
    SFloatKeyPairLess comp;
    UnguardedInsertionSortAux(a + 1, a + 1, 0, comp);          // empty range: no change
    bool ok = a[1].key == 3.5f;
    UnguardedInsertionSortAux(a + 1, a + 6, 0, comp);
    static const float sorted[6] = { -100.0f, 0.5f, 1.0f, 1.0f, 2.25f, 3.5f };
    static const long origin[6] = { 0, 5, 2, 4, 3, 1 };              // equal keys keep their order (strict less)
    for (int i = 0; i < 6; ++i) if (a[i].key != sorted[i] || a[i].value != origin[i]) ok = false;
    if (!ok) { std::printf("004d03c0_TEST FAIL\n"); return 1; }
    std::printf("004d03c0_TEST PASS\n");
    return 0;
}