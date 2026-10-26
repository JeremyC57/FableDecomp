// Behaviour test for 0x00470E9E __introsort_loop<pair<unsigned long, CDefString>*>: leaves runs of <= 16 unsorted inside but
// partitioned so that the final insertion sort orders the keys; depth 0 hands the whole range to partial_sort
#include <cstdio>
class CDefString { public: int TablePos; };
struct Entry { unsigned int first; CDefString second; };
struct Less { bool operator()(const Entry& a, const Entry& b) const { return a.first < b.first; } };
static int partialSorts = 0;
static const Entry& Median(const Entry& a, const Entry& b, const Entry& c, Less comp)
{
    if (comp(a, b)) { if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a; }
    else if (comp(a, c)) return a; else if (comp(b, c)) return c; else return b;
}
static Entry* UnguardedPartition(Entry* first, Entry* last, Entry pivot, Less comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        Entry t = *first; *first = *last; *last = t; ++first;
    }
}
static void PartialSort(Entry* first, Entry* middle, Entry* last, Less comp)
{
    ++partialSorts;
    for (Entry* i = first; i < last; ++i) for (Entry* j = i + 1; j < last; ++j) if (comp(*j, *i)) { Entry t = *i; *i = *j; *j = t; }
    (void)middle;
}
static void IntrosortLoop(Entry* first, Entry* last, Entry*, int depth, Less comp)
{
    while (last - first > 16) {
        if (depth == 0) { PartialSort(first, last, last, comp); return; }
        --depth;
        Entry* cut = UnguardedPartition(first, last, Entry(Median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        IntrosortLoop(cut, last, (Entry*)0, depth, comp);
        last = cut;
    }
}
static void InsertionSort(Entry* first, Entry* last)
{
    for (Entry* i = first + 1; i < last; ++i) { Entry v = *i; Entry* j = i; while (j > first && v.first < (j - 1)->first) { *j = *(j - 1); --j; } *j = v; }
}
int main()
{
    Entry v[100];
    unsigned int seed = 12345;
    for (int i = 0; i < 100; ++i) { seed = seed * 1103515245u + 12345u; v[i].first = (seed >> 16) % 1000; v[i].second.TablePos = (int)v[i].first * 3; }
    IntrosortLoop(v, v + 100, (Entry*)0, 12, Less());
    // every element of each <=16 block must be >= everything left of the block's start
    InsertionSort(v, v + 100);
    bool ok = partialSorts == 0;
    for (int i = 1; i < 100; ++i) if (v[i - 1].first > v[i].first) ok = false;
    for (int i = 0; i < 100; ++i) if (v[i].second.TablePos != (int)v[i].first * 3) ok = false;
    Entry w[40];
    for (int i = 0; i < 40; ++i) { w[i].first = 40 - i; w[i].second.TablePos = 0; }
    IntrosortLoop(w, w + 40, (Entry*)0, 0, Less());
    ok = ok && partialSorts == 1 && w[0].first == 1 && w[39].first == 40;
    IntrosortLoop(w, w + 10, (Entry*)0, 0, Less());
    ok = ok && partialSorts == 1;
    if (!ok) { std::printf("00471338_TEST FAIL\n"); return 1; }
    std::printf("00471338_TEST PASS\n");
    return 0;
}