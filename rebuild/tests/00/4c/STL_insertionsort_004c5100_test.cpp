// Behaviour test for 0x004C5100 std::__insertion_sort over NGraphicAppearance::CMeshInfo with CSortPrimaryMeshesFirst (inlined __linear_insert).
#include <cstdio>

struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct CMeshInfo { long Index; float SizeX; unsigned char PrimaryMesh : 1; };
struct CSortPrimaryMeshesFirst {
    void* m_context;
    bool operator()(const CMeshInfo& a, const CMeshInfo& b) const { return a.PrimaryMesh; }
};

static int g_backward = 0, g_unguarded = 0;
static CMeshInfo* __copy_backward(CMeshInfo* first, CMeshInfo* last, CMeshInfo* result, const random_access_iterator_tag&, int*)
{
    ++g_backward;
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}
// The retail unguarded insert tests the same single bit of the value (a non-primary value never sinks: it stays in place).
static void __unguarded_linear_insert(CMeshInfo* last, CMeshInfo value, CSortPrimaryMeshesFirst comp)
{
    ++g_unguarded;
    *last = value;
}
inline CMeshInfo* copy_backward(CMeshInfo* first, CMeshInfo* last, CMeshInfo* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}
inline void __linear_insert(CMeshInfo* first, CMeshInfo* last, CMeshInfo value, CSortPrimaryMeshesFirst comp)
{
    if (comp(value, *first)) {
        copy_backward(first, last, last + 1);
        *first = value;
    }
    else
        __unguarded_linear_insert(last, value, comp);
}
static void __insertion_sort(CMeshInfo* first, CMeshInfo* last, CSortPrimaryMeshesFirst comp)
{
    if (first == last)
        return;
    for (CMeshInfo* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    CMeshInfo a[5];
    unsigned char primary[5] = { 0, 1, 0, 1, 0 };
    for (int k = 0; k < 5; ++k) { a[k].Index = k; a[k].SizeX = 1.5f * k; a[k].PrimaryMesh = primary[k]; }
    CSortPrimaryMeshesFirst comp = { 0 };
    __insertion_sort(a, a + 5, comp);
    // each primary entry is moved to the front, the non-primary ones keep their relative order behind them
    long order[5] = { 3, 1, 0, 2, 4 };
    for (int k = 0; k < 5; ++k)
        if (a[k].Index != order[k] || a[k].SizeX != 1.5f * order[k]) { std::printf("004c5100_TEST FAIL\n"); return 1; }
    if (!a[0].PrimaryMesh || !a[1].PrimaryMesh || a[2].PrimaryMesh || a[4].PrimaryMesh) { std::printf("004c5100_TEST FAIL\n"); return 1; }
    if (g_backward != 2 || g_unguarded != 2) { std::printf("004c5100_TEST FAIL\n"); return 1; }
    std::printf("004c5100_TEST PASS\n");
    return 0;
}