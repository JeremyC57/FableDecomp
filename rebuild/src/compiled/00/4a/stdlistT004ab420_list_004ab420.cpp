// std::list<T_004ab420>::~list() -- STLport list destructor for a trivially destructible element: free every node,
// relink the sentinel to itself (the inlined clear()), then free the sentinel.
// Leaf: ??1?$list@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AB420, 62 bytes, thiscall, plain `ret`.
// Renamed (rename1, 2026-10-24) from _global::StdList_Clear_004ab420 (symbol SortTreeRecursively_00436900) @
//   00/4a/CGuiControlTreePane_SortTreeRecursively_004ab420.cpp: the body is not a clear -- after the node loop and
//   the sentinel self-link it frees the sentinel itself (`mov edi,[edi]; test edi,edi; je; push edi; call free`),
//   i.e. ~_List_base() = clear() + deallocate(_M_node). The manifest/catalog CGuiControlTreePane::
//   SortTreeRecursively label is propagated (no tree, no sort). Callers: the destructor helper 0x004AC800 and
//   COpinionReactionManagerDef::Transfer 0x0067BFF2/0x0067C012, plus the tail jumps 0x004AC000 / 0x0067BC90. The
//   element type is not recoverable from the bytes (no per-element destructor call, so the node size never
//   appears); the name carries the address.
// Layout: STLport list -- one sentinel-node pointer; node {_M_next +0, _M_prev +4, _M_data +8}.
// Shape: /O2 (`lea esp,[esp]` loop-head pad; the sentinel pointer is re-read from `this` after every free).
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14

struct T_004ab420 { long _value; };                     // element type not recoverable (trivially destructible)

namespace std {
struct _List_node_base {
    _List_node_base* _M_next;                           // +0x00
    _List_node_base* _M_prev;                           // +0x04
};
template <class T> struct _List_node : public _List_node_base {
    T _M_data;                                          // +0x08
};
template <class T> class list {
public:
    _List_node<T>* _M_node;                             // sentinel
    void clear()
    {
        _List_node<T>* cur = (_List_node<T>*)_M_node->_M_next;
        while (cur != _M_node) {
            _List_node<T>* tmp = cur;
            cur = (_List_node<T>*)cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    ~list()
    {
        clear();
        if (_M_node != 0)
            free(_M_node);
    }
};
}
template class std::list<T_004ab420>;
