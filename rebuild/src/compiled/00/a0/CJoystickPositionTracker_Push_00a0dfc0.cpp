// CJoystickPositionTracker::Push(const CInputEvent& event) -- record the event's normalised stick direction at the front of RecentPositions and trim the history to its 30 newest entries
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0DFC0, length 215 bytes (thiscall `ret 4`, + int3 padding). Caller:
// CInputProcessCombat::ProcessInput 0x0068FB90 (landed; it calls this "Push"). Method name descriptive (Ego_r.pdb names only the
// tracker's constructor, destructor, HasPerformed360ClockwiseMove and HadPerformedMaximumDamageMove).
// Callees: CInputEvent::GetJoystickPosition @ 0x00A043A0 (hidden result into the local), C2DVector::Normalise @ 0x00A14540,
// deque::_M_push_front_aux_v @ 0x00A0F3B0, free @ 0x00BFEA14.
// Layout: CJoystickPositionTracker (Ego_r.pdb) -- RecentPositions, a deque<C2DVector>, at +0 (the TLC STLport deque is 0x28 bytes).
// Shape: `C2DVector position = event.GetJoystickPosition(); position.Normalise(); RecentPositions.push_front(position);
// while (RecentPositions.size() > 30) RecentPositions.pop_back();` with push_front, size and pop_back (and its
// _M_pop_back_aux) inlined; the loop rotated with the size test at the bottom (`ja`).
// Leaf: ?Push@CJoystickPositionTracker
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
    C2DVector& Normalise();                                 // 0x00A14540
};

namespace _STL {
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

template <class _Tp, class _Alloc = allocator<_Tp> > class deque {
public:
    typedef _Tp value_type;
    typedef unsigned int size_type;
    typedef int difference_type;
    typedef _Tp** _Map_pointer;
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _Deque_iterator<_Tp, _Const_traits<_Tp> > const_iterator;

    iterator     _M_start;                                  // +0x00
    iterator     _M_finish;                                 // +0x10
    _Map_pointer _M_map;                                    // +0x20
    size_type    _M_map_size;                               // +0x24

    static size_type buffer_size() { return (size_type)_Deque_iterator_base<_Tp>::__buffer_size; }
    size_type size() const { return this->_M_finish - this->_M_start; }

    void push_front(const value_type& __t)
    {
        if (this->_M_start._M_cur != this->_M_start._M_first) {
            _Construct(this->_M_start._M_cur - 1, __t);
            --this->_M_start._M_cur;
        }
        else
            _M_push_front_aux_v(__t);
    }
    void pop_back()
    {
        if (this->_M_finish._M_cur != this->_M_finish._M_first) {
            --this->_M_finish._M_cur;
            _Destroy(this->_M_finish._M_cur);
        }
        else
            _M_pop_back_aux();
    }
    iterator erase(iterator __first, iterator __last);                                   // 0x00A0F040
    void insert(iterator __pos, size_type __n, const value_type& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const value_type& __x);
    void clear();

    void _M_fill_insert(iterator __pos, size_type __n, const value_type& __x);          // 0x00A0FAA0
    void _M_push_front_aux_v(const value_type& __t);                                     // 0x00A0F3B0
    void _M_pop_back_aux();
    void _M_reallocate_map(size_type __nodes_to_add, bool __add_at_front);              // 0x00A0EE50
    void _M_reserve_map_at_back(size_type __nodes_to_add = 1)
    {
        if (__nodes_to_add + 1 > this->_M_map_size - (this->_M_finish._M_node - this->_M_map))
            _M_reallocate_map(__nodes_to_add, false);
    }
    void _M_reserve_map_at_front(size_type __nodes_to_add = 1)
    {
        if (__nodes_to_add > size_type(this->_M_start._M_node - this->_M_map))
            _M_reallocate_map(__nodes_to_add, true);
    }
    iterator _M_reserve_elements_at_front(size_type __n);
    void _M_new_elements_at_front(size_type __new_elements);                            // 0x00A0F300
    void _M_new_elements_at_back(size_type __new_elements);                             // 0x00A0F350
};

template <class _Tp, class _Alloc> void deque<_Tp, _Alloc>::_M_pop_back_aux()
{
    _Alloc().deallocate(this->_M_finish._M_first, this->buffer_size());
    this->_M_finish._M_set_node(this->_M_finish._M_node - 1);
    this->_M_finish._M_cur = this->_M_finish._M_last - 1;
    _Destroy(this->_M_finish._M_cur);
}

}

class CInputEvent {
public:
    C2DVector GetJoystickPosition() const;                  // 0x00A043A0
};

class CJoystickPositionTracker {
public:
    _STL::deque<C2DVector> RecentPositions;                 // +0x00
    void Push(const CInputEvent& event);
};

void CJoystickPositionTracker::Push(const CInputEvent& event)
{
    C2DVector position = event.GetJoystickPosition();
    position.Normalise();
    RecentPositions.push_front(position);
    while (RecentPositions.size() > 30)
        RecentPositions.pop_back();
}