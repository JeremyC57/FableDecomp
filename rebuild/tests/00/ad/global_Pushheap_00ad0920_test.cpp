// behaviour test for 0x00AD0920 _Push_heap<COptimisedVertex::CGroup, bool (*)(const CGroup&, const CGroup&)>
#include <cstdio>
struct CGroup {
    unsigned int BoneIndex;
    float BoneWeight;
};
typedef bool (*TGroupCompare)(const CGroup& a, const CGroup& b);
static bool LighterThan(const CGroup& a, const CGroup& b) { return a.BoneWeight < b.BoneWeight; }
static void _Push_heap(CGroup* first, long hole, long top, CGroup value, TGroupCompare comp)
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
    static const float w[6] = { 0.25f, 0.5f, 0.125f, 0.75f, 0.0f, 1.0f };
    CGroup heap[6];
    for (int i = 0; i < 6; ++i) {
        CGroup g; g.BoneIndex = 20 + i; g.BoneWeight = w[i];
        _Push_heap(heap, i, 0, g, LighterThan);
        for (int k = 1; k <= i; ++k)
            if (LighterThan(heap[(k - 1) / 2], heap[k])) { std::printf("00ad0920_TEST FAIL\n"); return 1; }
    }
    if (heap[0].BoneIndex != 25 || heap[0].BoneWeight != 1.0f) { std::printf("00ad0920_TEST FAIL\n"); return 1; }
    // top bound respected: a push with hole == top stays put
    CGroup g; g.BoneIndex = 99; g.BoneWeight = 9.0f;
    _Push_heap(heap, 4, 4, g, LighterThan);
    if (heap[4].BoneIndex != 99 || heap[0].BoneIndex != 25) { std::printf("00ad0920_TEST FAIL\n"); return 1; }
    std::printf("00ad0920_TEST PASS\n");
    return 0;
}