// Behaviour test for 0x004C1EC0 __unguarded_linear_insert<CMeshInfo*, CSortPrimaryMeshesFirst>: the comparator reads only the value's
// PrimaryMesh bit, so a value it orders "not before" is dropped straight into the hole at last and nothing moves.
// (A value it orders "before" would shift forever -- the unguarded loop has no exit for a left-only predicate -- so
// only the drop path is exercised.)
#include <cstdio>

struct CMeshInfo { long Index; float SizeX; unsigned char PrimaryMesh : 1; };
struct CSortPrimaryMeshesFirst {
    void* m_context;
    bool operator()(const CMeshInfo& a, const CMeshInfo& b) const { return a.PrimaryMesh; }
};
static void UnguardedLinearInsert(CMeshInfo* last, CMeshInfo value, CSortPrimaryMeshesFirst comp)
{
    CMeshInfo* next = last;
    --next;
    while (comp(value, *next)) { *last = *next; last = next; --next; }
    *last = value;
}

int main()
{
    CMeshInfo arr[3];
    for (int i = 0; i < 3; ++i) { arr[i].Index = i; arr[i].SizeX = 1.0f * i; arr[i].PrimaryMesh = 1; }
    CMeshInfo v; v.Index = 42; v.SizeX = 2.5f; v.PrimaryMesh = 0;
    CSortPrimaryMeshesFirst comp = { 0 };
    if (comp(v, arr[0])) { std::printf("004c1ec0_TEST FAIL\n"); return 1; }
    UnguardedLinearInsert(arr + 2, v, comp);
    if (arr[2].Index != 42 || arr[2].SizeX != 2.5f || arr[2].PrimaryMesh != 0 || arr[1].Index != 1 || arr[0].Index != 0) {
        std::printf("004c1ec0_TEST FAIL\n"); return 1;
    }
    CMeshInfo w; w.Index = 7; w.SizeX = 0.0f; w.PrimaryMesh = 1 - 0;
    if (!comp(w, arr[0])) { std::printf("004c1ec0_TEST FAIL\n"); return 1; }
    std::printf("004c1ec0_TEST PASS\n");
    return 0;
}