// std::list<CStringParserCommentBoundaries>::_M_insert_dispatch(iterator pos, const_iterator first, const_iterator last, const __false_type&) -- STLport range insert: copy every element of [first, last) into a new node linked in before pos
// Leaf: ??$_M_insert_dispatch@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004638C8, 57 bytes, `ret 10h` (the three iterators by value plus the tag reference);
//         size TU without a frame pointer.
// Callees: list<CStringParserCommentBoundaries>::_M_create_node(const CStringParserCommentBoundaries&) @ 0x00463901 (thiscall:
//          malloc(0x10) through the node allocator, then the null-checked placement copy of the 8-byte element at node+8).
// Identity: the only caller is the range insert 0x004638A1 (landed as CRetailList004638a1_Insert: it passes the `stosb`
//   tag), itself called from the list copy constructor 0x00463871 inside the CStringParser copy constructor 0x0046381B
//   (vtable 0x0122D874), which copies member +0x14 = PDB CStringParser::Comments, a list<CStringParserCommentBoundaries>
//   (element = {char* PStart; char* PEnd}). The catalog label CEngineSceneGrid_InsertBoundingBoxNode is behavioural.
// Shape: STLport 4.x `for (; first != last; ++first) insert(pos, *first);` with the single-element insert inline (create the
//   node, then tmp->next = n, tmp->prev = n->prev, n->prev->next = tmp, n->prev = tmp); the position node is hoisted into edi,
//   the loop is rotated (one `first == last` test ahead of it). The size TU keeps insert(pos, x) out of line unless it is
//   __forceinline (retail has no call to it); a speed TU caches `last` in ebp instead of re-reading it.
#pragma optimize("s", on)

class CStringParserCommentBoundaries {
public:
    char* PStart;                       // +0x00
    char* PEnd;                         // +0x04
};

namespace std {

struct __false_type {};

struct _List_node_base {
    _List_node_base* _M_next;           // +0x00
    _List_node_base* _M_prev;           // +0x04
};

template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;                        // +0x08
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    _List_iterator_base(const _List_iterator_base& __x) : _M_node(__x._M_node) {}
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp, class _Ref> struct _List_iterator : _List_iterator_base {
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) {}
    _List_iterator(const _List_iterator_base& __x) : _List_iterator_base(__x) {}
    _Ref operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_incr(); return *this; }
};

template <class _Tp> class allocator {};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list {
public:
    typedef _List_iterator<_Tp, _Tp&>       iterator;
    typedef _List_iterator<_Tp, const _Tp&> const_iterator;

    _List_node_base* _M_node;           // +0x00 the sentinel

    _List_node<_Tp>* _M_create_node(const _Tp& __x);   // 0x00463901

    __forceinline iterator insert(iterator __position, const _Tp& __x)
    {
        _List_node<_Tp>* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }

    template <class _InputIter>
    void _M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last, const __false_type&)
    {
        for ( ; __first != __last; ++__first)
            insert(__pos, *__first);
    }
};

} // namespace std

template void std::list<CStringParserCommentBoundaries>::_M_insert_dispatch<std::list<CStringParserCommentBoundaries>::const_iterator>(
    std::list<CStringParserCommentBoundaries>::iterator, std::list<CStringParserCommentBoundaries>::const_iterator,
    std::list<CStringParserCommentBoundaries>::const_iterator, const std::__false_type&);