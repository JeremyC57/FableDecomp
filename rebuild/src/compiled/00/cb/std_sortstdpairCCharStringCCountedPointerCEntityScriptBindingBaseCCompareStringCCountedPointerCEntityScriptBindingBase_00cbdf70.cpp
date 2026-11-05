// std::sort(std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >* first, std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >* last, CCompareString<CCountedPointer<CEntityScriptBindingBase> > comp) -- introsort loop to depth 2*lg(n), then the final insertion sort
// Leaf: ??$sort@PAU?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@std@@V?$CCompareString@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@@@std@@YIXPAU?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@0@0V?$CCompareString@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBDF70, oracle length 82 bytes.
// Identity (naming wave 20, chunk 686): STLport 4.6 sort (stl/_algo.c) over the 12-byte pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >
//   with CCompareString -- the caller CVectorMap<CCharString, CCountedPointer<CEntityScriptBindingBase>, CCompareString<...> >::Sort 0x00CB8930
//   (landed) binds this spelling; the callees are the landed __introsort_loop 0x00CBDAD0 (chunk 686, the same instantiation) and
//   __final_insertion_sort 0x00CBD420. Was the descriptive Sort_SElement12_00cbdf70 over SElement12 / SCompare stand-ins (one of 15 bodies
//   of this shape); the comparator is the empty CCompareString passed by value in a 4-byte slot.
// Retail bytes:  push ebx ; push edi ; mov ebx,edx ; mov edi,ecx ; cmp edi,ebx ; je E ; (n = (last - first) / 12) ; xor ecx,ecx ; cmp eax,1 ; je K ; L: sar eax,1 ; inc ecx ; cmp eax,1 ; jne L ; K: push [comp] ; push 0 ; mov edx,ebx ; mov ecx,edi ; call __introsort_loop ; push [comp] ; mov edx,ebx ; mov ecx,edi ; call __final_insertion_sort ; E: pop edi ; pop ebx ; ret 4
// /Gr TU (__fastcall: first in ecx, last in edx, comp on the stack); `__lg` is the shift-count loop, its result int (the iterator difference).
void __cdecl operator delete(void* block);                 // 0x00BFE9BC
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    bool operator<(const CCharString& other) const;        // 0x009E6530 ??MCCharString@@QBE_NABV0@@Z (FableWin.pdb)
};
#include "engine/CCountedPointer.h"
class CEntityScriptBindingBase;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class T> class CCompareString {
public:
    bool operator()(const std::pair<CCharString, T>& a, const std::pair<CCharString, T>& b) const { return a.first < b.first; }
};
typedef std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> > CScriptBindingPair;
typedef CCompareString<CCountedPointer<CEntityScriptBindingBase> > CScriptBindingLess;
namespace std {
template <class _Iterator> struct iterator_traits;
template <class _Tp> struct iterator_traits<_Tp*> {
    typedef _Tp value_type;
};
const int __stl_threshold = 16;
template <class _Size> inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1)
        ++__k;
    return __k;
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp);   // 0x00CBDAD0
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);                          // 0x00CBD420
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, (typename iterator_traits<_RandomAccessIter>::value_type*)0, __lg(__last - __first) * 2, __comp);   // _STLP_VALUE_TYPE
        __final_insertion_sort(__first, __last, __comp);
    }
}
template void __fastcall sort<CScriptBindingPair*, CScriptBindingLess>(CScriptBindingPair*, CScriptBindingPair*, CScriptBindingLess);
}
