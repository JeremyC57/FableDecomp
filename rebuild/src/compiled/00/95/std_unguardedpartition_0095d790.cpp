// std::__unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp) -- (CThing* elements, CThingCompare_Nearest) STLport Hoare partition: advance first past elements that compare before the pivot, retreat last past those the pivot compares before, swap the pair, until the cursors cross; return first
// Leaf: ??$__unguarded_partition@PAPAVCThing@@PAV1@VCThingCompare_Nearest@@@std@@YIPAPAVCThing@@PAPAV1@0PAV1@VCThingCompare_Nearest@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095D790, 126 bytes, `__fastcall` (first in ecx, last in edx; the pivot and the
//   12-byte comparator by value on the stack), `ret 10h`, returns first in eax; /O2 TU (16-aligned loop heads).
// Identity (proven): std::__unguarded_partition<CThing**, CThing*, CThingCompare_Nearest>; evidence: its comparator callee 0x004A8D80 is CThingCompare_Nearest::operator()<CThing> (bindings and FableWin.pdb; it returns the nearer thing, which the partition tests as a bool), and its only caller is the introsort loop 0x0095DF40 <- sort 0x0095E010 <- 0x0095D550 and CAIStateGroup_RangedCombatUsePreferredFirePoint::IsRunnable 0x0095D590 -- the class whose FableWin.pdb ProcessFirePointList / GetFirePointList take vector<CThing*>&; 0x0095D550 sorts that vector by distance from the brain owner's position (CThing::GetPos 0x004C73D0).
// Label: the manifest's "std_sort_Partition" is propagated / behavioural (family J082: five partitions that differ in the comparator
//   callee, the comparator size and the result test; retail has no identical-code folding).
// Callees: CThingCompare_Nearest::operator()<CThing>(CThing*, CThing*) const 0x004A8D80 (bindings / FableWin.pdb ??$?RVCThing@@@CThingCompare_Nearest@@QBEPAVCThing@@PAV1@0@Z), four sites (thiscall on the by-value comparator, `ret 8`).
// Body: STLport 4.6 _algo.c `for (;;) { while (__comp(*__first, __pivot)) ++__first; --__last; while (__comp(__pivot, *__last))
//   --__last; if (!(__first < __last)) return __first; iter_swap(__first, __last); ++__first; }` (the J052 model) with the inline
//   STLport iter_swap (__iter_swap through the value type: a pointer swap); the comparator returns the nearer thing (a pointer), so each test is `test eax,eax`.
class C3DVector {                                        // Ego_r.pdb 12 bytes
public:
    float X;
    float Y;
    float Z;
};
class CThing;
class CThingCompare_Nearest {                            // Ego_r.pdb 12 bytes
public:
    template <class T> T* operator()(T* a, T* b) const;  // 0x004A8D80 for CThing (returns the nearer thing)
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
typedef CThing* TElement_0095d790;
template TElement_0095d790* __fastcall __unguarded_partition<TElement_0095d790*, TElement_0095d790, CThingCompare_Nearest >(TElement_0095d790*, TElement_0095d790*, TElement_0095d790, CThingCompare_Nearest);
}   // namespace std