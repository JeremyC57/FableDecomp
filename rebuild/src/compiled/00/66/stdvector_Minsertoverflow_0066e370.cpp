// std::vector::_M_insert_overflow(C3DVector* position, const C3DVector& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport non-POD vector growth for 3D points: allocate old + max(old, n), copy the head, construct the n new copies, copy the tail, free the old block
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066E370, 272 bytes (270 without padding), thiscall `ret 14h`. Callers: 65 sites,
//   every vector<C3DVector> push_back that runs out of room (CRegionMinimap::PopulateRegionMinimap, the camera manager,
//   CCardinalSpline::AppendControlPoint 0x0075F980 landed, CTCCreatureFlyInCircles::QueueSwoop, ...).
// Callees: malloc 0x00BFEA0E (allocate), __uninitialized_copy(.., const __false_type&) 0x0066DDF0 (landed; the tail
//   copy only), free 0x00BFEA14 (deallocate).
// Label correction: the catalog "Vector_InsertRange" -- this is STLport 4.6 `vector<C3DVector>::_M_insert_overflow`.
// Shape: STLport 4.6 verbatim for a non-POD element (C3DVector has user constructors, no __type_traits specialisation):
//   const-reference `(max)`, `n ? malloc(n * 12) : 0`, C3DVector's trivial destructor makes _M_clear's _Destroy vanish.
//   Retail's inliner expanded the head __uninitialized_copy, the `fill_len == 1` _Construct and __uninitialized_fill_n
//   (placement copies with the null test) but left the TAIL __uninitialized_copy as the out-of-line 0x0066DDF0 call. The
//   lane's VC7.1 declines to auto-inline any four-argument loop helper at /O2 (probe: even a plain `int` 4th parameter
//   blocks it), so the inlining decision is spelled out: __uninitialized_fill_n is `__forceinline`, the head copy goes
//   through a `__forceinline` twin of __uninitialized_copy, the tail calls the declared out-of-line instance.
//   C3DVector's memberwise user copy constructor (as the landed 0x0066DDF0) moves each float through one register;
//   the implicit copy routes the source/destination through esi/ebx (291 v 270).
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14
inline void* operator new(unsigned int, void* __p) throw() { return __p; }

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&);   // 0x0066DDF0 (landed)
template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy_head(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

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
    __new_finish = __uninitialized_copy_head(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<C3DVector>::_M_insert_overflow(C3DVector*, const C3DVector&, const __false_type&, unsigned int, bool);
}