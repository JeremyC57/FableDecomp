// CJoystickPositionTracker::HasPerformed360ClockwiseMove() const -- walk the last 30 stick positions: fail on a position inside the 0.15 dead zone or a step that turns by more than 110 degrees, succeed once the summed turning (acos of each step's dot product, in turns) passes 290/360
// Grade: DIFFER(256v256) -- one swapped pair: retail forms the step dot product x first (`fld prev.X ; fmul [esi]` then the Y product); ours always forms the Y product first, for X-first and Y-first source order and an inline Dot member alike. Everything else (front(), the STLport operator[] copies, dead zone, acos sum) matches. Next: previous held as two floats or a pointer
// Leaf: ?HasPerformed360ClockwiseMove@CJoystickPositionTracker@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0DCD0, 256 bytes, thiscall, plain `ret`, bool in al. No direct callers.
// Identity: Ego_r.pdb `?HasPerformed360ClockwiseMove@CJoystickPositionTracker@@QBE_NXZ` -- the only large function of the
//   tracker's run (HasPerformed360AntiClockwiseMove .. HadPerformedMaximumDamageMove are stubs, FrameUpdate and
//   HasJabbedStickInDirection follow) and its deque<C2DVector> RecentPositions is the deque this TU's STLport rows serve
//   (the chunk label "Path_FindNextValidPoint" is descriptive).
// Layout: CJoystickPositionTracker = deque<C2DVector> RecentPositions (STLport 4.6: _M_start +0x00, _M_finish +0x10,
//   _M_map +0x20, _M_map_size +0x24).
// Callees: _Deque_iterator_base<C2DVector>::_M_advance @ 0x00A0E2A0, _CIacos (thunk 0x00BFEBA2); sqrt / cos are intrinsics.
// Constants: 110 degrees in radians (double) 0x0129CA50, 0.15f 0x012786A8, 1/(2pi) 0x01237F08, 290/360 0x0129CA48.
// Shape: RecentPositions[i] is STLport's `*(_M_start + i)` (the iterator copied into a temporary, advanced, copied into the
//   result); the position is read through the reference, the previous one is a C2DVector local (front() first) copied as dwords.
extern "C" double __cdecl sqrt(double x);
extern "C" double __cdecl cos(double x);
extern "C" double __cdecl acos(double x);
#pragma intrinsic(sqrt, cos, acos)

class C2DVector {                                           // 8 bytes
public:
    float X, Y;
    C2DVector() { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
    float GetLength() const { return (float)sqrt(X * X + Y * Y); }
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
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

template <class _Tp, class _Alloc = allocator<_Tp> > class deque {
public:
    typedef _Tp value_type;
    typedef unsigned int size_type;
    typedef int difference_type;
    typedef _Tp** _Map_pointer;
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef const _Tp& const_reference;

    iterator     _M_start;                                  // +0x00
    iterator     _M_finish;                                 // +0x10
    _Map_pointer _M_map;                                    // +0x20
    size_type    _M_map_size;                               // +0x24

    const_reference operator[](size_type __n) const { return this->_M_start[difference_type(__n)]; }
    const_reference front() const { return *this->_M_start; }
};
}

class CJoystickPositionTracker {
public:
    std::deque<C2DVector> RecentPositions;                  // +0x00
    bool HasPerformed360ClockwiseMove() const;
};

bool CJoystickPositionTracker::HasPerformed360ClockwiseMove() const
{
    float minCosine = (float)cos(1.9198622098128482);       // 110 degrees
    C2DVector previous = RecentPositions.front();
    float totalTurns = 0.0f;
    for (long i = 1; i < 30; ++i) {
        const C2DVector& position = RecentPositions[i];
        if (position.GetLength() <= 0.15f)
            return false;
        float cosine = previous.X * position.X + previous.Y * position.Y;
        if (cosine < minCosine)
            return false;
        totalTurns += (float)acos(cosine) * 0.15915494f;
        if (totalTurns > 0.80555558f)
            return true;
        previous = position;
    }
    return false;
}
