// Behaviour test for 0x00A73A00 _STL::__introsort_loop over (CCharString key, record) pairs: string keys under the
// CCharString order (null first); the loop plus a final insertion sort orders the range, depth 0 partial-sorts it.
#include <cstdio>
static bool DataLess(const char* a, const char* b)
{
    for (;;) {
        if (*a == 0 && *b == 0) return false;
        if (*a < *b) return true;
        if (*a > *b) return false;
        ++a; ++b;
    }
}
struct SRec { const char* key; int tag; };
struct CLess {
    bool operator()(const SRec& a, const SRec& b) const
    {
        if (b.key == a.key) return false;
        if (a.key == 0) return true;
        if (b.key == 0) return false;
        return DataLess(a.key, b.key);
    }
};
static void AdjustHeap(SRec* first, int hole, int len, SRec val, CLess comp)
{
    int top = hole;
    int child = 2 * hole + 2;
    while (child < len) {
        if (comp(first[child], first[child - 1])) child--;
        first[hole] = first[child];
        hole = child;
        child = 2 * (child + 1);
    }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], val)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = val;
}
static void PartialSort(SRec* first, SRec* middle, SRec* last, SRec*, CLess comp)
{
    int len = (int)(middle - first);
    if (len >= 2)
        for (int parent = (len - 2) / 2; ; parent--) {
            AdjustHeap(first, parent, len, first[parent], comp);
            if (parent == 0) break;
        }
    for (SRec* i = middle; i < last; ++i)
        if (comp(*i, *first)) { SRec v = *i; *i = *first; AdjustHeap(first, 0, len, v, comp); }
    while (middle - first > 1) { --middle; SRec v = *middle; *middle = *first; AdjustHeap(first, 0, (int)(middle - first), v, comp); }
}
static const SRec& Median(const SRec& a, const SRec& b, const SRec& c, CLess comp)
{
    if (comp(a, b))
        if (comp(b, c)) return b;
        else if (comp(a, c)) return c;
        else return a;
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
static SRec* UnguardedPartition(SRec* first, SRec* last, SRec pivot, CLess comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        SRec t = *first; *first = *last; *last = t;
        ++first;
    }
}
static int g_partialSorts = 0;
// the function under test (same shape as the source)
static void IntrosortLoop(SRec* first, SRec* last, SRec*, int depth_limit, CLess comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) {
            ++g_partialSorts;
            PartialSort(first, last, last, (SRec*)0, comp);
            return;
        }
        --depth_limit;
        SRec* cut = UnguardedPartition(first, last, SRec(Median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        IntrosortLoop(cut, last, (SRec*)0, depth_limit, comp);
        last = cut;
    }
}
static void InsertionSort(SRec* first, SRec* last, CLess comp)
{
    for (SRec* i = first + 1; i < last; ++i) {
        SRec v = *i; SRec* j = i;
        while (j > first && comp(v, *(j - 1))) { *j = *(j - 1); --j; }
        *j = v;
    }
}
static bool Sorted(SRec* first, SRec* last, CLess comp)
{
    for (SRec* i = first + 1; i < last; ++i) if (comp(*i, *(i - 1))) return false;
    return true;
}
static char g_text[120][4];
int main()
{
    SRec v[120];
    unsigned int seed = 777;
    for (int i = 0; i < 120; ++i) {
        seed = seed * 1103515245u + 12345u;
        g_text[i][0] = (char)('a' + (seed >> 9) % 26); g_text[i][1] = (char)('a' + (seed >> 17) % 26); g_text[i][2] = 0;
        v[i].key = (i % 37 == 5) ? 0 : g_text[i]; v[i].tag = i;
    }
    IntrosortLoop(v, v + 120, (SRec*)0, 12, CLess());
    InsertionSort(v, v + 120, CLess());
    if (!Sorted(v, v + 120, CLess()) || v[0].key != 0 || g_partialSorts != 0) { std::printf("00a73a00_TEST FAIL\n"); return 1; }
    IntrosortLoop(v + 50, v + 90, (SRec*)0, 0, CLess());
    if (!Sorted(v, v + 120, CLess()) || g_partialSorts != 1) { std::printf("00a73a00_TEST FAIL\n"); return 1; }
    std::printf("00a73a00_TEST PASS\n");
    return 0;
}