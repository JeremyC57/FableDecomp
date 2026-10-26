// std::__unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp) -- (const CThing* elements, CCompare_FurthestFromInitPos2D) STLport Hoare partition: advance first past elements that compare before the pivot, retreat last past those the pivot compares before, swap the pair, until the cursors cross; return first
// Leaf: ??$__unguarded_partition@PAPBVCThing@@PBV1@VCCompare_FurthestFromInitPos2D@?A0xf4de8d9e@@@std@@YIPAPBVCThing@@PAPBV1@0PBV1@VCCompare_FurthestFromInitPos2D@?A0xf4de8d9e@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00748B60, 126 bytes, `__fastcall` (first in ecx, last in edx; the pivot and the
//   empty comparator by value on the stack), `ret 8`, returns first in eax; /O2 TU (16-aligned loop heads).
// Identity (proven): std::__unguarded_partition<const CThing**, const CThing*, CCompare_FurthestFromInitPos2D>; evidence: its comparator callee 0x007483E0 is the anonymous-namespace CCompare_FurthestFromInitPos2D::operator() of the CTCStealth TU (agent952's landed sort_heap 0x00749390 of the same instantiation calls it through __adjust_heap 0x00748C90); its only caller is the introsort loop 0x007496B0, reached from the sort 0x00749740 that CTCStealth::UpdateDangerInfo 0x00747C70 runs; FableWin.pdb has sort / _Unguarded_partition over vector<const CThing*> with this comparator.
// Label: the manifest's "CCompare_FurthestFromInitPos2D::Partition" is propagated / behavioural (family J082: five partitions that differ in the comparator
//   callee, the comparator size and the result test; retail has no identical-code folding).
// Callees: CCompare_FurthestFromInitPos2D::operator()(const CThing*, const CThing*) const 0x007483E0 (anonymous namespace; FableWin.pdb ??RCCompare_FurthestFromInitPos2D@?A0xae0dd725@@QBE_NPBVCThing@@0@Z), four sites (thiscall on the by-value comparator, `ret 8`).
// Body: STLport 4.6 _algo.c `for (;;) { while (__comp(*__first, __pivot)) ++__first; --__last; while (__comp(__pivot, *__last))
//   --__last; if (!(__first < __last)) return __first; iter_swap(__first, __last); ++__first; }` (the J052 model) with the inline
//   STLport iter_swap (__iter_swap through the value type: a pointer swap).
class CThing;
namespace {
class CCompare_FurthestFromInitPos2D {                   // Ego_r.pdb: empty, anonymous namespace (?A0xae0dd725 there)
public:
    bool operator()(const CThing* a, const CThing* b) const;   // 0x007483E0
};
}
namespace std {
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}
template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __a, _ForwardIter2 __b)
{
    __iter_swap(__a, __b, __value_type(__a));
}
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}
typedef const CThing* TElement_00748b60;
template TElement_00748b60* __fastcall __unguarded_partition<TElement_00748b60*, TElement_00748b60, CCompare_FurthestFromInitPos2D >(TElement_00748b60*, TElement_00748b60*, TElement_00748b60, CCompare_FurthestFromInitPos2D);
}   // namespace std