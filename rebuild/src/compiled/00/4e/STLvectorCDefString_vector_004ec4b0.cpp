// std::vector<CDefString>::vector(const vector<CDefString>& x) -- STLport vector copy constructor for a CArray<CDefString> (4-byte def-string handles): the base allocates exactly x.size() elements, then x's range is copy-constructed into them
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EC4B0, 66 bytes, `ret 4` (thiscall); the vector<C2DVector> twin 0x004EBE1A at a
//   4-byte stride.
// Callers: CObjectAugmentationParticleSet's copy constructor @ 0x004EC46D (StartPoint at +0x14, EndPoint at +0x20; Ego_r.pdb
//   CArray<CDefString> members, retail STLport vectors of 12 bytes), reached from the augmentation map's node creation 0x004EC43E.
// Label correction: the catalog's "VectorAppendRange" is descriptive; this is the copy constructor of vector<CDefString>.
// Callee: _Vector_base<CDefString>::_Vector_base(size_t n, const allocator&) @ 0x004EC4F2 (out of line; catalogued
//   "Vector_AllocateRange_4byte").
// Shape: STLport `vector(const _Self& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator()) { _M_finish =
//   __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish, _M_start, _IsPODType()); }` -- the
//   empty allocator temporary (user constructor: no store) is homed in the high byte of the dead x slot (`lea eax,[ebp+0Bh]`),
//   the `__false_type` copy loop (placement-new null test, the implicit one-dword CDefString copy) is inlined and
//   `__forceinline` (the size TU would out-line it). Frame-pointer size TU: `optimize("y", off)` + `("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>

inline void* operator new(size_t, void* where) { return where; }

class CDefString {                              // 4 bytes
public:
    long TablePos;                              // +0x00 def-string table index
};

namespace std {
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
    _Vector_base(size_t n, const allocator<T>& a);   // 0x004EC4F2
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
template vector<CDefString>::vector(const vector<CDefString>&);
}