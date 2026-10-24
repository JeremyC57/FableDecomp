// _STL::__linear_insert(CVertexGroupBlockOptimiser::CBlockInfo* first, CBlockInfo* last, CBlockInfo val, less<CBlockInfo> comp) -- STLport insertion-sort step over block candidates: a value cheaper than the first shifts the run up and goes first, otherwise it sinks unguarded
// Flags: /Gr
// Leaf: ??$__linear_insert@PAVCBlockInfo@CVertexGroupBlockOptimiser
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADBC50, 0x5C bytes (+ int3 padding), __fastcall (first ecx, last edx),
// `ret 10h` (the 12-byte value and the empty 4-byte comparator).
// Identity: Ego_r.pdb CVertexGroupBlockOptimiser::CBlockInfo (12 bytes: BlockIndex +0, Cost +4, Benefit +8) under
// CBlockInfo::operator< on Cost (`cmp edi,eax ; jge`); the out-of-line copy of the __linear_insert that the landed
// __insertion_sort 0x00ADBE70 inlines. Catalog "Vector_Pair_InsertOrShift" is descriptive.
// Callees: __copy_backward(first, last, result, const random_access_iterator_tag&, ptrdiff_t*) @ 0x00ADAB20 (landed
// global_CopyBackwardSElement1200adab20), __unguarded_linear_insert(last, val, comp) @ 0x00ADAA30.
// Shape: STLport 4.x `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
// __unguarded_linear_insert(last, val, comp);` -- copy_backward's empty iterator tag is homed over the comparator's
// argument slot (`lea eax,[esp+1Ch]`), which needs the real __copy_backward body out of line in the TU.
namespace _STL {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

#pragma auto_inline(off)
// Same-TU stand-in for the out-of-line __copy_backward (retail 0x00ADAB20): it never touches the tag, which lets
// VC7.1 home the caller's empty tag on a dead argument slot as retail does.
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
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
void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00ADAA30

template <class _RandomAccessIter, class _Tp, class _Compare>
void __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

} // namespace _STL

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

namespace _STL {
template void __linear_insert<CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo,
                              less<CVertexGroupBlockOptimiser::CBlockInfo> >(
    CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo*, CVertexGroupBlockOptimiser::CBlockInfo,
    less<CVertexGroupBlockOptimiser::CBlockInfo>);
}