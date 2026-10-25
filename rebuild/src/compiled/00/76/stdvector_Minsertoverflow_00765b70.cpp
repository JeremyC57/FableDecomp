// std::vector::_M_insert_overflow(vector<C3DVector>* pos, const vector<C3DVector>& x, const __false_type&, size_type fill_len, bool atend) -- STLport growth of a vector of point lists: move into a size + max(size, n) block with x inserted fill_len times at pos
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00765B70, length 272 bytes (thiscall, `ret 14h`).
// Identity: the catalog label "vector_insert_range_CFeatDef" is propagated. The element is 12 bytes (`imul 2AAAAAABh ;
//   sar edx,1`) and is copy-constructed through 0x006F0810, the landed vector<C3DVector> copy constructor; the old
//   block goes through 0x00765AD0 (landed: free every inner buffer, then the array -- the outer _M_clear). Callers:
//   the landed push_back 0x00765C80 (`_M_insert_overflow(_M_finish, x, __false_type(), 1, true)`) and the polygon /
//   patch-tessellation path builders 0x00763060 / 0x007634E0 (vector<vector<C3DVector> >).
// Callees: malloc 0x00BFEA0E (allocator::allocate), vector<C3DVector>::vector(const vector&) 0x006F0810,
//   __uninitialized_copy 0x00764970 (the suffix copy only), _M_clear 0x00765AD0.
// Shape: STLport 4.x `_M_insert_overflow(.., const __false_type&, ..)` verbatim (the landed 0x00569E30 recipe); VC7.1's
//   inliner expands the prefix __uninitialized_copy and the __uninitialized_fill_n loop (null-tested placement copies)
//   but keeps the suffix copy an out-of-line call (its tag homed on the `fill_len` slot); _M_clear is out of line.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

#pragma pack(push, 1)
class C3DVector {                                           // 1-aligned in retail
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
#pragma pack(pop)

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
inline _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                    const __false_type&)
{                                                                                              // 0x00764970
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
    vector(const vector& __x);                              // 0x006F0810 (vector<C3DVector>)
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void _M_clear();                                        // 0x00765AD0 (vector<vector<C3DVector> >)
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

template void vector<vector<C3DVector> >::_M_insert_overflow(vector<C3DVector>*, const vector<C3DVector>&,
                                                             const __false_type&, unsigned int, bool);

}