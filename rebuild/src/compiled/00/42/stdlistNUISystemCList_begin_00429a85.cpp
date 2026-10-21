// std::list<NUISystem::CList*>::begin() -- returns an iterator on the first node (the sentinel's next pointer)
// Retail: Fable.exe @ 0x00429A85, 13 bytes, `ret 4` (hidden result pointer: the STLport list iterator has user
// constructors). Retail STLport lists are 4 bytes (the sentinel node pointer) with the node's next pointer first.
// Many callers in the frontend list code 0x00424CA0.. (the 13-byte body is the same under /O2 and /Os).
namespace NUISystem { class CList; }
namespace std {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
template <class T> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* x) : _M_node(x) {}
};
template <class T, class A> class list {
public:
    _List_node_base* _M_node;
    typedef _List_iterator<T> iterator;
    iterator begin() { return iterator(_M_node->_M_next); }
};
template <class T> class allocator {};
template class list<NUISystem::CList*, allocator<NUISystem::CList*> >;
}