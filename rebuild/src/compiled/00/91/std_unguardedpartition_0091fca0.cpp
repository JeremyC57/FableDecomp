// std::__unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp) -- (const CThingObject* elements, CThingCompare_Nearest) STLport Hoare partition: advance first past elements that compare before the pivot, retreat last past those the pivot compares before, swap the pair, until the cursors cross; return first
// Leaf: ??$__unguarded_partition@PAPBVCThingObject@@PBV1@VCThingCompare_Nearest@@@std@@YIPAPBVCThingObject@@PAPBV1@0PBV1@VCThingCompare_Nearest@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091FCA0, 126 bytes, `__fastcall` (first in ecx, last in edx; the pivot and the
//   12-byte comparator by value on the stack), `ret 10h`, returns first in eax; /O2 TU (16-aligned loop heads).
// Identity (probable): std::__unguarded_partition<const CThingObject**, const CThingObject*, CThingCompare_Nearest>; evidence: PROBABLE element -- its comparator 0x0091F740 returns the nearer of two things (a pointer the partition tests as a bool, `test eax,eax`), i.e. CThingCompare_Nearest::operator()<T>; it is a separate COMDAT from the <CThing> instance 0x004A8D80 (row 0x0095D790), so T is not CThing, and FableWin.pdb instantiates CThingCompare_Nearest sorts over exactly two elements: CThing* and const CThingObject*. The only user, CAIStateGroup_TestTestTest::Update 0x0091F140 (vtable slot 12, RTTI .?AVCAIStateGroup_TestTestTest@@), sorts by distance from the brain owner's position the pointer vector filled by the rough-area search 0x00920200 with CThingFilter_None -- FableWin.pdb's only matching search is GetAllThingsInRoughArea<const CThingObject, vector<const CThingObject*>, CThingFilter_None>. The type rests on that elimination, not on RTTI; the caller chain is introsort loop 0x009205D0 <- sort 0x009206A0 <- 0x0091F2EF.
// Label: the manifest's "Sort_Partition" is propagated / behavioural (family J082: five partitions that differ in the comparator
//   callee, the comparator size and the result test; retail has no identical-code folding).
// Callees: CThingCompare_Nearest::operator()<const CThingObject>(const CThingObject*, const CThingObject*) const 0x0091F740 (FableWin.pdb ??$?R$$CBVCThingObject@@@CThingCompare_Nearest@@QBEPBVCThingObject@@PBV1@0@Z), four sites (thiscall on the by-value comparator, `ret 8`).
// Body: STLport 4.6 _algo.c `for (;;) { while (__comp(*__first, __pivot)) ++__first; --__last; while (__comp(__pivot, *__last))
//   --__last; if (!(__first < __last)) return __first; iter_swap(__first, __last); ++__first; }` (the J052 model) with the inline
//   STLport iter_swap (__iter_swap through the value type: a pointer swap); the comparator returns the nearer thing (a pointer), so each test is `test eax,eax`.
class C3DVector {                                        // Ego_r.pdb 12 bytes
public:
    float X;
    float Y;
    float Z;
};
class CThingObject;
class CThingCompare_Nearest {                            // Ego_r.pdb 12 bytes
public:
    template <class T> T* operator()(T* a, T* b) const;  // 0x0091F740 for const CThingObject (returns the nearer thing)
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
typedef const CThingObject* TElement_0091fca0;
template TElement_0091fca0* __fastcall __unguarded_partition<TElement_0091fca0*, TElement_0091fca0, CThingCompare_Nearest >(TElement_0091fca0*, TElement_0091fca0*, TElement_0091fca0, CThingCompare_Nearest);
}   // namespace std