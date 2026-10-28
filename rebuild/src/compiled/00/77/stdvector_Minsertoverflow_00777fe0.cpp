// std::vector::_M_insert_overflow(GoodsPlace* pos, const GoodsPlace& x, const __false_type&, size_type fill_len, bool atend) -- STLport vector growth for CTCShop's 36-byte goods places: move into a size + max(size, n) block with x inserted fill_len times at pos
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00777FE0, length 256 bytes (thiscall, `ret 14h`).
// Identity: the catalog label "Vector_InsertRange_Fill_CKeyCamera" is descriptive and wrong about the element: the stride
//   is 0x24 (`imul 38E38E39h ; sar edx,3`), and the only caller is the landed push_back 0x00778210 of the goods-place
//   vector (CTCShop::GoodsPlaces, Ego_r.pdb: GoodsPlace = {C3DVector pos; CRightHandedSet rhset}, 36 bytes), reached
//   from CTCGraphicAppearance_InitGoodsPlaces / CTCShop::GetFreeSaleItemPlaces with `(_M_finish, x, tag, 1, true)`.
// Callees: malloc 0x00BFEA0E (allocator::allocate), free 0x00BFEA14 (the inlined _M_clear's deallocate),
//   __uninitialized_copy 0x00776B60 (twice: prefix and suffix), __uninitialized_fill_n 0x00776BC0.
// Shape: STLport 4.x `_M_insert_overflow(.., const __false_type&, ..)` verbatim (the landed 0x00569E30 recipe):
//   `len = old_size + (max)(old_size, fill_len)` (max through references: the `lea ecx` pick between the spilled size
//   and the fill_len slot), allocate, copy the prefix, `_Copy_Construct` when fill_len == 1 (nine dword moves under the
//   placement-new null test) else fill_n, copy the suffix unless atend, `_M_clear()` (the element's destructor is
//   trivial, so _M_clear collapses to `if (_M_start) free(_M_start)`), `_M_set(..)`. The tag-taking callees carry
//   same-TU `__declspec(noinline)` bodies that never touch the tag, so VC7.1 homes each `__false_type()` temporary
//   on the live `atend` slot; only _M_insert_overflow is graded.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

class CRightHandedSet {                                     // Ego_r.pdb, 24 bytes
public:
    C3DVector Forward;                                      // +0x00
    C3DVector Up;                                           // +0x0c
};

class CTCShop {
public:
    struct GoodsPlace {                                     // Ego_r.pdb, 36 bytes
        C3DVector       pos;                                // +0x00
        CRightHandedSet rhset;                              // +0x0c
    };
};

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
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
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                              // 0x00776B60
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                              // 0x00776BC0
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
    protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    public:
    protected:
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    public:
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
    public:
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

template void vector<CTCShop::GoodsPlace>::_M_insert_overflow(CTCShop::GoodsPlace*, const CTCShop::GoodsPlace&,
                                                              const __false_type&, unsigned int, bool);

}