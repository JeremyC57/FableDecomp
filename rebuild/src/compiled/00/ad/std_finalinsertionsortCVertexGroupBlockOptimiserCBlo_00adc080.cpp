// std::__final_insertion_sort<CVertexGroupBlockOptimiser::CBlockInfo*, std::less<CBlockInfo> > -- STLport 4.x:
// ranges longer than __stl_threshold (16) get __insertion_sort(first, first + 16) @ 0x00ADBE70 followed by
// __unguarded_insertion_sort(first + 16, last) -> __unguarded_insertion_sort_aux(.., (CBlockInfo*)0, comp)
// @ 0x00ADAF90; shorter ones a single __insertion_sort.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADC080, 80 bytes, `ret 4`: /Gr algorithm TU (first ecx, last edx, the empty
// 4-byte less<> on the stack), spelled here as explicit __fastcall.
// Identity: Ego_r.pdb CVertexGroupBlockOptimiser::CBlockInfo (12 bytes: BlockIndex +0, Cost +4, Benefit +8;
// `sub edx,ecx` then the divide-by-12); both callees are CBlockInfo instances (0x00ADBE70 keys the signed Cost,
// 0x00ADAF90 grades as __unguarded_insertion_sort_aux<CBlockInfo*>); called from sort<CBlockInfo*> 0x00ADC890.
// Leaf: ??$__final_insertion_sort@PAVCBlockInfo@CVertexGroupBlockOptimiser@@U?$less@VCBlockInfo@CVertexGroupBlockOptimiser@@@std@@@std@@YIXPAVCBlockInfo@CVertexGroupBlockOptimiser@@0U?$less@VCBlockInfo@CVertexGroupBlockOptimiser@@@0@@Z
namespace std {

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00ADBE70

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*,
                                               _Compare __comp);   // 0x00ADAF90

template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    __unguarded_insertion_sort_aux(__first, __last, (_Tp*)0, __comp);
}

template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first > 16) {
        __insertion_sort(__first, __first + 16, __comp);
        __unguarded_insertion_sort(__first + 16, __last, __first, __comp);
    }
    else
        __insertion_sort(__first, __last, __comp);
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
template void __fastcall __final_insertion_sort<CVertexGroupBlockOptimiser::CBlockInfo*, less<CVertexGroupBlockOptimiser::CBlockInfo> >(
    CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo*, less<CVertexGroupBlockOptimiser::CBlockInfo>);
}
