// behaviour test for 0x004494D0 std::__introsort_loop over pair<EGameAction, CCountedPointer<const CInputTypeBase> >: after the loop every run of at most 16 elements is bounded so a final insertion sort completes the order; with depth limit 0 a range above the threshold is fully sorted by the partial_sort fallback
#include <cstdio>
struct Entry { int Key; int Tag; };
static const Entry& Median(const Entry& a, const Entry& b, const Entry& c)
{
    if (a.Key < b.Key) { if (b.Key < c.Key) return b; else if (a.Key < c.Key) return c; else return a; }
    else if (a.Key < c.Key) return a; else if (b.Key < c.Key) return c; else return b;
}
static Entry* Partition(Entry* first, Entry* last, Entry pivot)
{
    for (;;) { while (first->Key < pivot.Key) ++first; --last; while (pivot.Key < last->Key) --last; if (!(first < last)) return first; Entry t = *first; *first = *last; *last = t; ++first; }
}
static void FullSort(Entry* first, Entry* last)             // stands for partial_sort(first, last, last)
{
    for (Entry* i = first; i < last; ++i) for (Entry* j = i + 1; j < last; ++j) if (j->Key < i->Key) { Entry t = *i; *i = *j; *j = t; }
}
static void IntrosortLoop(Entry* first, Entry* last, int depth)   // the function under test
{
    while (last - first > 16) {
        if (depth == 0) { FullSort(first, last); return; }
        --depth;
        Entry* cut = Partition(first, last, Median(*first, *(first + (last - first) / 2), *(last - 1)));
        IntrosortLoop(cut, last, depth);
        last = cut;
    }
}
int main()
{
    int fail = 0;
    Entry a[40]; for (int i = 0; i < 40; ++i) { a[i].Key = (i * 17 + 5) % 40; a[i].Tag = i; }
    IntrosortLoop(a, a + 40, 10);
    for (int i = 0; i < 40; ++i) for (int j = i + 17; j < 40; ++j) if (a[j].Key < a[i].Key) fail |= 1;   // only short-range disorder may remain
    int sum = 0; for (int i = 0; i < 40; ++i) sum += a[i].Tag; if (sum != 780) fail |= 2;
    Entry b[20]; for (int i = 0; i < 20; ++i) { b[i].Key = 19 - i; b[i].Tag = i; }
    IntrosortLoop(b, b + 20, 0);
    for (int i = 0; i < 20; ++i) if (b[i].Key != i) fail |= 4;
    if (fail) { std::printf("004494d0_TEST FAIL %d\n", fail); return 1; }
    std::printf("004494d0_TEST PASS\n");
    return 0;
}