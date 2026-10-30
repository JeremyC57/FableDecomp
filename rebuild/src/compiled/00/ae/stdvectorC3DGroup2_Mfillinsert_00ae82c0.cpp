// std::vector<C3DGroup2>::_M_fill_insert(C3DGroup2* position, unsigned int n, const C3DGroup2& x) -- STLport insert of n copies of a 16-byte skinning group: slide the tail inside the spare capacity and fill the hole from a copy of x, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VC3DGroup2@@V?$allocator@VC3DGroup2@@@std@@@std@@QAEXPAVC3DGroup2@@IABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE82C0, 270 bytes, thiscall `ret 0Ch`.
// Identity: STLport 4.6 vector.c `_M_fill_insert` over C3DGroup2 (CArray<C3DGroup2> = the particle program's SimGroups /
//   NonSimGroups): its callees are the landed vector<C3DGroup2> helpers and the growth path is the landed
//   `_M_insert_overflow` 0x00AE81F0 (whose header names this row as its `_M_fill_insert` caller). The landed resize
//   0x00AE8540 reaches it through the inline `insert(pos, n, x) { _M_fill_insert(pos, n, x); }`, so the out-of-line symbol is
//   _M_fill_insert (as for the 132 other bound vector instances), not `insert` (that resize binds `?insert@...` here;
//   NAME_FIXES.tsv).
// Callees: vector<C3DVertexBlend2>(const vector&) 0x00AE5D80 (the x_copy's blend array; landed leaf), __uninitialized_copy
//   <C3DGroup2*, C3DGroup2*> 0x00AE7480 (twice), __copy_backward<C3DGroup2*, C3DGroup2*, int> 0x00AE76A0 (behind the inline
//   __copy_backward_ptrs), uninitialized_fill_n<C3DGroup2*, unsigned int, C3DGroup2> 0x00AE75B0, fill<C3DGroup2*,
//   C3DGroup2> 0x00AE79F0 (both fills tail-merged), free 0x00BFEA14 (x_copy's destructor), _M_insert_overflow 0x00AE81F0.
//   Every free helper is __fastcall (first/last in ecx/edx), as in the landed siblings.
// Shape: STLport 4.6 verbatim; x_copy lives in the 16-byte frame. Retail homes the first `__false_type()` on the LIVE
//   `position` slot ([esp+24h], re-read after the call): that needs a same-TU noinline __uninitialized_copy body that
//   never touches the tag (EXTRA_RULES; as in the landed 0x00AE81F0). Declared-only gave the x slot (270v270, one lea).
// Model: real STLport 4.6 vector over _Vector_base + _STLP_alloc_proxy (the landed 0x00AE81F0 model); CArray<T> derives
//   from std::vector<T> with the implicit copy constructor, so copying a group calls the vector<C3DVertexBlend2> copy
//   constructor directly; C3DGroup2 is {long BoneIndex, CArray<C3DVertexBlend2> VertexBlends} = 16 bytes.
extern "C" void __cdecl free(void* p);                             // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x00AE7480
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);         // 0x00AE75B0
template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
_BidirectionalIter2 __fastcall __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                               _BidirectionalIter2 __result, const random_access_iterator_tag&,
                                               _Distance*);                                           // 0x00AE76A0
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val);                     // 0x00AE79F0

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    vector(const vector<_Tp, _Alloc>& __x);                              // 0x00AE5D80 (C3DVertexBlend2)
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                       // 0x00AE81F0 (C3DGroup2)
public:
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}
}

class C3DVertexBlend2 { public: long VertexIndex; float BlendValue; };   // 8 bytes

template <class T> class CArray : public std::vector<T> {   // retail CArray<T>: an STLport vector (12 bytes)
};

class C3DGroup2 { public:                                       // 16 bytes in retail
    long                     BoneIndex;                  // +0x0
    CArray<C3DVertexBlend2>  VertexBlends;               // +0x4
};

template void std::vector<C3DGroup2>::_M_fill_insert(C3DGroup2*, unsigned int, const C3DGroup2&);