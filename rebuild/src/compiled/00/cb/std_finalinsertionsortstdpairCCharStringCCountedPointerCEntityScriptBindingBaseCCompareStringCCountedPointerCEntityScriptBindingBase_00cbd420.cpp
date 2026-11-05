// std::__final_insertion_sort(std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >* first, std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >* last, CCompareString<CCountedPointer<CEntityScriptBindingBase> > comp) -- insertion sort of the first 16, unguarded insertion sort of the rest
// Leaf: ??$__final_insertion_sort@PAU?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@std@@V?$CCompareString@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@@@std@@YIXPAU?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@0@0V?$CCompareString@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBD420, oracle length 80 bytes.
// Identity (naming wave 20, chunk 686): STLport 4.6 __final_insertion_sort (stl/_algo.c) of the sort<> 0x00CBDF70 over the 12-byte
//   pair<CCharString, CCountedPointer<CEntityScriptBindingBase> > with CCompareString; callees the landed __insertion_sort 0x00CBCF70 (the
//   same instantiation, its own leaf) and __unguarded_insertion_sort_aux 0x00CBB2D0. Was the descriptive FinalInsertionSort_SElement12_00cbd420
//   over SElement12 / SCompare stand-ins (one of 25 bodies of this shape).
// Retail bytes:  push esi ; mov esi,edx ; sub edx,ecx ; (divide by 12) ; cmp eax,10h ; jle S ; push ebx ; mov ebx,[comp] ; push edi ; lea edi,[ecx+0xc0] ; push ebx ; mov edx,edi ; call __insertion_sort ; push ebx ; push 0 ; mov edx,esi ; mov ecx,edi ; call __unguarded_insertion_sort_aux ; pop edi ; pop ebx ; pop esi ; ret 4 ; S: push [comp] ; mov edx,esi ; call __insertion_sort ; pop esi ; ret 4
// /Gr TU (__fastcall: first in ecx, last in edx, comp on the stack).
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
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);                                 // 0x00CBCF70
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp);          // 0x00CBB2D0
template <class _RandomAccessIter, class _Compare>
inline void __fastcall __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __unguarded_insertion_sort_aux(__first, __last, (typename iterator_traits<_RandomAccessIter>::value_type*)0, __comp);   // _STLP_VALUE_TYPE
}
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first > __stl_threshold) {
        __insertion_sort(__first, __first + __stl_threshold, __comp);
        __unguarded_insertion_sort(__first + __stl_threshold, __last, __comp);
    }
    else
        __insertion_sort(__first, __last, __comp);
}
template void __fastcall __final_insertion_sort<CScriptBindingPair*, CScriptBindingLess>(CScriptBindingPair*, CScriptBindingPair*, CScriptBindingLess);
}
