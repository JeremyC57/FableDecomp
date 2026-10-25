// std::back_insert_iterator<deque<CCrimeWaitingForBribe> >::operator=(const CCrimeWaitingForBribe& value) -- append one bribe record through the iterator's deque (STLport push_back inlined: construct at the finish cursor while the node has room, else _M_push_back_aux_v) and return the iterator
// Leaf: ??4?$back_insert_iterator@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B5270, 62 bytes (+2 int3), thiscall `ret 4`, returns this.
// Callees: deque<CCrimeWaitingForBribe>::_M_push_back_aux_v(const value_type&) 0x007B4F70.
// Identity: no direct callers (an emitted COMDAT of the CTCGuard TU, whose load path copies the transferred
//   vector<CCrimeWaitingForBribe> back into the guard's deque PendingCrimes through back_inserter: __copy
//   0x007B5400). Catalog label "std_pair_PushBack" is descriptive.
// Shape: STLport `container->push_back(value); return *this;` with deque::push_back = `if (_M_finish._M_cur !=
//   _M_finish._M_last - 1) { _Construct(_M_finish._M_cur, t); ++_M_finish._M_cur; } else _M_push_back_aux_v(t);`
//   (finish iterator at +0x10: cur +0x10, last +0x18); the placement-new null test and the 8-byte copy through edi/edx.
//   Retail STLport deque layout: _M_start +0, _M_finish +0x10, _M_map +0x20, _M_map_size +0x24.
inline void* operator new(unsigned int, void* p) { return p; }

class CCrimeWaitingForBribe {                               // 8 bytes
public:
    long DeedHandle;                                        // +0x00
    long Type;                                              // +0x04 EOpinionDeedType
};

namespace std {
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class T> struct _Deque_iterator {
    T*  _M_cur;                                             // +0x00
    T*  _M_first;                                           // +0x04
    T*  _M_last;                                            // +0x08
    T** _M_node;                                            // +0x0c
};

template <class T> class deque {
public:
    typedef T value_type;
    _Deque_iterator<T> _M_start;                            // +0x00
    _Deque_iterator<T> _M_finish;                           // +0x10
    T**                _M_map;                              // +0x20
    unsigned int       _M_map_size;                         // +0x24
    void _M_push_back_aux_v(const value_type& __t);         // 0x007B4F70
    void push_back(const value_type& __t)
    {
        if (this->_M_finish._M_cur != this->_M_finish._M_last - 1) {
            _Construct(this->_M_finish._M_cur, __t);
            ++this->_M_finish._M_cur;
        }
        else
            _M_push_back_aux_v(__t);
    }
};

template <class _Container> class back_insert_iterator {
protected:
    _Container* container;
public:
    explicit back_insert_iterator(_Container& __x) : container(&__x) { }
    back_insert_iterator<_Container>& operator=(const typename _Container::value_type& __value)
    {
        container->push_back(__value);
        return *this;
    }
};

template class back_insert_iterator<deque<CCrimeWaitingForBribe> >;
}