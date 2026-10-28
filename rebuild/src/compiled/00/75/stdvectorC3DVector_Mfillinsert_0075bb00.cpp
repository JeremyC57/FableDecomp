// std::vector<C3DVector>::_M_fill_insert(pointer position, size_type n, const value_type& x) -- STLport fill insert: with room, shift the tail by n (copy the last n into raw storage, copy_backward the rest, assign x) or fill the raw part first; without room, grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAEXPAVC3DVector@@IABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075bb00, 311 bytes, thiscall, `ret 0Ch`.
// Identity (proven): its __uninitialized_copy 0x0066DDF0 is bound ??$__uninitialized_copy@PAVC3DVector@@PAV1@@std@@... and its grow callee 0x0066E370 is bound to the vector<C3DVector> / CArray<C3DVector> _M_insert_overflow; callers the C3DVector-vector insert (tail jump from 0x0075BFF0) and resize 0x0075C040.
// Callees: __uninitialized_copy 0x0066DDF0, __copy_backward 0x0075B5B0, fill 0x0075B430, uninitialized_fill_n 0x0075B470 (all __fastcall),
//   _M_insert_overflow 0x0066E370 (thiscall).
// Shape (family L059 = family L004's model with 12-byte trivially destroyed records): __x_copy in the frame, the
//   tag-taking helpers as same-TU noinline bodies, __copy_backward and _M_insert_overflow as declarations.
inline void* operator new(unsigned int, void* p) { return p; }
namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x0066DDF0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                      const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{                                                                                              // 0x0075B470
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&,
                                              _Distance*);                                     // 0x0075B5B0
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                           _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                              // 0x0075B430
    for (; __first != __last; ++__first)
        *__first = __val;
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                                             // 0x0066E370
    public:
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
    protected:
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(pointer __position, size_type __n, const _Tp& __x)
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
class C3DVector {                                          // user copy constructor (FableWin/Ego_d ??0C3DVector@@QAE@ABV0@@Z)
public:
    float X, Y, Z;                                        // +0x00
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

template void std::vector<C3DVector >::_M_fill_insert(C3DVector*, unsigned int, const C3DVector&);