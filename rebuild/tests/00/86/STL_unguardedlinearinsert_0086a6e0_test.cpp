// Behaviour test for 0x0086A6E0: unguarded linear insert of a 16-byte {signed key, 3 words} entry by signed key
#include <cstdio>

struct Entry { long key; unsigned long a, b, c; };

static void UnguardedLinearInsert(Entry* last, Entry val)
{
    Entry* next = last;
    --next;
    while (val.key < next->key) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

int main()
{
    Entry e[6];
    long keys[5] = { -2000000000L, -5L, 0L, 3L, 5L };                      // sorted signed (negative keys are small)
    for (int i = 0; i < 5; ++i) { e[i].key = keys[i]; e[i].a = (unsigned long)i; e[i].b = 100u + (unsigned long)i; e[i].c = 7u; }
    Entry v; v.key = -1L; v.a = 77u; v.b = 88u; v.c = 99u;
    e[5] = v;
    UnguardedLinearInsert(e + 5, v);
    long want[6] = { -2000000000L, -5L, -1L, 0L, 3L, 5L };
    for (int i = 0; i < 6; ++i)
        if (e[i].key != want[i]) { std::printf("0086a6e0_TEST FAIL\n"); return 1; }
    if (e[2].a != 77u || e[2].b != 88u || e[2].c != 99u || e[3].a != 2u || e[5].b != 104u) { std::printf("0086a6e0_TEST FAIL\n"); return 1; }
    Entry w; w.key = 2000000000L; w.a = 1u; w.b = 2u; w.c = 3u;   // largest: stays at the end
    Entry f[3]; f[0].key = -1L; f[1].key = 2L; f[2] = w;
    UnguardedLinearInsert(f + 2, w);
    if (f[2].key != 2000000000L || f[1].key != 2L) { std::printf("0086a6e0_TEST FAIL\n"); return 1; }
    std::printf("0086a6e0_TEST PASS\n");
    return 0;
}