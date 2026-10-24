// Behaviour test for 0x0076BBC0 STLport __partial_sort over pair<unsigned long, CParticleUpdateParamDesc> (unsigned key order)
#include <cstdio>

struct SElem {                                  // pair<unsigned long, 12-byte payload>
    unsigned int first;
    unsigned int a, b, c;
};
struct SLess { bool operator()(const SElem& x, const SElem& y) const { return x.first < y.first; } };

static const SElem& Median(const SElem& a, const SElem& b, const SElem& c, SLess comp)
{
    if (comp(a, b))
        if (comp(b, c)) return b;
        else if (comp(a, c)) return c;
        else return a;
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
static SElem* UnguardedPartition(SElem* first, SElem* last, SElem pivot, SLess comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        SElem t = *first; *first = *last; *last = t;
        ++first;
    }
}
static void PushHeap(SElem* first, int hole, int top, SElem val, SLess comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], val)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = val;
}
static void AdjustHeap(SElem* first, int hole, int len, SElem val, SLess comp)
{
    int top = hole, second = 2 * hole + 2;
    while (second < len) {
        if (comp(first[second], first[second - 1])) second--;
        first[hole] = first[second]; hole = second; second = 2 * (second + 1);
    }
    if (second == len) { first[hole] = first[second - 1]; hole = second - 1; }
    PushHeap(first, hole, top, val, comp);
}
static void MakeHeap(SElem* first, SElem* last, SLess comp)
{
    int len = (int)(last - first);
    if (len < 2) return;
    for (int parent = (len - 2) / 2; ; --parent) {
        AdjustHeap(first, parent, len, first[parent], comp);
        if (parent == 0) return;
    }
}
static void SortHeap(SElem* first, SElem* last, SLess comp)
{
    while (last - first > 1) {
        --last;
        SElem v = *last; *last = *first;
        AdjustHeap(first, 0, (int)(last - first), v, comp);
    }
}
static void PartialSort(SElem* first, SElem* middle, SElem* last, SLess comp)
{
    MakeHeap(first, middle, comp);
    for (SElem* i = middle; i < last; ++i)
        if (comp(*i, *first)) {
            SElem v = *i;
            *i = *first;
            AdjustHeap(first, 0, (int)(middle - first), v, comp);
        }
    SortHeap(first, middle, comp);
}
static void IntrosortLoop(SElem* first, SElem* last, int depth, SLess comp)
{
    while (last - first > 16) {
        if (depth == 0) { PartialSort(first, last, last, comp); return; }
        --depth;
        SElem* cut = UnguardedPartition(first, last, SElem(Median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        IntrosortLoop(cut, last, depth, comp);
        last = cut;
    }
}
static void InsertionSort(SElem* first, SElem* last, SLess comp)
{
    if (first == last) return;
    for (SElem* i = first + 1; i != last; ++i) {
        SElem v = *i; SElem* j = i;
        while (j != first && comp(v, *(j - 1))) { *j = *(j - 1); --j; }
        *j = v;
    }
}
static unsigned int g_seed = 12345;
static unsigned int Rand() { g_seed = g_seed * 1103515245u + 12345u; return (g_seed >> 8) & 0xffffffu; }
static bool CheckSorted(SElem* v, int n)
{
    for (int i = 1; i < n; ++i) if (v[i - 1].first > v[i].first) return false;
    for (int i = 0; i < n; ++i) if (v[i].a != v[i].first * 3u || v[i].c != v[i].first + 7u) return false;   // payload kept with key
    return true;
}
static void Fill(SElem* v, int n, bool highBit)
{
    for (int i = 0; i < n; ++i) {
        v[i].first = Rand() % 1000u + (highBit && (i & 1) ? 0x80000000u : 0u);
        v[i].a = v[i].first * 3u; v[i].b = i; v[i].c = v[i].first + 7u;
    }
}
int main()
{
    static SElem v[64], w[64];
    Fill(v, 64, true);
    for (int i = 0; i < 64; ++i) w[i] = v[i];
    PartialSort(v, v + 10, v + 64, SLess());          // the 10 smallest, sorted, in front
    InsertionSort(w, w + 64, SLess());
    for (int i = 0; i < 10; ++i) if (v[i].first != w[i].first) { std::printf("0076bbc0_TEST FAIL\n"); return 1; }
    if (!CheckSorted(v, 10)) { std::printf("0076bbc0_TEST FAIL\n"); return 1; }
    PartialSort(v, v + 64, v + 64, SLess());          // full heap sort
    if (!CheckSorted(v, 64)) { std::printf("0076bbc0_TEST FAIL\n"); return 1; }
    std::printf("0076bbc0_TEST PASS\n");
    return 0;
}