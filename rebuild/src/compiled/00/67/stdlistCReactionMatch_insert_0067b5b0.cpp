// std::list<CReactionMatch*>::insert(iterator pos, CReactionMatch* const& x) -- STLport list insert: malloc a 12-byte node, placement-copy the pointer into it, link it in front of `pos` and return an iterator to it
// Leaf: ?insert@?$list@PAVCReactionMatch@@
//
// Retail 0x0067B5B0, 53 bytes (+ int3 padding to 64), thiscall `ret 0Ch` (`this` unused; the hidden iterator result,
//   the by-value iterator and the element reference on the stack); returns the result address.
// Callee: malloc @ 0x00BFEA0E (STLport's static malloc node allocator, no out-of-memory check).
// Caller: 0x0067D016 in the range insert list<CReactionMatch*>::insert(pos, first, last) 0x0067CFF0 (landed), reached
//   from list<CReactionMatch*>::operator= 0x0067CF40 for COpinionReactionManagerDef's reaction-match list (+0xA8).
// Shape: STLport 4.x _list.h `_Node* tmp = _M_create_node(x); _List_node_base* n = pos._M_node; p = n->_M_prev;
//   tmp->_M_next = n; tmp->_M_prev = p; p->_M_next = tmp; n->_M_prev = tmp; return iterator(tmp);` as the landed
//   0x005355C0 (the element built with placement new -> the null test on &tmp->_M_data; the iterator has user
//   constructors, so it comes back through the hidden pointer).  Speed TU (`add esp,4` after malloc).
// Label: catalog "List_InsertNode" (descriptive).  RTTI .?AVCReactionMatch@@ (class).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

class CReactionMatch;

namespace std {
struct _List_node_base {
    _List_node_base* _M_next;                               // +0x00
    _List_node_base* _M_prev;                               // +0x04
};

template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;                                            // +0x08
};

template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};

template <class _Tp> class list {
public:
    typedef _List_node<_Tp>     _Node;
    typedef _List_iterator<_Tp> iterator;

    _List_node_base* _M_node;                               // the sentinel node

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }

    iterator insert(iterator __position, const _Tp& __x);
};

template <class _Tp> typename list<_Tp>::iterator list<_Tp>::insert(iterator __position, const _Tp& __x)
{
    _Node* __tmp = _M_create_node(__x);
    _List_node_base* __n = __position._M_node;
    _List_node_base* __p = __n->_M_prev;
    __tmp->_M_next = __n;
    __tmp->_M_prev = __p;
    __p->_M_next = __tmp;
    __n->_M_prev = __tmp;
    return __tmp;
}

template class list<CReactionMatch*>;
}