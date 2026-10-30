// std::vector::_M_fill_insert(CPreTransposedBoneMatrix* position, unsigned int n, const CPreTransposedBoneMatrix& x) -- (vector<CPreTransposedBoneMatrix>) STLport insert of n copies of a 48-byte bone matrix: slide the tail inside the spare capacity and fill the hole from a copy of x, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@UCPreTransposedBoneMatrix@@V?$allocator@UCPreTransposedBoneMatrix@@@std@@@std@@QAEXPAUCPreTransposedBoneMatrix@@IABU3@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B7CAB0, 314 bytes, thiscall `ret 0Ch`, no EH frame (STLport built without exceptions).
// Identity: the landed vector<CPreTransposedBoneMatrix>::resize 0x00B7EDF0 (CShaderRenderManager::BoneMatrices, Ego_r.pdb
//   CArray<CPreTransposedBoneMatrix>) calls this row through the inline `insert(pos, n, x)` and binds this exact
//   `_M_fill_insert` spelling; the landed _M_insert_overflow 0x00B7C870 (chunk 591) names this row as its only caller.
//   The 48-byte reciprocal (0x2AAAAAAB, sar 3) and the 12-dword x_copy (`rep movsd`, ecx = 0Ch) are the element.
//   STLport 4.6 vector.c verbatim.
// Callees (all over CPreTransposedBoneMatrix, free helpers __fastcall): __uninitialized_copy 0x00B7AF30 (twice),
//   __copy_backward_ptrs 0x00B7AF60, fill 0x00B7AAC0 (both fills tail-merged), uninitialized_fill_n 0x00B7AFA0,
//   _M_insert_overflow 0x00B7C870.
// Shape: x_copy (48 bytes) plus the saved _M_finish make the 0x34 frame; retail homes both `__false_type()` tags on the
//   live n slot, so __uninitialized_copy and __copy_backward_ptrs get same-TU noinline bodies that never touch the tag
//   (the live-slot rule of 0x00BBD950, the same split: __copy_backward_ptrs out of line, __copy_backward inline).
inline void* operator new(unsigned int, void* p) { return p; }

struct CPreTransposedBoneMatrix {             // 48 bytes (a struct in FableWin.pdb: U-mangled)
    float Data[3][4];
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x00B7AF30
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);         // 0x00B7AFA0
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                          _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _RandomAccessIter, class _BidirectionalIter>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward_ptrs(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                       _BidirectionalIter __result, const __false_type&)
{                                                                                              // 0x00B7AF60
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val);                     // 0x00B7AAC0

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
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
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                       // 0x00B7C870
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

template void std::vector<CPreTransposedBoneMatrix>::_M_fill_insert(CPreTransposedBoneMatrix*, unsigned int,
                                                                    const CPreTransposedBoneMatrix&);