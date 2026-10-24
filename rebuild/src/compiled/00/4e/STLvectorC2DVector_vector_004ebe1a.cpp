// _STL::vector<C2DVector>::vector(const vector<C2DVector>& x) -- STLport vector copy constructor for the 8-byte 2D vectors: the base allocates exactly x.size() elements, then x's range is copy-constructed into them
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EBE1A, 72 bytes, `ret 4` (thiscall).
// Callers: CCardPositionsDef's _Construct @ 0x004EBDF1 (landed: the Offset member at +0x3C) and 0x004F9E3C.
// Label correction: the catalog's "VectorAppendRange" is descriptive; this is the copy constructor of vector<C2DVector>
//   (Ego_r.pdb CCardPositionsDef::Offset), the 8-byte stride (`sar eax,3`).
// Callee: _Vector_base<C2DVector>::_Vector_base(size_t n, const allocator&) @ 0x004EBE62 (out of line: nulls the three words,
//   `n ? malloc(n * 8) : 0`, start = finish = p, end = p + n; catalogued "Vector_AllocateRange_8byte").
// Shape: STLport `vector(const _Self& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator()) { _M_finish =
//   __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish, _M_start, _IsPODType()); }` -- the
//   empty allocator temporary (user constructor: no store) is homed in the high byte of the dead x slot (`lea eax,[ebp+0Bh]`),
//   the `__false_type` copy loop (placement-new null test, the implicit two-dword C2DVector copy) is inlined and
//   `__forceinline` (the size TU would out-line it). Frame-pointer size TU: `optimize("y", off)` + `("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>

inline void* operator new(size_t, void* where) { return where; }

struct C2DVector {                              // 8 bytes
    float X;                                    // +0x00
    float Y;                                    // +0x04
};

namespace _STL {
struct __false_type { __false_type() { } };
template <class T> struct __type_traits { typedef __false_type is_POD_type; };
template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class InputIter, class ForwardIter>
__forceinline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template <class T> struct allocator {           // STLport allocator<T>: empty
    allocator() { }
    allocator(const allocator&) { }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
};
template <class T> struct _Vector_base {
    T*                   _M_start;              // +0
    T*                   _M_finish;             // +4
    _STLP_alloc_proxy<T> _M_end_of_storage;     // +8
    _Vector_base(size_t n, const allocator<T>& a);   // 0x004EBE62
};
template <class T> struct vector : _Vector_base<T> {
    typedef typename __type_traits<T>::is_POD_type _IsPODType;
    size_t size() const { return size_t(this->_M_finish - this->_M_start); }
    allocator<T> get_allocator() const { return allocator<T>(); }
    vector(const vector& x)
        : _Vector_base<T>(x.size(), x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, _IsPODType());
    }
};
template vector<C2DVector>::vector(const vector<C2DVector>&);
}