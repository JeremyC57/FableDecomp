// std::list<CReactionMatch*>::push_back(CReactionMatch* const& x) -- STLport list append: insert(end(), x) inline -- malloc a 12-byte node, placement-copy the pointer, link it in front of the sentinel
// Leaf: ?push_back@?$list@PAVCReactionMatch@@
//
// Retail 0x0067BCC0, 45 bytes, thiscall `ret 4`.
// Callee: malloc @ 0x00BFEA0E (STLport's static malloc node allocator, no out-of-memory check).
// Callers: 0x00678B26 / 0x00678E2F / 0x006794D9 in the reaction-match parser (0x00678E2F follows the `new`
//   CReactionMatch_MoralityRenownBlob 0x00677760): each parsed match is appended to the list<CReactionMatch*> whose
//   range insert 0x0067CFF0 and element insert 0x0067B5B0 are its siblings.
// Shape: STLport 4.x _list.h `void push_back(const _Tp& x) { insert(end(), x); }` with insert inline: end() reads the
//   sentinel (`mov esi,[ecx]`) before the node allocation, then the placement copy (null test on &tmp->_M_data) and the
//   four link stores.  Speed TU.
// Label: catalog "LinkedList_InsertNode" (descriptive).  RTTI .?AVCReactionMatch@@ (class).
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

    iterator end() { return _M_node; }
    iterator insert(iterator __position, const _Tp& __x)
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
    void push_back(const _Tp& __x) { insert(end(), __x); }
};

template void list<CReactionMatch*>::push_back(CReactionMatch* const&);
}