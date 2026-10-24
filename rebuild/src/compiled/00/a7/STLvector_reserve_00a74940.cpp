// _STL::vector::reserve(unsigned int n) -- STLport reserve for a vector of 16-byte C2DLineF lines: when the capacity is below n, copy the lines into a fresh n-element block and free the old one, or just allocate when the vector is empty; the size is kept
// Leaf: ?reserve@?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A74940, 171 bytes, thiscall `ret 4`.
// Caller: the line-vector copy at 0x00A74EA0 (this chunk, call 0x00A74ED4: a zeroed local vector reserved to the size).
// Callees: malloc 0x00BFEA0E (allocator::allocate), __uninitialized_copy(first, last, result, const __false_type&)
//   0x00A13490 (landed, `ret 8`), free 0x00BFEA14 (the inlined _M_clear: trivial destroy + deallocate).
// Label: catalog "Vector_Reserve_C2DLineF" is descriptive; the element name comes from the manifest label of the copy
//   callee (`_Copy_impl<C2DLineF*,C2DLineF*>`).
// Shape: STLport 4.x `reserve` verbatim (`if (capacity() < n) { old = size(); if (_M_start) { tmp =
//   _M_allocate_and_copy(n, _M_start, _M_finish); _M_clear(); } else tmp = allocate(n); _M_set(tmp, tmp + old, tmp + n); }`)
//   with `_M_allocate_and_copy` inline: allocate, then `__uninitialized_copy(first, last, result, _IsPODType())` for a
//   class element (__false_type). The empty tag is homed on the live `result` slot (n's dead argument slot): VC7.1 does
//   that when the tag-taking callee has a same-TU noinline body that never touches the tag.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class C2DVector {
public:
    float X;
    float Y;
};

class C2DLineF {                                            // 16 bytes: two end points
public:
    C2DVector Start;                                        // +0x00
    C2DVector End;                                          // +0x08
};

namespace _STL {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

#pragma auto_inline(off)
// 0x00A13490 (landed): the real body as a same-TU noinline function; it never touches the tag.
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
#pragma auto_inline(on)

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef __false_type _IsPODType;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, _IsPODType());
        return __result;
    }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void reserve(size_type __n);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::reserve(size_type __n)
{
    if (capacity() < __n) {
        const size_type __old_size = size();
        pointer __tmp;
        if (this->_M_start) {
            __tmp = _M_allocate_and_copy(__n, this->_M_start, this->_M_finish);
            _M_clear();
        }
        else {
            __tmp = this->_M_end_of_storage.allocate(__n);
        }
        _M_set(__tmp, __tmp + __old_size, __tmp + __n);
    }
}
}

template void _STL::vector<C2DLineF>::reserve(unsigned int);