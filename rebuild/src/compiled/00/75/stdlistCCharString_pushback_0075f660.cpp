// std::list<CCharString>::push_back(const CCharString& x) -- STLport list push_back: insert(end(), x) -- malloc a 0x0C-byte node, copy the string into it and link it before the sentinel
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075F660, 51 bytes, thiscall `ret 4`.
// Caller: CTCCameraPointScriptedSpline::UpdateCameraParams 0x0075D580 (call site 0x0075D84B).
// Callees: malloc 0x00BFEA0E, CCharString copy constructor 0x0099EC30.
// Shape: the landed STLport list<CClanMember>::push_back 0x00567FC0 with a one-word payload: end() (the sentinel,
//   `mov edi,[ecx]`) is read before the node is created, placement new null-tests &node->_M_data, then the four
//   link stores (tmp->next, tmp->prev, prev->next, sentinel->prev). The catalog's "DoublyLinkedList_InsertBack" is
//   a descriptive auto label.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* PStringData;                                  // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
};

namespace std {

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
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

    _List_node_base* _M_node;                           // the sentinel (retail list = one pointer)

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }
    iterator end() { return iterator(_M_node); }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    void push_back(const _Tp& __x);
};

template <class _Tp>
void list<_Tp>::push_back(const _Tp& __x)
{
    insert(end(), __x);
}

template void list<CCharString>::push_back(const CCharString&);

}