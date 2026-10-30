// std::__copy_backward(CSurfaceEntry* first, CSurfaceEntry* last, CSurfaceEntry* result, const random_access_iterator_tag&, int*) -- STLport random-access copy_backward over the (long, CCompositeSurfaceInfo) vector-map entries: assign [first, last) backwards onto the range ending at result, return the moved-back result
// Leaf: ??$__copy_backward@PAU?$pair@JVCCompositeSurfaceInfo@@@std@@PAU12@H@std@@YIPAU?$pair@JVCCompositeSurfaceInfo@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BED9A0, length 133 bytes (`__fastcall`: first in ecx, last in edx,
//   result + the two tag arguments on the stack, `ret 0Ch`).
// Identity: renamed (sweep1, 2026-10-25) from _global::CopyBackward_PairLongPointerHolder_00bed9a0 (descriptive; manifest
//   _Move_backward<pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> >*> was propagated). Callers are the
//   landed STLport sort/heap helpers of CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<...> > (Ego_r.pdb name
//   set, which also holds the Dinkumware twin _Copy_backward<pair<long,CCompositeSurfaceInfo>*>): std::__linear_insert
//   0x00BEE8F0 (declares this callee as std::__copy_backward<..., ptrdiff_t>) and three out-of-line
//   copy_backward(first, last, result) wrappers 0x00BEDEA0 / 0x00BEE1B0 / 0x00BEE3C0 (`push 0 ; push &tag ; push result ;
//   call`, no direct xrefs of their own) after the landed 0x00BEDD70 / 0x00BEE140 / 0x00BEE320 (FINDINGS chunk 432; agent432
//   NOTES).
// Element: std::pair<long, CCompositeSurfaceInfo> (16 bytes, `sar 4`): the key, then Ego_r.pdb CCompositeSurfaceInfo
//   {vptr 0x012A8154 (CBaseClass), CCountedPointer<CCompositeSurfaceCache> Cache}; the implicit operator= stores the key
//   and assigns Cache but never touches the vptr at +4.
// Callees: operator delete @ 0x00BFE9BC (the count block's delete function is called through its slot +4).
// Shape: STLport 4.6 `for (_Distance n = last - first; n > 0; --n) *--result = *--last; return result;`; the signed count
//   is computed once and counted down in the dead `result` argument slot (`dec dword ptr [esp+18h]`); the inlined
//   CCountedPointer::operator= reads the source's info and data first, keeps them in ebx and the `push ecx` frame word
//   across the release. `__fastcall` on the template definition.
#include <new>
#include <stddef.h>

// CBaseClass: the polymorphic engine root (out-of-line virtual destructor 0x0099A2E0, vtable 0x01231710).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CCPPointerInfo {                              // the shared count block of a CCountedPointer
public:
    long  RefCount;                                 // +0x00
    void (__fastcall* DeleteFunction)(void* data);  // +0x04
    void* Data;                                     // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                           // +0x00
    CCPPointerInfo* Info;                           // +0x04
    void DeleteData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunction(Info->Data);
                operator delete(Info);
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            DeleteData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};

class CCompositeSurfaceCache;
// Ego_r.pdb CCompositeSurfaceInfo (vtable 0x012A8154, 12 bytes): implicit copy, assignment and (virtual) destructor.
class CCompositeSurfaceInfo : public CBaseClass {
public:
    CCountedPointer<CCompositeSurfaceCache> Cache;  // +0x04
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class T1, class T2> struct pair {
    T1 first;                                       // +0x00
    T2 second;                                      // +0x04
};

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&,
                                              _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
}   // namespace std

typedef std::pair<long, CCompositeSurfaceInfo> CSurfaceEntry;   // 0x10 bytes

template CSurfaceEntry* __fastcall std::__copy_backward<CSurfaceEntry*, CSurfaceEntry*, ptrdiff_t>(
    CSurfaceEntry*, CSurfaceEntry*, CSurfaceEntry*, const std::random_access_iterator_tag&, ptrdiff_t*);
