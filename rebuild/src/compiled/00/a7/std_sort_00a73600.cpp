// std::sort(std::pair<CCharString, C3DDummyObject>* first, std::pair<CCharString, C3DDummyObject>* last, CCompareString<C3DDummyObject> comp) -- introsort the range, then finish with the final insertion sort
// Leaf: ??$sort@PAU?$pair@VCCharString@@VC3DDummyObject@@@std@@V?$CCompareString@VC3DDummyObject@@@@@std@@YIXPAU?$pair@VCCharString@@VC3DDummyObject@@@0@0V?$CCompareString@VC3DDummyObject@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A73600, 85 bytes, `__fastcall` (first ecx, last edx, the comparator's slot on the stack), `ret 4`.
// Identity (proven): its __final_insertion_sort is 0x00A722A0 (family M093: pair<CCharString, C3DDummyObject>, CCompareString<C3DDummyObject>); its callers are CPhysicsMesh::GetHelperDataFromFileData 0x00A6D0E0 and the vector map's Sort 0x00A73790.
// Callees: std::__introsort_loop 0x00A730E0, std::__final_insertion_sort 0x00A722A0 (both __fastcall).
// Shape (family M094, representative 0x004D2570): STLport 4.6 _algo.c `if (__first != __last) { __introsort_loop(__first, __last,
//   _STLP_VALUE_TYPE(__first), __lg(__last - __first) * 2, __comp); __final_insertion_sort(__first, __last, __comp); }` -- __lg
//   inline (`sar 1 ; inc` until 1).
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
template <class _Size> inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1) ++__k;
    return __k;
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp);   // 0x00A730E0
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00A722A0
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, (std::pair<CCharString, C3DDummyObject>*)0, __lg(__last - __first) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}
template void __fastcall sort<std::pair<CCharString, C3DDummyObject>*, CCompareString<C3DDummyObject> >(std::pair<CCharString, C3DDummyObject>*, std::pair<CCharString, C3DDummyObject>*, CCompareString<C3DDummyObject>);
}