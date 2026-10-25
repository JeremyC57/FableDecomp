// Link test: calls the source object's std::__final_insertion_sort<CBlockInfo*, less<CBlockInfo> > and supplies its
// two callees (recording their ranges).
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
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp);
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);
}
static int g_is = 0, g_ua = 0;
static CBlockInfo* g_isF; static CBlockInfo* g_isL; static CBlockInfo* g_uaF; static CBlockInfo* g_uaL; static void* g_uaT = (void*)1;
namespace std {
template <> void __fastcall __insertion_sort<CBlockInfo*, less<CBlockInfo> >(CBlockInfo* f, CBlockInfo* l, less<CBlockInfo>)
{ ++g_is; g_isF = f; g_isL = l; }
template <> void __fastcall __unguarded_insertion_sort_aux<CBlockInfo*, CBlockInfo, less<CBlockInfo> >(CBlockInfo* f, CBlockInfo* l, CBlockInfo* t, less<CBlockInfo>)
{ ++g_ua; g_uaF = f; g_uaL = l; g_uaT = t; }
}
int main()
{
    CBlockInfo a[20];
    std::__final_insertion_sort(a, a + 20, std::less<CBlockInfo>());
    if (g_is != 1 || g_ua != 1 || g_isF != a || g_isL != a + 16 || g_uaF != a + 16 || g_uaL != a + 20 || g_uaT != 0) { std::printf("00adc080_TEST FAIL long\n"); return 1; }
    std::__final_insertion_sort(a, a + 16, std::less<CBlockInfo>());
    if (g_is != 2 || g_ua != 1 || g_isF != a || g_isL != a + 16) { std::printf("00adc080_TEST FAIL short\n"); return 1; }
    std::printf("00adc080_TEST PASS\n");
    return 0;
}
