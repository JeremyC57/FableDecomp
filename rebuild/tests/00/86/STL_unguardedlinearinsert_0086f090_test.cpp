// Behaviour test for 0x0086F090: unguarded linear insert of a 12-byte {signed key, 2 words} entry by signed key
#include <cstdio>

struct Entry { long key; long type; long anim; };

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
    for (int i = 0; i < 5; ++i) { e[i].key = keys[i]; e[i].type = (long)i; e[i].anim = 100 + i; }
    Entry v; v.key = -1L; v.type = 77; v.anim = 88;
    e[5] = v;
    UnguardedLinearInsert(e + 5, v);
    long want[6] = { -2000000000L, -5L, -1L, 0L, 3L, 5L };
    for (int i = 0; i < 6; ++i)
        if (e[i].key != want[i]) { std::printf("0086f090_TEST FAIL\n"); return 1; }
    if (e[2].type != 77 || e[2].anim != 88 || e[3].type != 2 || e[5].anim != 104) { std::printf("0086f090_TEST FAIL\n"); return 1; }
    Entry w; w.key = 2000000000L; w.type = 1; w.anim = 2;   // largest: stays at the end
    Entry f[3]; f[0].key = -1L; f[1].key = 2L; f[2] = w;
    UnguardedLinearInsert(f + 2, w);
    if (f[2].key != 2000000000L || f[1].key != 2L) { std::printf("0086f090_TEST FAIL\n"); return 1; }
    std::printf("0086f090_TEST PASS\n");
    return 0;
}