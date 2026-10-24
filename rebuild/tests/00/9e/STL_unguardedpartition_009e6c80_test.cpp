// Behaviour test for 0x009E6C80 STLport __unguarded_partition over (key, value) entries.
#include <cstdio>

struct SEntry { int key; int val; };
struct CCompare { bool operator()(const SEntry& a, const SEntry& b) const { return a.key < b.key; } };

static void iter_swap(SEntry* a, SEntry* b) { SEntry t = *a; *a = *b; *b = t; }

static SEntry* __unguarded_partition(SEntry* first, SEntry* last, SEntry pivot, CCompare comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}

int main()
{
    int keys[9] = { 7, 2, 9, 4, 5, 1, 8, 3, 6 };
    SEntry e[9];
    for (int i = 0; i < 9; ++i) { e[i].key = keys[i]; e[i].val = i; }
    SEntry pivot = e[4];                              // key 5 (median-of-three style value inside the range)
    SEntry* cut = __unguarded_partition(e, e + 9, pivot, CCompare());
    bool ok = cut > e && cut < e + 9;
    for (SEntry* p = e; p < cut; ++p) ok = ok && p->key <= 5;
    for (SEntry* p = cut; p < e + 9; ++p) ok = ok && p->key >= 5;
    int sum = 0;
    for (int i = 0; i < 9; ++i) sum += e[i].key;
    ok = ok && sum == 45;
    if (!ok) { std::printf("009e6c80_TEST FAIL\n"); return 1; }
    std::printf("009e6c80_TEST PASS\n");
    return 0;
}