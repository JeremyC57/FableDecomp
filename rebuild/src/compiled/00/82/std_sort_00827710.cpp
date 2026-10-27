// std::sort(std::pair<long, CCountedPointer<CContainmentVolume> >* first, std::pair<long, CCountedPointer<CContainmentVolume> >* last, CKeyPairCompareLess<long, CCountedPointer<CContainmentVolume> > comp) -- STLport sort: when the range is not empty, introsort it with depth limit 2 * floor(log2(n)), then finish with the final insertion sort
// Leaf: ??$sort@PAU?$pair@JV?$CCountedPointer@VCContainmentVolume@@@@@std@@V?$CKeyPairCompareLess@JV?$CCountedPointer@VCContainmentVolume@@@@@@@std@@YIXPAU?$pair@JV?$CCountedPointer@VCContainmentVolume@@@@@0@0V?$CKeyPairCompareLess@JV?$CCountedPointer@VCContainmentVolume@@@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00827710, 82 bytes, /Gr __fastcall (first in ecx, last in edx, comp on the stack), `ret 4`.
// Identity (proven): its __introsort_loop 0x008274B0 is bound ??$__introsort_loop@PAU?$pair@JV?$CCountedPointer@VCContainmentVolume@@@@@std@@U12@HV?$CKeyPairCompareLess@JV?$CCountedPointer@VCContainmentVolume@@@@@@@std@@...; 12-byte stride; caller the CVectorMap Sort 0x00827830; bound only as the stand-in ?GuiTarget_Set@@YIXPAUGuiTarget@@H_N@Z.
// Callees: __introsort_loop<T*, T, int, Compare>(first, last, (T*)0, depth, comp) 0x008274B0 and
//   __final_insertion_sort<T*, Compare>(first, last, comp) 0x00827350 (both __fastcall, out of line).
// Shape (family L050): n by the 12-byte magic divide, the inline __lg loop (`sar eax,1 ; inc ecx` until 1), depth
//   `lea eax,[ecx+ecx]`; the 4-byte comparator copy re-pushed for both calls.
class CCPPointerInfo;
template <class T> class CCountedPointer {                 // 8 bytes: Data, Info
public:
    T* Data;                                              // +0x00
    CCPPointerInfo* Info;                                 // +0x04
};
class CContainmentVolume;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess {    // empty, passed by value
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};
namespace std {
template <class _Size> inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1)
        ++__k;
    return __k;
}
template <class _Tp> inline _Tp* __value_type(_Tp*) { return (_Tp*)0; }
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp);                                          // 0x008274B0
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00827350
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, __value_type(__first), __lg(__last - __first) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}
}
template void __fastcall std::sort(std::pair<long, CCountedPointer<CContainmentVolume> >*, std::pair<long, CCountedPointer<CContainmentVolume> >*, CKeyPairCompareLess<long, CCountedPointer<CContainmentVolume> >);