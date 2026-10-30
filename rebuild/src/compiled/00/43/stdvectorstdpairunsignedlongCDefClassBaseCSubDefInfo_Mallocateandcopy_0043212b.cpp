// std::vector<std::pair<unsigned long, CDefClassBase::CSubDefInfo> >::_M_allocate_and_copy(unsigned int, const value_type*, const value_type*) -- allocate n elements and copy-construct [first, last) into them
// Leaf: _M_allocate_and_copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0043212B, length 85 bytes, `ret 0xc` (__fastcall; `this` unused), returns the block.
// Name: STLport 4.x `vector::_M_allocate_and_copy(n, first, last)`; single caller 0x00431FDC inside the vector assignment 0x00431F90
// that CParentDefClassBase::operator= (landed 0x00431ED0) uses for its SubDefInfoMap -- 12-byte pair<unsigned long, CSubDefInfo>
// elements. The catalog's "Vector_AllocateAndCopy" is a label.
// Callee: malloc @ 0x00BFEA0E (`__cdecl`, `pop ecx`; Fable's STLport node/element allocator is malloc-based).
// Pragma: #pragma optimize("s", on) -- unaligned entry and `pop ecx` after the cdecl call (size TU).
// Shape: STLport verbatim -- allocator<T>::allocate is `n != 0 ? malloc(n * sizeof(T)) : 0` (`lea eax,[eax+eax*2] ; shl eax,2`),
// then the NON-POD __uninitialized_copy (STLport has no POD trait for pair): `for (; first != last; ++first, ++cur)
// _Construct(&*cur, *first)` with the placement-new null test (`test esi,esi`); VC strength-reduces the destination to
// `first + (result - first)` in edx. Returns the block (`return result`), not the end of the copy.
// The element copy is pair's EXPLICIT copy constructor (STLport 4.x declares `pair(const pair&) : first(o.first), second(o.second)`):
// three dword moves through [edx+ecx+n]; with the implicit copy constructor /O1 emits `mov esi,ecx ; movsd x3` instead. In this
// size TU allocate, _Construct and __uninitialized_copy must be __forceinline (else all three stay out of line: 38 bytes).
// Model: local -- engine/CDefClassBase.h is data-only (no member functions); only the nested CSubDefInfo (its copy constructor)
// is needed, so it is local.
#pragma optimize("s", on)
#include <stdlib.h>
#include <new>
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& o) : first(o.first), second(o.second) {}   // STLport 4.x's explicit copy constructor
};
template <class T> class allocator {
public:
    __forceinline T* allocate(size_t n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class InputIter, class ForwardIter>
__forceinline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template <class T, class A = allocator<T> > class vector {
public:
    typedef T*       pointer;
    typedef const T* const_pointer;
    typedef size_t   size_type;
    T* _M_start;
    T* _M_finish;
    A  _M_end_of_storage_alloc;
    T* _M_end_of_storage;
    pointer _M_allocate_and_copy(size_type n, const_pointer first, const_pointer last)
    {
        pointer result = _M_end_of_storage_alloc.allocate(n);
        __uninitialized_copy(first, last, result);
        return result;
    }
};
template class vector<pair<unsigned long, CDefClassBase::CSubDefInfo> >;
}