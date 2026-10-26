// std::__unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp) -- (CThingBuilding* elements, CIsHouseLargerThan) STLport Hoare partition: advance first past elements that compare before the pivot, retreat last past those the pivot compares before, swap the pair, until the cursors cross; return first
// Leaf: ??$__unguarded_partition@PAPAVCThingBuilding@@PAV1@VCIsHouseLargerThan@@@std@@YIPAPAVCThingBuilding@@PAPAV1@0PAV1@VCIsHouseLargerThan@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00513E20, 126 bytes, `__fastcall` (first in ecx, last in edx; the pivot and the
//   empty comparator by value on the stack), `ret 8`, returns first in eax; /O2 TU (16-aligned loop heads).
// Identity (proven): std::__unguarded_partition<CThingBuilding**, CThingBuilding*, CIsHouseLargerThan>; evidence: its comparator callee 0x00512410 is CIsHouseLargerThan::operator() (the bindings' ??RCIsHouseLargerThan@@QBE_N..VCThingBuilding@@0@Z; FableWin.pdb spells it with const CThingBuilding*), the empty comparator is passed by value (`ret 8`), and its only caller is the introsort loop 0x0051BAF0 (called by 0x0051C400, the sort of the house-size ordering in 0x005048F0); FableWin.pdb instantiates sort / _Insertion_sort / _Unguarded_partition over vector<CThingBuilding*> with CIsHouseLargerThan and no other element for it.
// Label: the manifest's "IntrosortShellsort_Partition_Hoare" is propagated / behavioural (family J082: five partitions that differ in the comparator
//   callee, the comparator size and the result test; retail has no identical-code folding).
// Callees: CIsHouseLargerThan::operator()(const CThingBuilding*, const CThingBuilding*) const 0x00512410 (FableWin.pdb ??RCIsHouseLargerThan@@QBE_NPBVCThingBuilding@@0@Z), four sites (thiscall on the by-value comparator, `ret 8`).
// Body: STLport 4.6 _algo.c `for (;;) { while (__comp(*__first, __pivot)) ++__first; --__last; while (__comp(__pivot, *__last))
//   --__last; if (!(__first < __last)) return __first; iter_swap(__first, __last); ++__first; }` (the J052 model) with the inline
//   STLport iter_swap (__iter_swap through the value type: a pointer swap).
class CThingBuilding;
class CIsHouseLargerThan {                               // Ego_r.pdb: empty
public:
    bool operator()(const CThingBuilding* a, const CThingBuilding* b) const;   // 0x00512410
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
typedef CThingBuilding* TElement_00513e20;
template TElement_00513e20* __fastcall __unguarded_partition<TElement_00513e20*, TElement_00513e20, CIsHouseLargerThan >(TElement_00513e20*, TElement_00513e20*, TElement_00513e20, CIsHouseLargerThan);
}   // namespace std