// std::list<CCountedPointer<CStrikeEvent> >::list(const list& x) -- STLport list copy constructor for the combat action's strike-event list: allocate and self-link the sentinel, then range-insert x's elements at begin()
// Leaf: ??0?$list@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0062F610, 55 bytes (+ int3 padding), `ret 4`.
// Identity: the element type is CCountedPointer<CCombatActionBase::CStrikeEvent>: the only caller is the landed
// CCombatActionBase copy constructor 0x0062F3B0 (call at 0x0062F431, copying PDB CCombatActionBase::StrikeEvents,
// a list<CCountedPointer<CCombatActionBase::CStrikeEvent>>). The catalog label "std_list_Initialize_Window" is
// propagated and wrong.
// Callees: malloc @ 0x00BFEA0E (the STLport node allocator; `add esp,4` delayed), list::insert<const_iterator>(iterator,
// const_iterator, const_iterator) @ 0x0062F650 (landed, `ret 0Ch`).
// Shape: STLport 4.x `list(const list& x) : _List_base(x.get_allocator()) { insert(begin(), x.begin(), x.end()); }` with
// the _List_base constructor INLINED here: `_M_node(alloc, 0)` is the zero store between the size push and the malloc,
// then the new node is self-linked and published. x.end() / x.begin() are const_iterators whose copy is trivial
// (pushed directly); begin() is an iterator with STLport's user-declared `_List_iterator(const iterator&)`, so it is
// built in place in its reserved argument slot (`push ecx ; mov ecx,esp ; mov [ecx],edx`). Same skeleton as the
// landed list copy ctor 0x00463871 (which calls its _List_base out of line). Node: next +0, prev +4, value +8 (16 bytes).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CCPPointerInfo;
template <class T> struct CCountedPointer {      // 8 bytes: object + shared reference-count block
    T*              Data;                        // +0x00
    CCPPointerInfo* Info;                        // +0x04
};

class CCombatActionBase {
public:
    class CStrikeEvent;
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() {}
    allocator(const allocator<_Tp>&) {}
    _Tp* allocate(unsigned int n) { return n != 0 ? (_Tp*)malloc(n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
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
    void insert(iterator __pos, _InputIterator __first, _InputIterator __last);   // 0x0062F650 for const_iterator

    list(const list<_Tp, _Alloc>& __x) : _List_base<_Tp, _Alloc>(__x.get_allocator())
    {
        insert(begin(), __x.begin(), __x.end());
    }
};

} // namespace std

template std::list<CCountedPointer<CCombatActionBase::CStrikeEvent> >::list(const std::list<CCountedPointer<CCombatActionBase::CStrikeEvent> >& __x);