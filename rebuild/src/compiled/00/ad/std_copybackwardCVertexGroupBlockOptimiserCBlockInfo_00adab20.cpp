// std::__copy_backward<CVertexGroupBlockOptimiser::CBlockInfo*, CBlockInfo*, int> -- STLport 4.x random-access
// backward copy: `for (n = last - first; n > 0; --n) *--result = *--last;`, returns the moved-back result.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADAB20, 76 bytes, `ret 0Ch`: /Gr algorithm TU (first ecx, last edx; result,
// the const random_access_iterator_tag& and the int* distance tag on the stack), spelled here as explicit __fastcall.
// Identity: Ego_r.pdb CVertexGroupBlockOptimiser::CBlockInfo (12 bytes); called by copy_backward inside the CBlockInfo
// __insertion_sort 0x00ADBE70 (call at 0xADBEAA) and __linear_insert 0x00ADBC50 (0xADBC6C), both graded under the
// CBlockInfo/less<> model; the other three callers (0xADB0A0 / 0xADB4B0 / 0xADB820) are uncatalogued, unreferenced
// out-of-line copy_backward(first, last, result) wrappers (`ret 8`) inside the same CBlockInfo sort TU.
// Leaf: ??$__copy_backward@PAVCBlockInfo@CVertexGroupBlockOptimiser@@PAV12@H@std@@YIPAVCBlockInfo@CVertexGroupBlockOptimiser@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

} // namespace std

class CVertexGroupBlockOptimiser {
public:
    class CBlockInfo {                       // Ego_r.pdb, 12 bytes
    public:
        unsigned long BlockIndex;            // +0x0
        long          Cost;                  // +0x4
        long          Benefit;               // +0x8
        bool operator<(const CBlockInfo& other) const { return Cost < other.Cost; }
    };
};

namespace std {
template CVertexGroupBlockOptimiser::CBlockInfo* __fastcall
__copy_backward<CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo*, int>(
    CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo*,
    const random_access_iterator_tag&, int*);
}
