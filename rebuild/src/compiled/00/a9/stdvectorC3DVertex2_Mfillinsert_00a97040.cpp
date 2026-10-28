// std::vector<C3DVertex2>::_M_fill_insert(iterator pos, size_type n, const value_type& x) -- STLport fill insert of n copies of one 32-byte mesh vertex: in place through a local copy of x when the spare capacity holds n (shift the tail, then fill), else hand over to _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VC3DVertex2@@V?$allocator@VC3DVertex2@@@std@@@std@@QAEXPAVC3DVertex2@@IABV3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A97040, 307 bytes, thiscall `ret 0Ch`.
// Identity (probable): the landed C3DMeshFileVerticesChunk::ReadFromFile 0x00ABD150 sizes its CArray<C3DVertex2> vertex
//   array through it (bound there as an invented CArray<C3DVertex2>::Insert); the 32-byte element matches Ego_r.pdb
//   C3DVertex2 {C3DVector Position, Normal; float U, V}. Its overflow path is _M_insert_overflow 0x00A96B30.
// Callees: __uninitialized_copy 0x00A95A10, __copy_backward_ptrs 0x00A95A70, fill 0x00A94600, __uninitialized_fill_n
//   0x00A94950 (all __fastcall, landed), _M_insert_overflow 0x00A96B30.
// Shape: STLport 4.6 vector.c `_M_fill_insert` verbatim (no EH); the tag-taking helpers are same-TU noinline stand-ins
//   that never read their tags, so the tags are homed on argument slots as in retail.
class C3DVector {                                          // user memberwise copy
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
class C3DVertex2 {                                         // 32 bytes (Ego_r.pdb), implicit copy
public:
    C3DVector Position;                                    // +0x00
    C3DVector Normal;                                      // +0x0C
    float     U;                                           // +0x18
    float     V;                                           // +0x1C
};
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);         // 0x00A96B30
};

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
__declspec(noinline) _BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result, const __false_type&)
{
    while (__first != __last)
        *--__result = *--__last;
    return __result;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
#pragma auto_inline(on)
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
                __uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy, __false_type());
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
} // namespace std
template void std::vector<C3DVertex2>::_M_fill_insert(C3DVertex2*, unsigned int, const C3DVertex2&);