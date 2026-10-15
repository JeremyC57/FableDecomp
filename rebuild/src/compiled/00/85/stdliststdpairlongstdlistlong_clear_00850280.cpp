// std::list<std::pair<long, std::list<long> > >::clear() -- STLport clear of a list whose entries each hold a key and an inner list: free every inner node and sentinel, then every outer node, and reset the sentinel
// Leaf: ?clear@?$list@U?$pair@JV?$list@J
// Retail: Fable.exe (TLC, VC7.1) @ 0x00850280, 111 bytes (+ int3 padding).
// Label correction: the catalog name CNavigatorQueueManager::FrameUpdate is propagated. Its callers 0x008502F0 / 0x00850310
//   (`clear(); if (sentinel) free(sentinel);` = the list destructor) and the destructors at 0x00850330 / 0x00850360 call it on an
//   embedded list; the nodes are next/prev + an entry whose second word is an inner STLport list (sentinel pointer).
//   The entry types are not pinned by the bytes (key and inner element are trivially destructible words).
// Callees: free @ 0x00BFEA14 (STLport malloc allocator) for every node and the inner sentinel.
// Shape: STLport clear -- the cursor advances BEFORE the node is destroyed; the inner list's destructor (its own clear, the
//   sentinel reset re-reading the member, the null-tested sentinel free) is inlined into the entry's implicit destructor.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
namespace std {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
template <class T> struct list {                         // STLport list over the malloc allocator: the sentinel pointer only
    _List_node_base* _M_node;
    void clear()
    {
        _List_node<T>* cur = (_List_node<T>*)_M_node->_M_next;
        while (cur != _M_node) {
            _List_node<T>* tmp = cur;
            cur = (_List_node<T>*)cur->_M_next;
            tmp->_M_data.~T();
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    ~list()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template struct list<pair<long, list<long> > >;
}