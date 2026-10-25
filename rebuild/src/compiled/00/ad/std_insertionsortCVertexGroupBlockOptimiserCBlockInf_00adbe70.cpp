// std::__insertion_sort<CVertexGroupBlockOptimiser::CBlockInfo*, std::less<CBlockInfo> > -- STLport 4.x insertion
// sort of the block candidates by Cost (CBlockInfo::operator<), __linear_insert inlined: a value cheaper than the
// first shifts the run up one slot (out-of-line __copy_backward 0x00ADAB20) and goes first, anything else sinks
// through the out-of-line __unguarded_linear_insert 0x00ADAA30 (`ret 10h`).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADBE70, 135 bytes, `ret 4`: the SGI/STLport algorithm TU is /Gr (first ecx,
// last edx, the empty 4-byte less<> on the stack), spelled here as explicit __fastcall.
// Identity: Ego_r.pdb CVertexGroupBlockOptimiser::CBlockInfo (12 bytes: BlockIndex +0, Cost +4, Benefit +8; the
// PDB's Dinkumware build sorts CBlockInfo* with operator<, e.g. `??$_Insertion_sort1@PAVCBlockInfo@...`); signed
// `cmp ebx,[edi+4] ; jge` on Cost; callers are the __final_insertion_sort 0x00ADC080 (`call` at 0xADC0A9 / 0xADC0C7);
// the out-of-line twin of the inlined step is __linear_insert<CBlockInfo*> 0x00ADBC50.
// Shape: copy_backward's empty iterator tag is homed over the LIVE comparator slot only with the real
// __copy_backward body defined out of line in the TU (GOTCHAS "empty tag over a LIVE argument slot").
// Leaf: ??$__insertion_sort@PAVCBlockInfo@CVertexGroupBlockOptimiser@@U?$less@VCBlockInfo@CVertexGroupBlockOptimiser@@@std@@@std@@YIXPAVCBlockInfo@CVertexGroupBlockOptimiser@@0U?$less@VCBlockInfo@CVertexGroupBlockOptimiser@@@0@@Z
namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

#pragma auto_inline(off)
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
#pragma auto_inline(on)

template <class _BI1, class _BI2>
inline _BI2 copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00ADAA30

template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last)
        return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
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
template void __fastcall __insertion_sort<CVertexGroupBlockOptimiser::CBlockInfo*, less<CVertexGroupBlockOptimiser::CBlockInfo> >(
    CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo*, less<CVertexGroupBlockOptimiser::CBlockInfo>);
}
