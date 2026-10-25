// Link test: builds a max-heap of bone groups through the source object's std::__push_heap<CGroup*, int, CGroup, pfn>.
#include <cstdio>
namespace QuadricOptimiserInternals {
class COptimisedVertex {
public:
    class CGroup {
    public:
        unsigned long BoneIndex;
        float         BoneWeight;
    };
};
}
typedef QuadricOptimiserInternals::COptimisedVertex::CGroup CGroup;
typedef bool (__fastcall* PFNGroupCompare)(const CGroup&, const CGroup&);
namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __value, _Compare __comp);
}
static int g_calls = 0;
static bool __fastcall LessWeight(const CGroup& a, const CGroup& b) { ++g_calls; return a.BoneWeight < b.BoneWeight; }
int main()
{
    static const int keys[9] = { 4, 9, -1, 12, 3, 12, 0, 7, 20 };
    CGroup a[9];
    for (int i = 0; i < 9; ++i) {
        CGroup v; v.BoneIndex = 100 + i; v.BoneWeight = (float)keys[i];
        std::__push_heap(a, i, 0, v, (PFNGroupCompare)LessWeight);
        for (int k = 1; k <= i; ++k)
            if (a[(k - 1) / 2].BoneWeight < a[k].BoneWeight) { std::printf("00ad0920_TEST FAIL heap %d\n", i); return 1; }
    }
    if (a[0].BoneWeight != 20.0f || a[0].BoneIndex != 108) { std::printf("00ad0920_TEST FAIL root\n"); return 1; }
    int before = g_calls;
    CGroup v; v.BoneIndex = 7; v.BoneWeight = 1.0f;
    std::__push_heap(a, 3, 3, v, (PFNGroupCompare)LessWeight);   // hole == top: store only
    if (g_calls != before || a[3].BoneIndex != 7) { std::printf("00ad0920_TEST FAIL top\n"); return 1; }
    std::printf("00ad0920_TEST PASS\n");
    return 0;
}
