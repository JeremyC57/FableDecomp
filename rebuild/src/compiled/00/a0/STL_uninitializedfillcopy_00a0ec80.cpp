// std::__uninitialized_fill_copy(_Deque_iterator result, _Deque_iterator mid, const C2DVector& x, _Deque_iterator first, _Deque_iterator last) -- fill [result, mid) with x, then copy-construct [first, last) at mid and return the end of the copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0EC80, length 191 bytes (`__fastcall`: hidden result in ecx, x in edx,
// `ret 40h`). Caller: 0x00A0F9FB (the deque fill-insert path).
// Callees: __uninitialized_fill @ 0x00A0E740 and __uninitialized_copy @ 0x00A0E3D0 (both with the __false_type tag).
// Shape: as 0x00A0EBC0 -- the forwarders written as direct calls (`__uninitialized_fill(__result, __mid, __x, __false_type());
// return __uninitialized_copy(__first, __last, __mid, __false_type());`), the copy's result built straight through the hidden
// pointer; the callees are same-TU noinline stand-ins.
// Leaf: ??$__uninitialized_fill_copy@
// Model: STLport 4.6 as TLC links it -- deque<C2DVector> (Ego_r.pdb: CJoystickPositionTracker::RecentPositions) with
// 8-byte elements in 0x80-byte node buffers (16 per node); the deque is {_M_start, _M_finish, _M_map, _M_map_size} =
// +0x00/+0x10/+0x20/+0x24, an iterator {_M_cur, _M_first, _M_last, _M_node}; the allocator is malloc 0x00BFEA0E / free
// 0x00BFEA14 (deallocate null-tests). The TU's free functions are `__fastcall` (/Gr): a 16-byte iterator result comes back
// through the hidden pointer in ecx, the first register-sized argument goes in edx (ecx when there is no hidden result).
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class C2DVector {                                           // 8 bytes
public:
    float X, Y;
    C2DVector() { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp*) { }
template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; typedef const _Tp* pointer; };

template <class _Tp> struct _Deque_iterator_base {          // 16 bytes
    enum _Constants { _blocksize = 128, __buffer_size = (sizeof(_Tp) < (unsigned int)_blocksize ? ((unsigned int)_blocksize / sizeof(_Tp)) : 1) };
    typedef int difference_type;
    typedef _Tp** _Map_pointer;
    typedef _Deque_iterator_base<_Tp> _Self;

    _Tp*        _M_cur;                                     // +0x00
    _Tp*        _M_first;                                   // +0x04
    _Tp*        _M_last;                                    // +0x08
    _Map_pointer _M_node;                                   // +0x0c

    _Deque_iterator_base() : _M_cur(0), _M_first(0), _M_last(0), _M_node(0) { }
    difference_type _M_subtract(const _Self& __x) const
    {
        return difference_type(__buffer_size) * (_M_node - __x._M_node - 1) + (_M_cur - _M_first) + (__x._M_last - __x._M_cur);
    }
    void _M_increment()
    {
        if (++_M_cur == _M_last) {
            _M_set_node(_M_node + 1);
            _M_cur = _M_first;
        }
    }
    void _M_decrement()
    {
        if (_M_cur == _M_first) {
            _M_set_node(_M_node - 1);
            _M_cur = _M_last;
        }
        --_M_cur;
    }
    void _M_advance(difference_type __n);                   // 0x00A0E2A0
    void _M_set_node(_Map_pointer __new_node)
    {
        _M_last = (_M_first = *(_M_node = __new_node)) + difference_type(__buffer_size);
    }
};

template <class _Tp, class _Traits> struct _Deque_iterator : public _Deque_iterator_base<_Tp> {
    typedef typename _Traits::reference reference;
    typedef int difference_type;
    typedef _Deque_iterator<_Tp, _Traits> _Self;
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > _Nonconst_self;

    _Deque_iterator() { }
    _Deque_iterator(const _Nonconst_self& __x) : _Deque_iterator_base<_Tp>(__x) { }

    reference operator*() const { return *this->_M_cur; }
    difference_type operator-(const _Self& __x) const { return this->_M_subtract(__x); }
    _Self& operator++() { this->_M_increment(); return *this; }
    _Self& operator--() { this->_M_decrement(); return *this; }
    _Self& operator+=(difference_type __n) { this->_M_advance(__n); return *this; }
    _Self operator+(difference_type __n) const
    {
        _Self __tmp = *this;
        return __tmp += __n;
    }
    _Self& operator-=(difference_type __n) { return *this += -__n; }
    _Self operator-(difference_type __n) const
    {
        _Self __tmp = *this;
        return __tmp -= __n;
    }
    reference operator[](difference_type __n) const { return *(*this + __n); }
};

template <class _Tp, class _Traits1, class _Traits2>
inline bool __fastcall operator==(const _Deque_iterator<_Tp, _Traits1>& __x, const _Deque_iterator<_Tp, _Traits2>& __y) { return __x._M_cur == __y._M_cur; }
template <class _Tp, class _Traits1, class _Traits2>
inline bool __fastcall operator!=(const _Deque_iterator<_Tp, _Traits1>& __x, const _Deque_iterator<_Tp, _Traits2>& __y) { return __x._M_cur != __y._M_cur; }

// Same-TU stand-ins for the out-of-line __uninitialized_copy 0x00A0E3D0 and __uninitialized_fill 0x00A0E740 (not the retail
// bodies; those are the separate rows): a noinline body that reaches an external helper keeps VC7.1 passing the tag in edx.
void UninitCopyLoop_00a0e3d0(void* out, const void* first, const void* last, const void* result);
void UninitFillLoop_00a0e740(const void* first, const void* last, const void* value);
#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    UninitCopyLoop_00a0e3d0(&__result, &__first, &__last, &__result);
    return __result;
}
template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall __uninitialized_fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __x, const __false_type&)
{
    UninitFillLoop_00a0e740(&__first, &__last, &__x);
}
#pragma auto_inline(on)

template <class _ForwardIter, class _Tp, class _InputIter>
_ForwardIter __fastcall __uninitialized_fill_copy(_ForwardIter __result, _ForwardIter __mid, const _Tp& __x, _InputIter __first, _InputIter __last)
{
    __uninitialized_fill(__result, __mid, __x, __false_type());
    return __uninitialized_copy(__first, __last, __mid, __false_type());
}

template _Deque_iterator<C2DVector, _Nonconst_traits<C2DVector> > __fastcall __uninitialized_fill_copy(_Deque_iterator<C2DVector, _Nonconst_traits<C2DVector> >, _Deque_iterator<C2DVector, _Nonconst_traits<C2DVector> >, const C2DVector&, _Deque_iterator<C2DVector, _Nonconst_traits<C2DVector> >, _Deque_iterator<C2DVector, _Nonconst_traits<C2DVector> >);
}