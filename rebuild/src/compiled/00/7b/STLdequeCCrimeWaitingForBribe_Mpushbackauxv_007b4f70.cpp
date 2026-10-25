// std::deque<CCrimeWaitingForBribe>::_M_push_back_aux_v(const CCrimeWaitingForBribe& t) -- STLport deque push_back slow path: copy the value, make room for one more node at the back of the map, allocate the new 128-byte node buffer, construct the value at the old finish cursor and step the finish iterator onto the new node
// Leaf: ?_M_push_back_aux_v@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B4F70, 111 bytes (+1 int3), thiscall `ret 4`.
// Callees: deque::_M_reallocate_map(size_type nodes_to_add, bool add_at_front) 0x007B4980, malloc 0x00BFEA0E.
// Identity: called from the inline deque::push_back of back_insert_iterator<deque<CCrimeWaitingForBribe> >::operator=
//   0x007B5270 and __copy 0x007B5400 (and a tail jump at 0x007B5108); the deque is CTCGuard::PendingCrimes.
//   Catalog label "StdVector_PushBack" is descriptive.
// Shape: STLport 4.5 `value_type __t_copy = __t; _M_reserve_map_at_back(); *(_M_finish._M_node + 1) =
//   _M_map_size.allocate(buffer_size()); _Construct(_M_finish._M_cur, __t_copy); _M_finish._M_set_node(_M_finish._M_node
//   + 1); _M_finish._M_cur = _M_finish._M_first;` (no EH: the TU is built without exceptions). The copy lives in
//   edi/ebx from the entry; `_M_reserve_map_at_back(1)` is `if (2 > _M_map_size - (_M_finish._M_node - _M_map))
//   _M_reallocate_map(1, false)`; allocate(0x80 / 8 = 16) folds to `malloc(0x80)`.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class CCrimeWaitingForBribe {                               // 8 bytes
public:
    long DeedHandle;                                        // +0x00
    long Type;                                              // +0x04 EOpinionDeedType
};

namespace std {
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class T> struct _Deque_iterator {
    T*  _M_cur;                                             // +0x00
    T*  _M_first;                                           // +0x04
    T*  _M_last;                                            // +0x08
    T** _M_node;                                            // +0x0c
    static unsigned int _S_buffer_size() { return 128 / sizeof(T); }
    void _M_set_node(T** __new_node)
    {
        _M_node = __new_node;
        _M_first = *__new_node;
        _M_last = _M_first + _S_buffer_size();
    }
};

template <class T> class deque {
public:
    typedef T value_type;
    typedef unsigned int size_type;
    _Deque_iterator<T> _M_start;                            // +0x00
    _Deque_iterator<T> _M_finish;                           // +0x10
    T**                _M_map;                              // +0x20
    size_type          _M_map_size;                         // +0x24
    static size_type buffer_size() { return 128 / sizeof(T); }
    void _M_reallocate_map(size_type __nodes_to_add, bool __add_at_front);   // 0x007B4980
    void _M_reserve_map_at_back(size_type __nodes_to_add = 1)
    {
        if (__nodes_to_add + 1 > this->_M_map_size - (this->_M_finish._M_node - this->_M_map))
            _M_reallocate_map(__nodes_to_add, false);
    }
    void _M_push_back_aux_v(const value_type& __t);
};

template <class T> void deque<T>::_M_push_back_aux_v(const value_type& __t)
{
    value_type __t_copy = __t;
    _M_reserve_map_at_back();
    *(this->_M_finish._M_node + 1) = allocator<T>().allocate(buffer_size());
    _Construct(this->_M_finish._M_cur, __t_copy);
    this->_M_finish._M_set_node(this->_M_finish._M_node + 1);
    this->_M_finish._M_cur = this->_M_finish._M_first;
}

template class deque<CCrimeWaitingForBribe>;
}