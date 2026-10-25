// Link test: calls the source object's std::__copy_backward<CBlockInfo*, CBlockInfo*, int>.
#include <cstdio>
class CVertexGroupBlockOptimiser {
public:
    class CBlockInfo {
    public:
        unsigned long BlockIndex;
        long          Cost;
        long          Benefit;
    };
};
typedef CVertexGroupBlockOptimiser::CBlockInfo CBlockInfo;
namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*);
}
int main()
{
    CBlockInfo a[6];
    for (int k = 0; k < 6; ++k) { a[k].BlockIndex = k; a[k].Cost = 10 * k; a[k].Benefit = 100 + k; }
    CBlockInfo* r = std::__copy_backward(a, a + 4, a + 5, std::random_access_iterator_tag(), (int*)0);
    if (r != a + 1) { std::printf("00adab20_TEST FAIL ret\n"); return 1; }
    for (int k = 1; k < 5; ++k)
        if (a[k].BlockIndex != (unsigned long)(k - 1) || a[k].Cost != 10 * (k - 1) || a[k].Benefit != 100 + k - 1) { std::printf("00adab20_TEST FAIL %d\n", k); return 1; }
    if (a[5].BlockIndex != 5 || a[0].BlockIndex != 0) { std::printf("00adab20_TEST FAIL edge\n"); return 1; }
    if (std::__copy_backward(a, a, a + 3, std::random_access_iterator_tag(), (int*)0) != a + 3) { std::printf("00adab20_TEST FAIL empty\n"); return 1; }
    std::printf("00adab20_TEST PASS\n");
    return 0;
}
