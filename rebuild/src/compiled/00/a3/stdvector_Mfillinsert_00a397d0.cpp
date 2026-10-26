// std::vector::_M_fill_insert(pointer position, size_type n, const value_type& x) -- (vector<pair<unsigned long, long> >) STLport fill-insert of n copies of an 8-byte element at position: shift in place when the capacity suffices (x copied first), else grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@U?$pair@KJ@std@@V?$allocator@U?$pair@KJ@std@@@2@@std@@QAEXPAU?$pair@KJ@2@IABU32@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A397D0, 280 bytes, thiscall, `ret 0Ch`.
// Identity (proven): element pair<unsigned long, long>; evidence: its overflow callee 0x009B61F0 is agent950's proven vector<pair<unsigned long, long> >::_M_insert_overflow (F008); its __copy_backward_ptrs callee 0x009B3100 is family H004's.
//   Family H018 (7 byte-identical bodies, agent951 G011's shape; each its own COMDAT: retail has no ICF, so each address is
//   one element type); the manifest label "Vector_Insert_Pair_Float_Attitude_Alt" is propagated/behavioural.
// Callees: __uninitialized_copy 0x009B25C0 (twice), __copy_backward_ptrs 0x009B3100 (out of line, __copy_backward inlined
//   into it: `ret 8`), fill 0x00A39220 (twice), uninitialized_fill_n 0x00A39380 (the three-argument front end),
//   _M_insert_overflow 0x009B61F0; the free helpers are __fastcall (first in ecx, second in edx).
// Shape: STLport 4.x vector.c `_M_fill_insert` verbatim; x_copy lives in the 8-byte frame (`sub esp,8`), the tags in
//   dead argument slots. The tag-taking helpers are same-TU `__declspec(noinline)` bodies that never read their tags.
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {

struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct output_iterator_tag { output_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

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
{                                                                                              // 0x009B25C0
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
{                                                                                              // 0x00A39380
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                     _BidirectionalIter __result, const random_access_iterator_tag&,
                                                     _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
__declspec(noinline) _BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first,
                                                                         _BidirectionalIter1 __last,
                                                                         _BidirectionalIter2 __result,
                                                                         const __false_type&)
{                                                                                              // 0x009B3100
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                              // 0x00A39220
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
                            bool __atend = false);                                             // 0x009B61F0
public:
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
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

typedef pair<unsigned long, long> _Elem;
template void vector<_Elem>::_M_fill_insert(_Elem*, unsigned int, const _Elem&);

} // namespace std