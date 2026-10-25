// std::_Deque_iterator_base<C2DVector>::_M_advance(int n) -- move a deque iterator by n elements: inside the current 16-element node just bump the cursor, else step the node pointer by the signed node offset and re-base the cursor in the new node
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0E2A0, length 105 bytes (thiscall `ret 4`, + int3 padding). 13 callers in the TU
// (the iterator operator+= / operator-= inlined into operator+ 0x00A0E980, operator[] 0x00A0E9E0 / 0x00A0EE00, operator- 0x00A0EA30,
// the deque insert/erase paths). The landed operator+ 0x00A0E980 calls this "operator+="; it is the void _M_advance.
// Callees: none.
// Shape: STLport's in-class body verbatim: `__node_offset = __offset > 0 ? __offset / __buffer_size :
// -difference_type((-__offset - 1) / __buffer_size) - 1` with the enum (signed) buffer size gives the two `cdq ; and edx,0Fh ;
// add ; sar 4` divisions (the second as `(offset + 1) / 16 - 1`), then `_M_set_node(_M_node + __node_offset)` and
// `_M_cur = _M_first + (__offset - __node_offset * 16)`.
// Leaf: ?_M_advance@
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

template <class _Tp> void _Deque_iterator_base<_Tp>::_M_advance(difference_type __n)
{
    difference_type __offset = __n + (_M_cur - _M_first);
    if (__offset >= 0 && __offset < difference_type(__buffer_size))
        _M_cur += __n;
    else {
        difference_type __node_offset = __offset > 0 ? __offset / __buffer_size : -difference_type((-__offset - 1) / __buffer_size) - 1;
        _M_set_node(_M_node + __node_offset);
        _M_cur = _M_first + (__offset - __node_offset * difference_type(__buffer_size));
    }
}

template void _Deque_iterator_base<C2DVector>::_M_advance(int);
}