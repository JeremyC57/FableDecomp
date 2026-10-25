// Behaviour test for 0x0086B3B0: unguarded partition of 16-byte {signed key, 3 words} entries around a pivot key
#include <cstdio>

struct Entry { long key; long a; long b; bool c; };

static Entry* UnguardedPartition(Entry* first, Entry* last, Entry pivot)
{
    for (;;) {
        while (first->key < pivot.key) ++first;
        --last;
        while (pivot.key < last->key) --last;
        if (!(first < last)) return first;
        Entry t = *first; *first = *last; *last = t;
        ++first;
    }
}

int main()
{
    long keys[9] = { 5, -3, 8, 1, 9, -7, 4, 0, 6 };
    Entry e[9];
    for (int i = 0; i < 9; ++i) { e[i].key = keys[i]; e[i].a = 10 * keys[i]; e[i].b = i; e[i].c = (i & 1) != 0; }
    Entry pivot = e[0];                                       // key 5 (present, so both scans stay in range)
    Entry* cut = UnguardedPartition(e, e + 9, pivot);
    int k = (int)(cut - e);
    if (k <= 0 || k >= 9) { std::printf("0086b3b0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < k; ++i) if (e[i].key > 5) { std::printf("0086b3b0_TEST FAIL\n"); return 1; }
    for (int i = k; i < 9; ++i) if (e[i].key < 5) { std::printf("0086b3b0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 9; ++i) if (e[i].a != 10 * e[i].key) { std::printf("0086b3b0_TEST FAIL\n"); return 1; }   // records moved whole
    long sum = 0; for (int i = 0; i < 9; ++i) sum += e[i].b;
    if (sum != 36) { std::printf("0086b3b0_TEST FAIL\n"); return 1; }
    std::printf("0086b3b0_TEST PASS\n");
    return 0;
}