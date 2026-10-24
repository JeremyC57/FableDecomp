// std::list<CClass*>::_Insert(iterator where, const_iterator first, const_iterator last, std::input_iterator_tag) -- inserts a copy of every element of [first, last) before where, one node at a time
// Leaf: _Insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053CAA0, 48 bytes, `ret 10h`. Caller: 0x0053CB17. Callee: list<CClass*>::insert(iterator, const T&) @
// 0x0053CA7B (landed as std_list_node_insert). 0x005794C3 is the same body for another list (insert @ 0x0042AB33).
// Shape: `for (; first != last; ++first) insert(where, *first);` -- `where` is copied into its reserved argument slot
// (`push ecx ; mov eax,esp ; mov [eax],ecx`: user copy ctor), the discarded returned iterator reuses the dead `first` argument slot
// (`lea eax,[ebp+0Ch]`), the by-value input_iterator_tag is the unused 4th slot. Size TU (`#pragma optimize("s", on)`).
#pragma optimize("s", on)
class CClass;
namespace std {
struct input_iterator_tag {};
}
template <class T> struct SListNode {             // VC6-era Dinkumware list node
    SListNode* next;                              // +0x00
    SListNode* prev;                              // +0x04
    T value;                                      // +0x08
};
template <class T> class CListIterator {          // list<T>::iterator / const_iterator: one node pointer with a user copy ctor
public:
    SListNode<T>* m_node;
    CListIterator(SListNode<T>* node) : m_node(node) {}
    CListIterator(const CListIterator& o) : m_node(o.m_node) {}
    T& operator*() const { return m_node->value; }
    CListIterator& operator++() { m_node = m_node->next; return *this; }
    bool operator!=(const CListIterator& o) const { return m_node != o.m_node; }
};
template <class T> class CList {
public:
    typedef CListIterator<T> iterator;
    unsigned char m_allocator[4];
    SListNode<T>* m_head;
    unsigned long m_size;
    iterator insert(iterator where, const T& value);   // 0x0053CA7B (out of line)
    template <class It> void _Insert(iterator where, It first, It last, std::input_iterator_tag)
    {
        for ( ; first != last; ++first)
            insert(where, *first);
    }
};
template void CList<CClass*>::_Insert(CListIterator<CClass*>, CListIterator<CClass*>, CListIterator<CClass*>, std::input_iterator_tag);