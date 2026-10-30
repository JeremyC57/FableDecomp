// std::__copy_backward_ptrs<CCardPositionsDef*,CCardPositionsDef*>(CCardPositionsDef* first, CCardPositionsDef* last, CCardPositionsDef* result, const __false_type&) -- STLport backward assignment copy of card-position defs: assign [first, last) onto the range ending at result, last element first, return the new start
// Leaf: ??$__copy_backward_ptrs@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004F9F62, 65 bytes, `ret 8` (__fastcall: first in ecx, last in edx, result and the
//   tag reference on the stack -- a /Gr algorithm TU).
// Caller: 0x004F9E9F, vector<CCardPositionsDef>::_M_fill_insert (shift the tail up before filling the gap).
//   Catalog label "CActiveFile_Copy_Element" is propagated.
// Element: CCardPositionsDef (retail 0x48: CParentDefClassBase 0x39 bytes, vector<C2DVector> Offset +0x3C); its
//   assignment runs CParentDefClassBase::operator= @ 0x00431ED0 then the Offset vector operator= @ 0x0046EABD (manifest
//   "CActiveFile_Copy" is propagated), written as a `__forceinline` memberwise operator=. The engine header
//   CParentDefClassBase.h carries the Ego_r donor size 0x3D, so a local model stands in.
// Shape: STLport `__copy_backward_ptrs(first, last, result, __false_type) { return __copy_backward(first, last, result,
//   random_access_iterator_tag(), (ptrdiff_t*)0); }` with the counted `__copy_backward` inlined (`*--result = *--last`,
//   last walking as `edi + esi`); the count is an `idiv` by 0x48. Size TU (`dec ebx ; jne`).
#pragma optimize("s", on)

class C2DVector {
public:
    float X;
    float Y;
};

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class T> class allocator {
public:
    allocator() { }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

template <class T, class A> class _Vector_base {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;          // +0x08
};

template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    vector<T, A>& operator=(const vector<T, A>& x);      // vector<C2DVector>: 0x0046EABD
};

template <class BidirectionalIter1, class BidirectionalIter2, class Distance>
__forceinline BidirectionalIter2 __copy_backward(BidirectionalIter1 first, BidirectionalIter1 last,
                                                 BidirectionalIter2 result, const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2 __fastcall __copy_backward_ptrs(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result,
                                                   const __false_type&)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

}

// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"

class CCardPositionsDef : public CParentDefClassBase {   // Ego_r.pdb, retail sizeof 0x48
public:
    std::vector<C2DVector> Offset;                  // +0x3C
    __forceinline CCardPositionsDef& operator=(const CCardPositionsDef& other)
    {
        CParentDefClassBase::operator=(other);
        Offset = other.Offset;
        return *this;
    }
};

template CCardPositionsDef* __fastcall std::__copy_backward_ptrs<CCardPositionsDef*, CCardPositionsDef*>(CCardPositionsDef* first,
    CCardPositionsDef* last, CCardPositionsDef* result, const std::__false_type&);