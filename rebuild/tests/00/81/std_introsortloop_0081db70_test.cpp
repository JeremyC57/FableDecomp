// Behaviour test for 0x0081DB70 __introsort_loop<pair<EHeroMorphType,CTextureMorphEntry>*> (STLport __introsort_loop over 24-byte (key, entry) pairs).
#include <cstdio>
#include <cstdlib>
typedef int TKey;
struct CEntry { int Payload; unsigned char _rest[16]; };
struct TPair { TKey first; CEntry second; };
struct CLess { bool operator()(const TPair& a, const TPair& b) const { return a.first < b.first; } };
static int g_partial = 0;
static const TPair& median(const TPair& a, const TPair& b, const TPair& c, CLess comp)
{
    if (comp(a, b))
        if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a;
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
static TPair* unguarded_partition(TPair* first, TPair* last, TPair pivot, CLess comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        TPair t = *first; *first = *last; *last = t;
        ++first;
    }
}
static void partial_sort_all(TPair* first, TPair* last, CLess comp)   // stands in for the heap sort
{
    ++g_partial;
    for (TPair* i = first; i != last; ++i)
        for (TPair* j = i + 1; j != last; ++j)
            if (comp(*j, *i)) { TPair t = *i; *i = *j; *j = t; }
}
static void introsort_loop(TPair* first, TPair* last, int depth, CLess comp)
{
    while (last - first > 16) {
        if (depth == 0) { partial_sort_all(first, last, comp); return; }
        --depth;
        TPair* cut = unguarded_partition(first, last,
                                         median(*first, *(first + (last - first) / 2), *(last - 1), comp), comp);
        introsort_loop(cut, last, depth, comp);
        last = cut;
    }
}
static void insertion_sort(TPair* first, TPair* last, CLess comp)
{
    for (TPair* i = first + 1; i < last; ++i) {
        TPair v = *i; TPair* j = i;
        while (j != first && comp(v, *(j - 1))) { *j = *(j - 1); --j; }
        *j = v;
    }
}
int main()
{
    int bad = 0;
    if (sizeof(TPair) != 24) bad = 1;
    static TPair v[200];
    unsigned int seed = 12345;
    for (int round = 0; round < 3 && !bad; ++round) {
        for (int i = 0; i < 200; ++i) { seed = seed * 1103515245u + 12345u; v[i].first = (TKey)((seed >> 16) % 97); v[i].second.Payload = (int)v[i].first * 3; }
        g_partial = 0;
        introsort_loop(v, v + 200, round == 2 ? 0 : 14, CLess());
        insertion_sort(v, v + 200, CLess());
        for (int i = 1; i < 200; ++i) if (v[i].first < v[i - 1].first) bad = 1;
        for (int i = 0; i < 200; ++i) if (v[i].second.Payload != (int)v[i].first * 3) bad = 1;
        if (round == 2 && g_partial != 1) bad = 1;
    }
    if (bad) { std::printf("0081db70_TEST FAIL\n"); return 1; }
    std::printf("0081db70_TEST PASS\n");
    return 0;
}