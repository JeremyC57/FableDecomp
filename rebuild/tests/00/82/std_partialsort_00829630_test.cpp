// behaviour test for 0x00829630 std::__partial_sort over pair<long, CCountedPointer<CRegionConnectionNode> >: after the call [first, middle) holds the smallest keys in ascending order and the whole range is a permutation of the input
#include <cstdio>
struct Entry { int Key; int Tag; };
static void SiftDown(Entry* h, int n, int i)
{
    for (;;) { int c = 2 * i + 1; if (c >= n) return; if (c + 1 < n && h[c].Key < h[c + 1].Key) ++c; if (!(h[i].Key < h[c].Key)) return; Entry t = h[i]; h[i] = h[c]; h[c] = t; i = c; }
}
static void PartialSort(Entry* first, Entry* middle, Entry* last)   // the function under test
{
    int n = (int)(middle - first);
    for (int i = n / 2 - 1; i >= 0; --i) SiftDown(first, n, i);                       // make_heap
    for (Entry* it = middle; it < last; ++it)
        if (it->Key < first->Key) { Entry t = *it; *it = *first; *first = t; SiftDown(first, n, 0); }   // __pop_heap
    for (int k = n; k > 1; --k) { Entry t = first[0]; first[0] = first[k - 1]; first[k - 1] = t; SiftDown(first, k - 1, 0); }   // sort_heap
}
int main()
{
    int fail = 0;
    Entry a[8] = { { 7, 0 }, { 3, 1 }, { 9, 2 }, { 1, 3 }, { 8, 4 }, { 2, 5 }, { 6, 6 }, { 4, 7 } };
    PartialSort(a, a + 3, a + 8);
    if (a[0].Key != 1 || a[1].Key != 2 || a[2].Key != 3) fail |= 1;
    int sum = 0; for (int i = 0; i < 8; ++i) sum += a[i].Tag;
    if (sum != 28) fail |= 2;
    for (int i = 3; i < 8; ++i) if (a[i].Key < 3) fail |= 4;
    if (fail) { std::printf("00829630_TEST FAIL %d\n", fail); return 1; }
    std::printf("00829630_TEST PASS\n");
    return 0;
}