// std::list<CCountedPointer<C3DSkeletalMorphParameter> >::insert(iterator position, const_iterator first, const_iterator last) -- STLport range insert: insert a copy of each element of [first, last) before position
// Leaf: ?insert@?$list@U?$CCountedPointer@VC3DSkeletalMorphParameter@@@@@std@@QAEXU
// Retail: Fable.exe @ 0x0062F650, 54 bytes, `__fastcall`, `ret 0Ch`. Label correction: the manifest's "ForEachIf<list<CGuiWindow*>
// iterator, unary_negate<...>, binder2nd<...> >" is propagated across byte-identical bodies (this one and 0x0067CFF0).
// Its only caller is the list copy constructor at 0x0062F610 (`list(const list& x)`: new head node, then insert(begin(),
// x.begin(), x.end())); the single-element insert 0x0062F690 allocates a 16-byte node and copies the 8-byte
// CCountedPointer<C3DSkeletalMorphParameter> through _Cons_val 0x0062F6D0 (the manifest label there names the element type).
// Layout: retail list = the bare head-node pointer; node next +0, prev +4, value +8.
// Shape: STLport's `for (; first != last; ++first) insert(position, *first);` -- the by-value position iterator has a user
// copy constructor, so each call copies it into its argument slot (`push ecx ; mov eax,esp ; mov [eax],ebp`), and the
// returned iterator temporary is homed on the dead `first` argument slot.
class C3DSkeletalMorphParameter;
template <class T> struct CCountedPointer {  // 8 bytes: object + shared reference count
    T*    Data;
    long* RefCount;
    CCountedPointer(const CCountedPointer& other);
};
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
template class list<CCountedPointer<C3DSkeletalMorphParameter> >;
}