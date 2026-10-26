// std::list::list(const list& x) -- (list<C3DVector>) STLport list copy constructor: allocate and self-link the sentinel, then range-insert x's elements at begin()
// Leaf: ??0?$list@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FC7A0, 63 bytes (+ int3 padding), thiscall `ret 4`; node size 0x14.
// Identity (proven): list<C3DVector>; evidence: its _M_insert_dispatch 0x008FBF10 is the landed list<C3DVector> range insert (0x14-byte nodes; the NCameraModes::CPhysicsForceModelBase TU); no direct caller (an unreferenced COMDAT).
// Label: the manifest's "LinkedList_CloneInitialize" is descriptive (family H007: seven rows that differ only in the node size and the call
//   displacement; retail has no identical-code folding, so each address is one instantiation).
// Callees: malloc @ 0x00BFEA0E (the STLport node allocator), this list's _M_insert_dispatch<const_iterator>(iterator,
//   const_iterator, const_iterator, const __false_type&) @ 0x008FBF10 (out of line, `ret 10h`).
// Shape: STLport 4.6 `list(const list& x) : _List_base(x.get_allocator()) { insert(begin(), x.begin(), x.end()); }` with
//   _List_base's constructor and the member-template insert inline: insert's `_Integral()` tag (a __false_type with a user
//   constructor, so nothing is stored) is homed in x's dead argument slot; begin() is built in place in its argument slot
//   (the iterator's user `_List_iterator(const iterator&)`), the const_iterators are pushed directly (the landed
//   0x0062F610 skeleton).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
#pragma pack(push, 1)
class C3DVector {                                   // retail: byte-packed, 12 bytes
public:
    float X, Y, Z;
};
#pragma pack(pop)
namespace std {

struct __false_type { __false_type() { } };
struct __true_type { __true_type() { } };
template <class _Tp> struct _Is_integer { typedef __false_type _Integral; };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int n) { return n != 0 ? (_Tp*)malloc(n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

struct _List_node_base {
    _List_node_base* _M_next;           // +0x00
    _List_node_base* _M_prev;           // +0x04
};

template <class _Tp> struct _List_node : public _List_node_base {
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
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
};

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, typename _Traits::_Non_const_traits> iterator;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    typedef allocator<_Node> _Node_allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }
    _List_base(const allocator_type&) : _M_node(_Node_allocator_type(), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;   // +0x00 the sentinel
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> >    const_iterator;

    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }

    template <class _InputIterator>
    void insert(iterator __pos, _InputIterator __first, _InputIterator __last)
    {
        typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
        _M_insert_dispatch(__pos, __first, __last, _Integral());
    }
    template <class _InputIter>
    void _M_insert_dispatch(iterator __position, _InputIter __first, _InputIter __last,
                            const __false_type&);   // 0x008FBF10 (this instantiation's own COMDAT)

    list(const list<_Tp, _Alloc>& __x) : _List_base<_Tp, _Alloc>(__x.get_allocator())
    {
        insert(begin(), __x.begin(), __x.end());
    }
};

}   // namespace std

template std::list<C3DVector >::list(const std::list<C3DVector >& __x);