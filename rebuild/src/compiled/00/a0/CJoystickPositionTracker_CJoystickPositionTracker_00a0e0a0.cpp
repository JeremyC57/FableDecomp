// CJoystickPositionTracker::CJoystickPositionTracker() -- build the stick-position history: an empty deque of C2DVector, then resized to 30 zero positions
// Leaf: ??0CJoystickPositionTracker@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0E0A0, 80 bytes, __fastcall `ret`, returns `this`.
// Identity (label correction): the catalog's "pair<EHeroMorphType,CParticleMorphs::CEntry>" is propagated. Both callers
//   (0x00489383, 0x0048D666) are CInputProcessCombat constructors (vtable 0x012380B4) building the member at +0x28 =
//   Ego_r.pdb CInputProcessCombat::PositionTracker; FableWin.pdb ??0CJoystickPositionTracker@@QAE@XZ. Its only member is
//   RecentPositions, a deque<C2DVector> (TLC STLport: 0x28 bytes), as in the landed CJoystickPositionTracker::Push 0x00A0DFC0.
// Callees: _Deque_base<C2DVector>::_M_initialize_map @ 0x00A0E310, deque<C2DVector>::resize @ 0x00A0FC80 (landed).
// Shape: STLport 4.6 `deque() : _Deque_base(allocator_type(), 0)` -- the two default-constructed iterators and the map
//   pointer/size are zero-stored from one register, then _M_initialize_map(0) (the map pointer is STLport's
//   _STLP_alloc_proxy over the empty map allocator: a plain pointer schedules the `push 0` one store early); the body is
//   `RecentPositions.resize(30, C2DVector(0.0f, 0.0f))`, whose 8-byte temporary is stored with immediates after the pushes.
class C2DVector {                                       // 8 bytes
public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}
};

namespace _STL {
template <class _Tp> class allocator {
public:
    allocator() {}
};

template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };

template <class _Tp> struct _Deque_iterator_base {      // 16 bytes
    typedef _Tp** _Map_pointer;
    _Tp*         _M_cur;                                // +0x00
    _Tp*         _M_first;                              // +0x04
    _Tp*         _M_last;                               // +0x08
    _Map_pointer _M_node;                               // +0x0C
    _Deque_iterator_base() : _M_cur(0), _M_first(0), _M_last(0), _M_node(0) {}
};

template <class _Tp, class _Traits> struct _Deque_iterator : public _Deque_iterator_base<_Tp> {
    _Deque_iterator() {}
};

template <class _Tp, class _Alloc> class _Deque_base {
public:
    typedef _Tp value_type;
    typedef _Alloc allocator_type;
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;

    _Deque_base(const allocator_type&, unsigned int __num_elements)
        : _M_start(), _M_finish(), _M_map(allocator<_Tp*>(), 0), _M_map_size(0)
    {
        _M_initialize_map(__num_elements);
    }
    void _M_initialize_map(unsigned int __num_elements);   // 0x00A0E310

    iterator     _M_start;                              // +0x00
    iterator     _M_finish;                             // +0x10
    _STLP_alloc_proxy<value_type**, value_type*, allocator<value_type*> > _M_map;   // +0x20
    unsigned int _M_map_size;                           // +0x24
};

template <class _Tp, class _Alloc = allocator<_Tp> > class deque : public _Deque_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef _Alloc allocator_type;
    typedef unsigned int size_type;

    explicit deque(const allocator_type& __a = allocator_type()) : _Deque_base<_Tp, _Alloc>(__a, 0) {}
    void resize(size_type __new_size, const value_type& __x);   // 0x00A0FC80
};
}

class CJoystickPositionTracker {
public:
    _STL::deque<C2DVector> RecentPositions;             // +0x00
    CJoystickPositionTracker();
};

CJoystickPositionTracker::CJoystickPositionTracker()
{
    RecentPositions.resize(30, C2DVector(0.0f, 0.0f));
}