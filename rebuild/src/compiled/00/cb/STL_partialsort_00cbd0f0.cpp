// std::__partial_sort(CBindingPair* first, CBindingPair* middle, CBindingPair* last, CBindingPair*, CBindingLess comp) -- partial sort of (name, entity script binding) pairs by name: heapify [first, middle), pop every smaller later pair into the heap, then sort the heap
// Leaf: ??$__partial_sort@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBD0F0, 120 bytes (`ret 0Ch`).
// TU: the scripting library's STLport 4.x instantiations (0x00CB6xxx-0x00CBDxxx); the game is built /Gr, so the
//   algorithm templates are __fastcall (first two pointer arguments in ecx/edx, callee pops the rest).
// Identity: STLport __partial_sort over (name, entity script binding) = std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> > (0x0C bytes, CCompareString -> CCharString::operator< 0x009E6530); the 6 callers
//   0x00CBD49C, 0x00CBDDE4..0x00CBDEB3 are the binding-map sort. The manifest label "CGameCameraManager_InitModes" is
//   propagated and wrong.
// Callees: __make_heap 0x00CBBC00, __pop_heap 0x00CBBAD0, sort_heap 0x00CBC580, CCharString::operator< 0x009E6530,
//   CCharString copy constructor 0x0099EC30 (the by-value `_Tp(*__i)`).
// Shape: as 0x00CB7140 for the 12-byte element.
void __cdecl operator delete(void* block);                 // 0x00BFE9BC
class CCharString {
public:
    void* m_pStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0
    bool operator<(const CCharString& other) const;      // 0x009E6530 ??MCCharString@@QBE_NABV0@@Z (FableWin.pdb; was the descriptive FastCompare -- naming wave 20)
};
#include "engine/CCountedPointer.h"
class CEntityScriptBindingBase;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class V> class CCompareString {                  // empty, passed by value (4-byte slot)
public:
    bool operator()(const std::pair<CCharString, V>& a, const std::pair<CCharString, V>& b) const { return a.first.operator<(b.first); }
};
typedef std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> > CBindingPair;   // 12 bytes
typedef CCompareString<CCountedPointer<CEntityScriptBindingBase> > CBindingLess;
namespace std {
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*);   // 0x00CBBC00
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*);   // 0x00CBBAD0
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x00CBC580

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, (CBindingPair*)0, (int*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, (int*)0);
    sort_heap(__first, __middle, __comp);
}
template void __fastcall __partial_sort<CBindingPair*, CBindingPair, CBindingLess>(CBindingPair*, CBindingPair*, CBindingPair*, CBindingPair*, CBindingLess);
}