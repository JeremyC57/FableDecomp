// behaviour test for 0x00A81BC0 std::__unguarded_partition over pair<long, CCountedPointer<CNavConnectorNode> >: every key left of the returned split is not above the pivot key, every key from it on is not below, the keys are a permutation of the input, and the pivot's own reference is dropped at the end
#include <cstdio>
struct Entry { int Key; int Tag; };
static int gPivotReleased = 0;
static Entry* Partition(Entry* first, Entry* last, Entry pivot)   // the function under test
{
    for (;;) {
        while (first->Key < pivot.Key) ++first;
        --last;
        while (pivot.Key < last->Key) --last;
        if (!(first < last)) { ++gPivotReleased; return first; }
        Entry t = *first; *first = *last; *last = t;
        ++first;
    }
}
int main()
{
    int fail = 0;
    Entry a[7] = { { 5, 0 }, { 9, 1 }, { 1, 2 }, { 7, 3 }, { 3, 4 }, { 8, 5 }, { 2, 6 } };
    Entry pivot = a[3];
    Entry* cut = Partition(a, a + 7, pivot);
    int n = (int)(cut - a), sum = 0;
    for (int i = 0; i < 7; ++i) sum += a[i].Tag;
    for (int i = 0; i < n; ++i) if (a[i].Key > pivot.Key) fail |= 1;
    for (int i = n; i < 7; ++i) if (a[i].Key < pivot.Key) fail |= 2;
    if (sum != 21 || n <= 0 || n >= 7) fail |= 4;
    if (gPivotReleased != 1) fail |= 8;
    if (fail) { std::printf("00a81bc0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00a81bc0_TEST PASS\n");
    return 0;
}