// std::vector<CArray<CRandomAppearanceMorph::CBodyPartMesh>>::_M_fill_insert(pointer position, size_type n, const value_type& x) -- STLport fill insert: with room, shift the tail by n (copy the last n into raw storage, copy_backward the rest, assign x) or fill the raw part first; without room, grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@V?$CArray@VCBodyPartMesh@CRandomAppearanceMorph@@@@V?$allocator@V?$CArray@VCBodyPartMesh@CRandomAppearanceMorph@@@@@std@@@std@@IAEXPAV?$CArray@VCBodyPartMesh@CRandomAppearanceMorph@@@@IABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086eca0, 300 bytes, thiscall, `ret 0Ch`.
// Identity (proven): bound ?_M_fill_insert@?$vector@V?$CArray@VCBodyPartMesh@CRandomAppearanceMorph@@@@...@Z; its element copy / destructor callees are bound to CArray<CRandomAppearanceMorph::CBodyPartMesh>; Ego_r.pdb CRandomAppearanceMorph::BodyParts is CArray<CArray<CBodyPartMesh> >; caller the resize 0x0086EDE0.
// Callees: CArray<CBodyPartMesh>::CArray(const CArray&) 0x00675230 and ~CArray 0x006755A0 (both bound), __uninitialized_copy 0x0086E710, __copy_backward 0x0086E770, uninitialized_fill_n 0x0086E740, fill 0x0086E920 (landed under stand-in names), _M_insert_overflow 0x0086EB70 (L003).
// Shape (family L004, representative 0x0051C920): STLport 4.6 vector.c `_M_fill_insert` verbatim; __x_copy lives in the
//   frame and is destroyed at the end of the in-place arm; __copy_backward_ptrs is inline over the out-of-line
//   __copy_backward; the tag-taking helpers (__uninitialized_copy, uninitialized_fill_n, fill) are same-TU
//   `__declspec(noinline)` bodies (they never read their tags), __copy_backward and _M_insert_overflow are declarations.
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
{                                                                                              // 0x0086E710
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
{                                                                                              // 0x0086E740
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&,
                                              _Distance*);                                     // 0x0086E770
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                           _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                              // 0x0086E920
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
                            bool __atend = false);                                             // 0x0086EB70
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
}
class CRandomAppearanceMorph {
public:
    class CBodyPartMesh;
};
template <class T> class CArray {                           // retail CArray<T> = 12-byte STLport vector
public:
    T* m_first;
    T* m_last;
    T* m_end;
    CArray(const CArray& other);                          // 0x00675230 for CBodyPartMesh
    ~CArray();                                            // 0x006755A0 for CBodyPartMesh
    CArray& operator=(const CArray& other);
};
template void std::vector<CArray<CRandomAppearanceMorph::CBodyPartMesh> >::_M_fill_insert(CArray<CRandomAppearanceMorph::CBodyPartMesh>*, unsigned int, const CArray<CRandomAppearanceMorph::CBodyPartMesh>&);