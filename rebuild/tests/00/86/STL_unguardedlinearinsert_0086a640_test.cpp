// Behaviour test for 0x0086A640: unguarded linear insert of a 12-byte {unsigned key, 2 words} entry by unsigned key
#include <cstdio>

struct Entry { unsigned long key; unsigned long a, b; };

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
    unsigned long keys[5] = { 0u, 3u, 5u, 0x80000000u, 0xF0000000u };   // sorted unsigned (high bit set entries are large)
    for (int i = 0; i < 5; ++i) { e[i].key = keys[i]; e[i].a = (unsigned long)i; e[i].b = 100u + (unsigned long)i; }
    Entry v; v.key = 4u; v.a = 77u; v.b = 88u;
    e[5] = v;
    UnguardedLinearInsert(e + 5, v);
    unsigned long want[6] = { 0u, 3u, 4u, 5u, 0x80000000u, 0xF0000000u };
    for (int i = 0; i < 6; ++i)
        if (e[i].key != want[i]) { std::printf("0086a640_TEST FAIL\n"); return 1; }
    if (e[2].a != 77u || e[2].b != 88u || e[3].a != 2u || e[5].b != 104u) { std::printf("0086a640_TEST FAIL\n"); return 1; }
    Entry w; w.key = 0xFFFFFFFFu; w.a = 1u; w.b = 2u;            // largest: stays at the end
    Entry f[3]; f[0].key = 1u; f[1].key = 2u; f[2] = w;
    UnguardedLinearInsert(f + 2, w);
    if (f[2].key != 0xFFFFFFFFu || f[1].key != 2u) { std::printf("0086a640_TEST FAIL\n"); return 1; }
    std::printf("0086a640_TEST PASS\n");
    return 0;
}