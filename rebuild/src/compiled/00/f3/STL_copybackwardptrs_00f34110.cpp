// std::__copy_backward_ptrs(CFireHeartPatternDef* first, CFireHeartPatternDef* last, CFireHeartPatternDef* result, const __false_type&) -- STLport backward assignment copy of a CFireHeartPatternDef range (0x34-byte defs) to a higher address: n = last - first assignments back to front, returns the new start
// Leaf: ??$__copy_backward_ptrs@PAVCFireHeartPatternDef@@PAV1@@std@@YIPAVCFireHeartPatternDef@@PAV1@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F34110, 74 bytes (+ int3 padding), __fastcall (first ecx, last edx), `ret 8`.
// Callees: the inlined implicit CFireHeartPatternDef::operator=: CDefClassBase::operator= @ 0x00431F10 and
//   vector<bool>::operator= @ 0x00462E78 (PadActive at +0x28).
// Identity: its only caller is vector<CFireHeartPatternDef>::_M_fill_insert at 0x00F33FCD (`copy_backward(pos, old_finish - n,
//   old_finish)` with the zero-stored __false_type tag); element 0x34 as the sibling __copy_ptrs 0x00F33EA0. Catalog
//   "vector_shift_up_CThingBuildingDef_0x34" is descriptive.
// Shape: STLport 4.6 `__copy_backward_ptrs(first, last, result, __false_type)` -> the random-access `__copy_backward` loop
//   `for (n = last - first; n > 0; --n) *--result = *--last;` inlined (the source folded into an offset from the
//   destination); `__fastcall` on the template definition; `__copy_backward` and the memberwise operator= `__forceinline`
//   (VC7.1 refuses the plain-inline chain). Local def base as in 0x00F33EA0.
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

template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
__forceinline _BidirectionalIter2 __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result,
                                           const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result,
                                                    const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template CFireHeartPatternDef* __fastcall __copy_backward_ptrs(CFireHeartPatternDef*, CFireHeartPatternDef*, CFireHeartPatternDef*, const __false_type&);

}   // namespace std