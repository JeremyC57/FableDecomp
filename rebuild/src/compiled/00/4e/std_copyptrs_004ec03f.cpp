// std::__copy_ptrs(const CCardDef* first, const CCardDef* last, CCardDef* result, const __false_type&) -- STLport assignment copy of def records: n = last - first, then `*result = *first` n times; returns the end of the copy
// Flags: /Gr
// Leaf: ??$__copy_ptrs@VCCardDef@@@std@@YI
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EC03F, 75 bytes, __fastcall (first ecx, last edx, result and the empty tag on the stack), `ret 8`,
//   frame pointer.
// Identity (proven): reloc_bindings ??$__copy_ptrs@VCCardDef@@@std@@YIPAVCCardDef@@PBV1@0PAV1@ABU__false_type@0@@Z (the landed operator= rows' one-parameter spelling); callers the landed vector<CCardDef>::operator= 0x004EBF32.
// Callees: CParentDefClassBase::operator= 0x00431ED0 per element (the implicit CCardDef::operator= inline: the base part, then the two own dwords).
// Shape (family K124, representative 0x004EABE5): STLport 4.6 `__copy_ptrs(..., const __false_type&)` -> `__copy(first, last, result,
//   random_access_iterator_tag(), (int*)0)` (__forceinline); frame/size TU (`push 30h ; pop esi ; idiv esi`, the count in the
//   result slot).
#pragma optimize("y", off)
#pragma optimize("s", on)
#pragma pack(push, 1)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#pragma pack(pop)
class CCardDef : public CParentDefClassBase {            // retail 0x44 bytes (Ego_r.pdb - 4, RTTI base CParentDefClassBase)
public:
    long CardName;                                       // +0x3C
    long CardVal;                                        // +0x40
    // the implicit memberwise assignment, written out: size mode would otherwise emit it out of line
    __forceinline CCardDef& operator=(const CCardDef& other)
    {
        CParentDefClassBase::operator=(other);
        CardName = other.CardName;
        CardVal = other.CardVal;
        return *this;
    }
};
namespace std {
struct __false_type { };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                 const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _Tp>
inline _Tp* __fastcall __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template CCardDef* __fastcall __copy_ptrs<CCardDef>(const CCardDef*, const CCardDef*, CCardDef*, const __false_type&);
}