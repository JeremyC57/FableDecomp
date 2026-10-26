// std::list<COpinionTransientOffset>::_M_insert_dispatch(iterator position, const_iterator first, const_iterator last, const __false_type&) -- STLport range insert: build a node copy of every source element and link it in before position
// Leaf: _M_insert_dispatch
// Retail: Fable.exe (TLC, VC7.1) @ 0x00471C40, 57 bytes, `ret 10h` (thiscall: position, first, last by value and the
//   __false_type tag by reference).
// Identity: the catalog's "LinkedList_InsertScriptData" is behavioural. The single caller is list::insert(position, first,
//   last) @ 0x00471C19 (builds the tag with `stosb`), called from list::operator= @ 0x00471B73, which the member-wise
//   COpinionDeedEffectsDef::operator= @ 0x00471A77 (reached from COpinionDeedEffectsDef::Copy @ 0x00471A72) runs for the
//   list at +0x4c: Ego_r.pdb COpinionDeedEffectsDef::Effects is list<COpinionTransientOffset> (24-byte element, 32-byte
//   node {next, prev, data}).
// Callees: list::_M_create_node(const COpinionTransientOffset&) @ 0x00471C79 (landed as "Alloc"; out of line).
// Model: STLport 4.x `_M_insert_dispatch(.., const __false_type&)`: `for (; first != last; ++first) insert(position, *first)`
//   with the single-element insert inlined (create the node, then tmp->next = n, tmp->prev = p, p->next = tmp,
//   n->prev = tmp). COpinionTransientOffset is local (no engine header).
// Shape: size TU (#pragma optimize("s", on)), no frame pointer.
#pragma optimize("s", on)

class COpinionTransientOffset { public:                    // PDB 24 bytes
    long  Opinion;                                  // +0x00 EOpinion
    float OffsetPerFrameRunIn;                      // +0x04
    long  FramesToRunIn;                            // +0x08
    long  FramesOfCappedPeak;                       // +0x0C
    float OffsetPerFrameRunOut;                     // +0x10
    long  FramesToRunOut;                           // +0x14
};

namespace std {
struct __false_type { };

struct _List_node_base {
    _List_node_base* _M_next;                       // +0x00
    _List_node_base* _M_prev;                       // +0x04
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;                                      // +0x08
};

template <class T> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* x) : _M_node(x) { }
    _List_iterator(const _List_iterator& x) : _M_node(x._M_node) { }
    T& operator*() const { return static_cast<_List_node<T>*>(_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& y) const { return _M_node != y._M_node; }
};

template <class T> class list {
public:
    typedef _List_iterator<T> iterator;
    typedef _List_iterator<T> const_iterator;
    _List_node_base* _M_node;                       // +0x00 the header node
    _List_node<T>* _M_create_node(const T& x);     // 0x00471C79
    __forceinline iterator insert(iterator position, const T& x)
    {
        _List_node<T>* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    template <class InputIter>
    void _M_insert_dispatch(iterator position, InputIter first, InputIter last, const __false_type&)
    {
        for (; first != last; ++first)
            insert(position, *first);
    }
};

template void list<COpinionTransientOffset>::_M_insert_dispatch(iterator position, const_iterator first, const_iterator last,
                                                                const __false_type&);
}