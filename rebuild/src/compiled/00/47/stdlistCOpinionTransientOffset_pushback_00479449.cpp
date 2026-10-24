// std::list<COpinionTransientOffset>::push_back(const COpinionTransientOffset& x) -- STLport list append: build the node, link it in front of the sentinel
// Retail: Fable.exe (TLC, VC7.1) @ 0x00479449, 29 bytes, `ret 4`.
// Callees: _M_create_node @ 0x00471C79 (malloc(0x20) + placement copy of the 0x18-byte value at node+8;
//          landed as global_Alloc_00471c79, it never reads `this`).
// Shape: STLport 4.x `push_back(x) { insert(end(), x); }` with `insert` inlined: end() (the sentinel word at
// this+0) is read into esi BEFORE the node is created because it is the by-value position argument; then
// `prev = pos->_M_prev` into a local, next/prev of the new node, prev->next, pos->prev (STLport store order).
// The manifest label "LinkedList_Insert_CDanyalTestScriptsData" is propagated; the body is shared by every
// list whose element copy goes through 0x00471C79 (four call sites: 0x004793EA, 0x00682198, 0x006821CF,
// 0x0068344F). The element is named after the 0x004793EA caller (COpinionTransientOffset::TransferBinaryIn).
// Pragma: optimize("s") (the pass-through `push dword ptr [esp+8]`); the size TU only inlines insert/end when
// they are `__forceinline`.
// Leaf: push_back
#pragma optimize("s", on)

struct COpinionTransientOffset {
    unsigned char _data[0x18];          // opaque 24-byte record (only copied by the out-of-line node builder)
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
    _List_iterator(_List_node_base* x) : _M_node(x) { }
    _List_iterator(const _List_iterator& x) : _M_node(x._M_node) { }
};

template <class _Tp> class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;

    _List_node_base* _M_node;           // +0x00 sentinel

    _Node* _M_create_node(const _Tp& x);                   // 0x00471C79
    __forceinline iterator end() { return iterator(_M_node); }
    __forceinline iterator insert(iterator position, const _Tp& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return iterator(tmp);
    }
    void push_back(const _Tp& x);
};

template <class _Tp> void list<_Tp>::push_back(const _Tp& x)
{
    insert(end(), x);
}

template void list<COpinionTransientOffset>::push_back(const COpinionTransientOffset&);

}