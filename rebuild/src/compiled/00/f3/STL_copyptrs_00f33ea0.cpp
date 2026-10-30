// std::__copy_ptrs(CFireHeartPatternDef* first, CFireHeartPatternDef* last, CFireHeartPatternDef* result, const __false_type&) -- STLport assignment copy of a CFireHeartPatternDef range (0x34-byte defs) to a lower address: n = last - first assignments front to back, returns the new end
// Leaf: ??$__copy_ptrs@PAVCFireHeartPatternDef@@PAV1@@std@@YIPAVCFireHeartPatternDef@@PAV1@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F33EA0, 74 bytes (+ int3 padding), __fastcall (first ecx, last edx), `ret 8`.
// Callees: the inlined implicit CFireHeartPatternDef::operator=: CDefClassBase::operator= @ 0x00431F10 (manifest
//   "CThingBuildingDef::operator=" is propagated) and vector<bool>::operator= @ 0x00462E78 (PadActive at +0x28; landed caller
//   CPlayerInventoryDef::operator= 0x00462E2D names it; manifest "CSimpleMemoryPool_AssignFrom" is propagated).
// Identity: its only caller is vector<CFireHeartPatternDef>::erase(first, last) 0x00F33E50 (`copy(last, _M_finish, first)`,
//   the zero-stored __false_type tag at [esp+13h]), then the virtual destructor loop; element 0x34 = the landed
//   CPersistContext_TransferableVectorOfSubComponents<CFireHeartPatternDef> (0x00F33D70). Catalog
//   "vector_shift_down_CThingBuildingDef_v2" is descriptive.
// Shape: STLport 4.6 `__copy_ptrs(first, last, result, __false_type)` -> the random-access `__copy` loop inlined (count by
//   the 0x34 reciprocal, signed `jle`, the source folded into an offset from the destination); `__fastcall` on the template
//   definition (the TU's free STLport helpers are fastcall). VC7.1 refuses to inline the plain-inline `__copy` (the
//   pointer-difference divide) and the memberwise operator=, so both are `__forceinline` (as the landed 0x00B7FD70). Local def base: engine/CDefClassBase.h is a 0x25-byte pack(1)
//   flat layout (vptr as data), unusable as a polymorphic base.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector;
template <class _Alloc> class vector<bool, _Alloc> {   // retail vector<bool>: 12 bytes of byte storage (landed 0x00462E2D)
public:
    bool* _M_start;
    bool* _M_finish;
    bool* _M_end_of_storage;
    vector<bool, _Alloc>& operator=(const vector<bool, _Alloc>& __x);   // 0x00462E78
};
}

class CFireHeartPatternDef : public CDefClassBase {   // 0x34 bytes in retail (Ego_r.pdb 60: the vector is 12 bytes here)
public:
    std::vector<bool> PadActive;                   // +0x28
    __forceinline CFireHeartPatternDef& operator=(const CFireHeartPatternDef& other)   // memberwise, inlined in retail
    {
        CDefClassBase::operator=(other);
        PadActive = other.PadActive;
        return *this;
    }
};

namespace std {

struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

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

template <class _InputIter, class _OutputIter>
_OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template CFireHeartPatternDef* __fastcall __copy_ptrs(CFireHeartPatternDef*, CFireHeartPatternDef*, CFireHeartPatternDef*, const __false_type&);

}   // namespace std