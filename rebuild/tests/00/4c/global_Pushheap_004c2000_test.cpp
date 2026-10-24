// behaviour test for 0x004C2000 _Push_heap<CMeshInfo, CSortPrimaryMeshesLast>
#include <cstdio>
struct CMeshInfo {
    int Index;
    float SizeX;
    unsigned char PrimaryMesh : 1;
};
struct CSortPrimaryMeshesLast {
    bool operator()(const CMeshInfo& a, const CMeshInfo& b) const { return !a.PrimaryMesh; }
};
static void _Push_heap(CMeshInfo* first, long hole, long top, CMeshInfo value, CSortPrimaryMeshesLast comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
int main()
{
    CMeshInfo heap[4];
    for (int i = 0; i < 4; ++i) { heap[i].Index = i; heap[i].SizeX = (float)i; heap[i].PrimaryMesh = 1; }
    heap[1].PrimaryMesh = 0;   // parent of 3 is not primary: the value climbs past it
    CMeshInfo v; v.Index = 9; v.SizeX = 2.5f; v.PrimaryMesh = 1;
    CSortPrimaryMeshesLast c;
    _Push_heap(heap, 3, 0, v, c);
    if (heap[1].Index != 9 || heap[3].Index != 1 || heap[3].PrimaryMesh != 0 || heap[0].Index != 0) { std::printf("004c2000_TEST FAIL\n"); return 1; }
    _Push_heap(heap, 2, 0, v, c);
    if (heap[2].Index != 9) { std::printf("004c2000_TEST FAIL\n"); return 1; }
    std::printf("004c2000_TEST PASS\n");
    return 0;
}