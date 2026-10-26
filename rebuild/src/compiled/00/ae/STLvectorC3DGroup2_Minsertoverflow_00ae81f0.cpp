// std::vector<C3DGroup2>::_M_insert_overflow(C3DGroup2* pos, const C3DGroup2& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport vector growth for the 16-byte skinning groups (CArray<C3DGroup2>): move into a size + max(size, fill_len) block with x inserted fill_len times at pos
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE81F0, 195 bytes, __fastcall `ret 14h`.
// Callers: _M_fill_insert 0x00AE82C0 (`_M_insert_overflow(pos, x, __false_type(), n)` when the spare capacity is short; that
//   is how resize 0x00AE8540 grows). push_back 0x00AE83D0 inlines its own copy of this body.
// Callees: malloc 0x00BFEA0E (allocator::allocate), __uninitialized_copy 0x00AE7480 (landed, x2), __uninitialized_fill_n
//   0x00AE74C0 (landed), CArray<C3DVertexBlend2> copy ctor 0x00AE5D80 (the fill_len == 1 copy construct), _M_clear
//   0x00AE7FA0 (destroy the old groups, free the block).
// Layout: retail CArray<T> is an STLport vector {_M_start, _M_finish, _M_end_of_storage}; C3DGroup2 is {long BoneIndex,
//   CArray<C3DVertexBlend2> VertexBlends} = 16 bytes with the implicit copy constructor.
// Label: catalog "Vector_Insert_Pair128BitMultiple" is descriptive.
// Shape: STLport 4.6 _vector.c verbatim -- the shape of the landed vector<string>::_M_insert_overflow 0x00569E30 and of
//   the PSC-entry instantiation 0x00AF37F0: (max) through references, allocate, copy the prefix, _Copy_Construct when
//   fill_len == 1 else fill_n, copy the suffix unless atend, _M_clear(), _M_set(). The tag-taking callees carry same-TU
//   noinline bodies that never touch the tag, so every `__false_type()` temporary is homed on the atend slot.
extern "C" void* __cdecl malloc(unsigned int size);                // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

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
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                              // 0x00AE74C0
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
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
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void _M_clear();                                                                           // 0x00AE7FA0
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Copy_Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
}

class C3DVertexBlend2 { public: long Vertex; float Weight; };   // 8 bytes

template <class T> class CArray : public std::vector<T> {   // retail CArray<T>: an STLport vector (12 bytes)
public:
    CArray(const CArray& other);                         // 0x00AE5D80 for CArray<C3DVertexBlend2>
};

class C3DGroup2 { public:                                       // 16 bytes in retail
    long                     BoneIndex;                  // +0x0
    CArray<C3DVertexBlend2>  VertexBlends;               // +0x4
};

template void std::vector<C3DGroup2>::_M_insert_overflow(C3DGroup2*, const C3DGroup2&, const std::__false_type&, unsigned int, bool);