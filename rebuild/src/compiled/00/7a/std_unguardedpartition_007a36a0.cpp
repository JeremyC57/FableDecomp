// std::__unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp) -- (CThing* elements, CThingCompare_NearestToPos) STLport Hoare partition: advance first past elements that compare before the pivot, retreat last past those the pivot compares before, swap the pair, until the cursors cross; return first
// Leaf: ??$__unguarded_partition@PAPAVCThing@@PAV1@VCThingCompare_NearestToPos@@@std@@YIPAPAVCThing@@PAPAV1@0PAV1@VCThingCompare_NearestToPos@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007A36A0, 126 bytes, `__fastcall` (first in ecx, last in edx; the pivot and the
//   12-byte comparator by value on the stack), `ret 10h`, returns first in eax; /O2 TU (16-aligned loop heads).
// Identity (proven): std::__unguarded_partition<CThing**, CThing*, CThingCompare_NearestToPos>; evidence: its comparator callee 0x007A2F90 is CThingCompare_NearestToPos::operator()<CThing> (FableWin.pdb and the bindings), the 12-byte comparator (Pos) is passed by value (`ret 10h`), and its only caller is the introsort loop 0x007A4140, whose binding is ??$__introsort_loop@PAPAVCThing@@PAV1@HVCThingCompare_NearestToPos@@@std@@ (sort 0x007A4260, used by CTCWillLightning::GetTargets and CGameScriptInterface::GetAllThingsWithDefNameByDistanceFrom); FableWin.pdb has _Unguarded_partition<CThing**, CThingCompare_NearestToPos>.
// Label: the manifest's "Partition_CThingAICreature" is propagated / behavioural (family J082: five partitions that differ in the comparator
//   callee, the comparator size and the result test; retail has no identical-code folding).
// Callees: CThingCompare_NearestToPos::operator()<CThing>(CThing*, CThing*) const 0x007A2F90 (bindings / FableWin.pdb ??$?RVCThing@@@CThingCompare_NearestToPos@@QBE_NPAVCThing@@0@Z), four sites (thiscall on the by-value comparator, `ret 8`).
// Body: STLport 4.6 _algo.c `for (;;) { while (__comp(*__first, __pivot)) ++__first; --__last; while (__comp(__pivot, *__last))
//   --__last; if (!(__first < __last)) return __first; iter_swap(__first, __last); ++__first; }` (the J052 model) with the inline
//   STLport iter_swap (__iter_swap through the value type: a pointer swap).
class C3DVector {                                        // Ego_r.pdb 12 bytes
public:
    float X;
    float Y;
    float Z;
};
class CThing;
class CThingCompare_NearestToPos {                       // Ego_r.pdb 12 bytes
public:
    template <class T> bool operator()(T* a, T* b) const;   // 0x007A2F90 for CThing
    C3DVector Pos;                                       // +0x0
};
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
typedef CThing* TElement_007a36a0;
template TElement_007a36a0* __fastcall __unguarded_partition<TElement_007a36a0*, TElement_007a36a0, CThingCompare_NearestToPos >(TElement_007a36a0*, TElement_007a36a0*, TElement_007a36a0, CThingCompare_NearestToPos);
}   // namespace std