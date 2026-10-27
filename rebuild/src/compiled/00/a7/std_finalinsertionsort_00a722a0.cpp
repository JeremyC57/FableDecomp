// std::__final_insertion_sort(std::pair<CCharString, C3DDummyObject>* first, std::pair<CCharString, C3DDummyObject>* last, CCompareString<C3DDummyObject> comp) -- finish an introsort: insertion-sort the first 16 elements, then the unguarded tail
// Leaf: ??$__final_insertion_sort@PAU?$pair@VCCharString@@VC3DDummyObject@@@std@@V?$CCompareString@VC3DDummyObject@@@@@std@@YIXPAU?$pair@VCCharString@@VC3DDummyObject@@@0@0V?$CCompareString@VC3DDummyObject@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A722A0, 86 bytes, `__fastcall` (first ecx, last edx, the comparator's slot on the stack), `ret 4`.
// Identity (proven): its callees are the landed __insertion_sort 0x00A71810 and __unguarded_insertion_sort_aux<pair<CCharString, C3DDummyObject>*, ..., CCompareString<C3DDummyObject> > 0x00A6F910 (0x38-byte records); its caller is that sort's driver 0x00A73600.
// Callees: std::__insertion_sort 0x00A71810, std::__unguarded_insertion_sort_aux 0x00A6F910 (both __fastcall).
// Shape (family M093, representative 0x004D1AE0): STLport 4.6 _algo.c `if (__last - __first > __stl_threshold) {
//   __insertion_sort(__first, __first + __stl_threshold, __comp); __unguarded_insertion_sort(__first + __stl_threshold, __last,
//   __comp); } else __insertion_sort(__first, __last, __comp);` with __unguarded_insertion_sort inline (its null _Tp* tag).
class CCharString {                                       // 4 bytes (PStringData)
public:
    void* PStringData;
};
class C3DDummyObject {                                    // 52 bytes
public:
    unsigned long Members[13];
};
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
template <class T> class CCompareString {
public:
    bool operator()(const std::pair<CCharString, T>& a, const std::pair<CCharString, T>& b) const;
};
namespace std {
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00A71810
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp);   // 0x00A6F910
template <class _RandomAccessIter, class _Compare>
inline void __fastcall __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __unguarded_insertion_sort_aux(__first, __last, (std::pair<CCharString, C3DDummyObject>*)0, __comp);
}
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first > 16) {
        __insertion_sort(__first, __first + 16, __comp);
        __unguarded_insertion_sort(__first + 16, __last, __comp);
    }
    else
        __insertion_sort(__first, __last, __comp);
}
template void __fastcall __final_insertion_sort<std::pair<CCharString, C3DDummyObject>*, CCompareString<C3DDummyObject> >(std::pair<CCharString, C3DDummyObject>*, std::pair<CCharString, C3DDummyObject>*, CCompareString<C3DDummyObject>);
}