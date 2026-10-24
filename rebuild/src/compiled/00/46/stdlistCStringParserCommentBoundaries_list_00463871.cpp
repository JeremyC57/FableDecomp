// std::list<CStringParserCommentBoundaries>::list(const list& x) -- STLport copy constructor: allocate the sentinel through the base, then range-insert x's elements at begin()
// Leaf: ??0?$list@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00463871, 48 bytes, `ret 4`; y-off + size TU (frame pointer, `leave`), like the
//         landed range insert it calls.
// Callees: _List_base<CStringParserCommentBoundaries>::_List_base(const allocator_type&) @ 0x00404690 (allocates and self-links
//          the sentinel; catalogued as the propagated std_list_node_Init), list::insert<const_iterator>(iterator, const_iterator,
//          const_iterator) @ 0x004638A1 (landed as CRetailList004638a1_Insert, `ret 0Ch`).
// Identity: the only caller is the CStringParser copy constructor 0x0046381B (vtable 0x0122D874), copying member +0x14 = PDB
//   CStringParser::Comments, a list<CStringParserCommentBoundaries>. The catalog label CEngineSceneGrid_InitFromCell is wrong.
// Shape: STLport 4.x `list(const list& x) : _List_base(x.get_allocator()) { insert(begin(), x.begin(), x.end()); }`; the
//   allocator temporary is the one-byte frame slot [ebp-1]; x.end() / x.begin() are const_iterators whose copy is trivial
//   (pushed directly), begin() is an iterator whose copy constructor is user-declared (STLport `_List_iterator(const
//   iterator&)`), so it is built in place in the reserved argument slot (`push ecx ; mov eax,esp ; mov [eax],ecx`). The
//   allocator carries STLport's user-declared constructors: an aggregate `allocator_type()` is value-initialised (`stosb`).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CStringParserCommentBoundaries {
public:
    char* PStart;                       // +0x00
    char* PEnd;                         // +0x04
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() {}
    allocator(const allocator<_Tp>&) {}
};

struct _List_node_base {
    _List_node_base* _M_next;           // +0x00
    _List_node_base* _M_prev;           // +0x04
};

template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;                        // +0x08
};

template <class _Tp> struct _Nonconst_traits;
template <class _Tp> struct _Const_traits {
    typedef const _Tp& reference;
    typedef _Nonconst_traits<_Tp> _Non_const_traits;
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp& reference;
    typedef _Nonconst_traits<_Tp> _Non_const_traits;
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
};

template <class _Tp, class _Traits> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<_Tp, typename _Traits::_Non_const_traits> iterator;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) {}
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _Alloc allocator_type;
    _List_node_base* _M_node;           // +0x00 the sentinel (_STLP_alloc_proxy over the node pointer)
    _List_base(const allocator_type& __a);   // 0x00404690
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_base<_Tp, _Alloc> _Base;
    typedef _Alloc allocator_type;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> >    const_iterator;

    allocator_type get_allocator() const { return allocator_type(); }
    iterator begin() { return iterator(this->_M_node->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node); }

    template <class _InputIterator>
    void insert(iterator __pos, _InputIterator __first, _InputIterator __last);   // 0x004638A1 for const_iterator

    list(const list<_Tp, _Alloc>& __x) : _List_base<_Tp, _Alloc>(__x.get_allocator())
    {
        insert(begin(), __x.begin(), __x.end());
    }
};

} // namespace std

template std::list<CStringParserCommentBoundaries>::list(const std::list<CStringParserCommentBoundaries>& __x);