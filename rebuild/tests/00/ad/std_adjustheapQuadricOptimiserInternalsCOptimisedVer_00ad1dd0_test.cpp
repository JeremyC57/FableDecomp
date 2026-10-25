// Link test: heap-sorts bone groups through the source object's std::__adjust_heap<CGroup*, int, CGroup, pfn>,
// supplying the STLport __push_heap body (0x00AD0920) and a __fastcall weight comparator.
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
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value, _Compare __comp);
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __value, _Compare __comp);
template <> void __fastcall __push_heap<CGroup*, int, CGroup, PFNGroupCompare>(CGroup* first, int hole, int top, CGroup value, PFNGroupCompare comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
}
static bool __fastcall LessWeight(const CGroup& a, const CGroup& b) { return a.BoneWeight < b.BoneWeight; }
int main()
{
    static const int keys[11] = { 7, -2, 15, 3, 3, 0, 42, -9, 11, 5, 8 };
    CGroup a[11];
    int n = 11, i;
    for (i = 0; i < n; ++i) { a[i].BoneWeight = keys[i] * 0.25f; a[i].BoneIndex = (unsigned long)(keys[i] + 1000); }
    PFNGroupCompare comp = LessWeight;
    for (i = n / 2; 0 < i; ) { --i; CGroup v = a[i]; std::__adjust_heap(a, i, n, v, comp); }
    for (i = 1; i < n; ++i) if (comp(a[0], a[i])) { std::printf("00ad1dd0_TEST FAIL heap\n"); return 1; }
    for (int last = n; 1 < last; --last) {
        CGroup v = a[last - 1];
        a[last - 1] = a[0];
        std::__adjust_heap(a, 0, last - 1, v, comp);
    }
    for (i = 1; i < n; ++i) if (comp(a[i], a[i - 1])) { std::printf("00ad1dd0_TEST FAIL order\n"); return 1; }
    for (i = 0; i < n; ++i) if (a[i].BoneIndex != (unsigned long)((int)(a[i].BoneWeight * 4.0f) + 1000)) { std::printf("00ad1dd0_TEST FAIL intact\n"); return 1; }
    if (a[0].BoneWeight != -9 * 0.25f || a[n - 1].BoneWeight != 42 * 0.25f) { std::printf("00ad1dd0_TEST FAIL ends\n"); return 1; }
    std::printf("00ad1dd0_TEST PASS\n");
    return 0;
}
