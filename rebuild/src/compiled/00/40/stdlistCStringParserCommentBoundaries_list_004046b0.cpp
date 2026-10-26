// std::list<CStringParserCommentBoundaries>::~list() -- STLport list destructor: free every node, relink the sentinel to itself, then free the sentinel
// Leaf: ??1?$list@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004046B0, 62 bytes, thiscall, plain `ret`.
// Callees: free 0x00BFEA14 (STLport node allocator over the CRT heap).
// Shape: STLport 4.6 `~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }` with `clear()` inlined:
//   `tmp = cur; cur = cur->_M_next; _Destroy(&tmp->_M_data); free(tmp)` -- the loop variable advances BEFORE the
//   node is released, so it lives in esi and the sentinel is re-read from `this` (edi) after every call; the
//   element is a trivially destructible POD (the comment start/end offsets), so nothing is destroyed. The node
//   free in the loop has no null test; the sentinel free does (`if (p) free(p)`). The retail list is one pointer
//   (the sentinel). The body is shared by ~90 byte-identical list destructors (COMDAT folding), so the element
//   type comes from the catalog label (CStringParser::Comments is such a list).
extern "C" void __cdecl free(void*);                                            // 0x00BFEA14

class CStringParserCommentBoundaries { public:
    long Start;                                         // +0x00 comment start offset
    long End;                                           // +0x04 comment end offset
};

namespace std {

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp> class list {
public:
    typedef _List_node<_Tp> _Node;
    _List_node_base* _M_node;                           // +0x00 the sentinel node

    void clear()
    {
        _Node* __cur = (_Node*)_M_node->_M_next;
        while (__cur != _M_node) {
            _Node* __tmp = __cur;
            __cur = (_Node*)__cur->_M_next;
            free(__tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    ~list();
};

template <class _Tp> list<_Tp>::~list()
{
    clear();
    if (_M_node)
        free(_M_node);
}

template list<CStringParserCommentBoundaries>::~list();

}