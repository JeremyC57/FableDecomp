// Link test: calls the source object's std::__insertion_sort<CBlockInfo*, less<CBlockInfo> > and supplies the
// out-of-line __unguarded_linear_insert 0x00ADAA30 (counted).
#include <cstdio>
class CVertexGroupBlockOptimiser {
public:
    class CBlockInfo {
    public:
        unsigned long BlockIndex;
        long          Cost;
        long          Benefit;
        bool operator<(const CBlockInfo& other) const { return Cost < other.Cost; }
    };
};
typedef CVertexGroupBlockOptimiser::CBlockInfo CBlockInfo;
namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);
}
static int g_unguarded = 0;
namespace std {
template <> void __fastcall __unguarded_linear_insert<CBlockInfo*, CBlockInfo, less<CBlockInfo> >(CBlockInfo* last, CBlockInfo val, less<CBlockInfo> comp)
{
    ++g_unguarded;
    CBlockInfo* next = last - 1;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
}
int main()
{
    CBlockInfo a[6];
    long costs[6] = { 5, -3, 8, 5, -10, 0 };
    for (int k = 0; k < 6; ++k) { a[k].BlockIndex = k; a[k].Cost = costs[k]; a[k].Benefit = 100 + k; }
    std::__insertion_sort(a, a + 6, std::less<CBlockInfo>());
    long sorted[6] = { -10, -3, 0, 5, 5, 8 };
    unsigned long origin[6] = { 4, 1, 5, 0, 3, 2 };
    for (int k = 0; k < 6; ++k)
        if (a[k].Cost != sorted[k] || a[k].BlockIndex != origin[k] || a[k].Benefit != (long)(100 + origin[k])) { std::printf("00adbe70_TEST FAIL %d\n", k); return 1; }
    if (g_unguarded != 3) { std::printf("00adbe70_TEST FAIL unguarded %d\n", g_unguarded); return 1; }
    std::__insertion_sort(a, a, std::less<CBlockInfo>());
    std::printf("00adbe70_TEST PASS\n");
    return 0;
}
