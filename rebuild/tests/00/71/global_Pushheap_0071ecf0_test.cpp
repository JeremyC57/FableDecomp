// behaviour test for 0x0071ECF0 _Push_heap<pair<CSkeletalMorphs::CEntry,float>, CIsSkeletalMorphLowerPriorityThan>
#include <cstdio>
struct CSkeletalMorphsEntry { int MorphName; short Priority; bool SupressesLowerMorphs; };
struct SMorphWeight { CSkeletalMorphsEntry first; float second; };
class CIsSkeletalMorphLowerPriorityThan { public:
    char m_tag;
    bool operator()(const SMorphWeight& a, const SMorphWeight& b) const { return a.first.Priority < b.first.Priority; }
};
static void _Push_heap(SMorphWeight* first, long hole, long top, SMorphWeight value, CIsSkeletalMorphLowerPriorityThan comp)
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
    static const short pri[7] = { 3, -2, 7, 0, 5, 9, 1 };
    SMorphWeight heap[7];
    CIsSkeletalMorphLowerPriorityThan c; c.m_tag = 0;
    for (int i = 0; i < 7; ++i) {
        SMorphWeight v; v.first.MorphName = 100 + i; v.first.Priority = pri[i]; v.first.SupressesLowerMorphs = (i & 1) != 0; v.second = 0.5f * i;
        _Push_heap(heap, i, 0, v, c);
        for (int k = 1; k <= i; ++k)
            if (c(heap[(k - 1) / 2], heap[k])) { std::printf("0071ecf0_TEST FAIL\n"); return 1; }
    }
    if (heap[0].first.Priority != 9 || heap[0].first.MorphName != 105 || heap[0].second != 2.5f || !heap[0].first.SupressesLowerMorphs) { std::printf("0071ecf0_TEST FAIL\n"); return 1; }
    std::printf("0071ecf0_TEST PASS\n");
    return 0;
}