// std::list<CReactionMatch_*>::insert(iterator position, const_iterator first, const_iterator last) -- STLport range insert: insert each pointer of [first, last) before position
// Leaf: ?insert@?$list@PAUCReactionMatch_@@@std@@QAEXU
// Retail: Fable.exe @ 0x0067CFF0, 54 bytes, `__fastcall`, `ret 0Ch`. Label correction: the manifest's "ForEachIf<list<CGuiWindow*>
// iterator, ...>" is propagated (byte twin of 0x0062F650). Its only caller is list<CReactionMatch_*>::operator= 0x0067CF40
// (used by the landed COpinionReactionManagerDef::operator= 0x0067CE50 for its Matches list at +0xA8), which inserts the
// source's remaining tail; the single-element insert 0x0067B5B0 allocates a 12-byte node.
// Layout / shape: as 0x0062F650 (bare head pointer list, node next +0 / prev +4 / value +8; the by-value iterator with a
// user copy constructor built in its argument slot; the result temporary homed on the dead `first` slot).
struct CReactionMatch_;
namespace std {
template <class T> struct _List_node {
    _List_node* _M_next;                    // +0x00
    _List_node* _M_prev;                    // +0x04
    T           _M_data;                    // +0x08
};
template <class T> struct _List_iterator {  // STLport: user copy ctor -> by-value copies are built in their argument slots
    _List_node<T>* _M_node;
    _List_iterator(_List_node<T>* node) : _M_node(node) { }
    _List_iterator(const _List_iterator& other) : _M_node(other._M_node) { }
    const T& operator*() const { return _M_node->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& other) const { return _M_node != other._M_node; }
};
template <class T> class list {             // retail: the bare head-node pointer
public:
    typedef _List_iterator<T> iterator;
    typedef _List_iterator<T> const_iterator;
    _List_node<T>* _M_node;
    iterator insert(iterator position, const T& x);
    void insert(iterator position, const_iterator first, const_iterator last);
};
template <class T> void list<T>::insert(iterator position, const_iterator first, const_iterator last)
{
    for (; first != last; ++first)
        insert(position, *first);
}
template class list<CReactionMatch_*>;
}