// Behaviour test for 0x004C2B90 __unguarded_insertion_sort_aux<CMeshInfo*, CSortPrimaryMeshesFirst>: each element is
// re-inserted through the unguarded linear insert; with the left-only predicate a non-primary element is stored back
// into its own slot, so a run of non-primary infos is left exactly as it was. (A primary element would shift without
// end, so only non-primary runs are exercised.)
#include <cstdio>

struct CMeshInfo { long Index; float SizeX; unsigned char PrimaryMesh : 1; };
class CSortPrimaryMeshesFirst { public:
    void* m_context;
    bool operator()(const CMeshInfo& a, const CMeshInfo& b) const { return a.PrimaryMesh; }
};
static int g_compares = 0;
static void UnguardedLinearInsert(CMeshInfo* last, CMeshInfo value, CSortPrimaryMeshesFirst comp)
{
    CMeshInfo* next = last;
    --next;
    while ((++g_compares, comp(value, *next))) { *last = *next; last = next; --next; }
    *last = value;
}
static void UnguardedInsertionSortAux(CMeshInfo* first, CMeshInfo* last, CMeshInfo*, CSortPrimaryMeshesFirst comp)
{
    for (CMeshInfo* i = first; i != last; ++i)
        UnguardedLinearInsert(i, CMeshInfo(*i), comp);
}

int main()
{
    CMeshInfo arr[5];
    for (int i = 0; i < 5; ++i) { arr[i].Index = 10 + i; arr[i].SizeX = 0.5f * i; arr[i].PrimaryMesh = 0; }
    CSortPrimaryMeshesFirst comp = { 0 };
    UnguardedInsertionSortAux(arr + 1, arr + 5, 0, comp);
    for (int i = 0; i < 5; ++i)
        if (arr[i].Index != 10 + i || arr[i].SizeX != 0.5f * i || arr[i].PrimaryMesh != 0) { std::printf("004c2b90_TEST FAIL\n"); return 1; }
    if (g_compares != 4) { std::printf("004c2b90_TEST FAIL\n"); return 1; }
    UnguardedInsertionSortAux(arr, arr, 0, comp);          // empty range: nothing
    if (g_compares != 4) { std::printf("004c2b90_TEST FAIL\n"); return 1; }
    std::printf("004c2b90_TEST PASS\n");
    return 0;
}