// Behaviour test for 0x00AD6F10 STLport __introsort_loop over 8-byte records with a function-pointer comparator.
#include <cstdio>

struct CGroup { unsigned int BoneIndex; unsigned int BoneWeight; };
typedef bool (*PFNGroupCompare)(const CGroup&, const CGroup&);
static int g_calls = 0;
static bool CompareWeight(const CGroup& a, const CGroup& b) { ++g_calls; return a.BoneWeight < b.BoneWeight; }

static const CGroup& Median(const CGroup& a, const CGroup& b, const CGroup& c, PFNGroupCompare comp)
{
    if (comp(a, b)) { if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a; }
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
static CGroup* UnguardedPartition(CGroup* first, CGroup* last, CGroup pivot, PFNGroupCompare comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        CGroup t = *first; *first = *last; *last = t;
        ++first;
    }
}
static int g_partialSorts = 0;
static void PartialSortWhole(CGroup* first, CGroup* last, PFNGroupCompare comp)
{
    ++g_partialSorts;  // stand-in: selection sort of the whole range (middle == last)
    for (CGroup* i = first; i != last; ++i)
        for (CGroup* j = i + 1; j != last; ++j)
            if (comp(*j, *i)) { CGroup t = *i; *i = *j; *j = t; }
}
// reconstructed body
static void IntrosortLoop(CGroup* first, CGroup* last, int depth, PFNGroupCompare comp)
{
    while (last - first > 16) {
        if (depth == 0) { PartialSortWhole(first, last, comp); return; }
        --depth;
        CGroup* cut = UnguardedPartition(first, last, CGroup(Median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        IntrosortLoop(cut, last, depth, comp);
        last = cut;
    }
}
static void InsertionSort(CGroup* first, CGroup* last, PFNGroupCompare comp)
{
    for (CGroup* i = first + 1; i < last; ++i) {
        CGroup v = *i; CGroup* j = i;
        while (j != first && comp(v, *(j - 1))) { *j = *(j - 1); --j; }
        *j = v;
    }
}

int main()
{
    CGroup v[200];
    unsigned int seed = 12345;
    for (int i = 0; i < 200; ++i) { seed = seed * 1103515245u + 12345u; v[i].BoneIndex = i; v[i].BoneWeight = (seed >> 16) % 1000; }
    IntrosortLoop(v, v + 200, 14, CompareWeight);
    InsertionSort(v, v + 200, CompareWeight);
    for (int i = 1; i < 200; ++i) if (v[i - 1].BoneWeight > v[i].BoneWeight) { std::printf("00ad6f10_TEST FAIL\n"); return 1; }
    if (g_partialSorts != 0) { std::printf("00ad6f10_TEST FAIL\n"); return 1; }
    // depth budget 0: the whole range goes to partial_sort straight away
    CGroup w[40];
    for (int i = 0; i < 40; ++i) { w[i].BoneIndex = i; w[i].BoneWeight = 40 - i; }
    IntrosortLoop(w, w + 40, 0, CompareWeight);
    if (g_partialSorts != 1) { std::printf("00ad6f10_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 40; ++i) if (w[i].BoneWeight != (unsigned int)(i + 1)) { std::printf("00ad6f10_TEST FAIL\n"); return 1; }
    // 16 or fewer: untouched
    CGroup u[16];
    for (int i = 0; i < 16; ++i) { u[i].BoneIndex = i; u[i].BoneWeight = 16 - i; }
    IntrosortLoop(u, u + 16, 5, CompareWeight);
    if (u[0].BoneWeight != 16 || u[15].BoneWeight != 1) { std::printf("00ad6f10_TEST FAIL\n"); return 1; }
    std::printf("00ad6f10_TEST PASS\n");
    return 0;
}