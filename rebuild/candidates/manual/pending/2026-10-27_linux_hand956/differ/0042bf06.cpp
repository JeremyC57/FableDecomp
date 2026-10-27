// std::list::list(const list& __x) -- (<NUISystem::CComponent*>) the copy constructor: an empty list, then insert(begin(), x.begin(), x.end())
// Grade: DIFFER(48v47) -- empty allocator temporary at [ebp-1] (push ecx frame, leave) where retail overlays it on the live __x slot [ebp+0Bh] (RTM lane gap, GOTCHAS wave 24l)
// Leaf: ??0?$list@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042BF06, 47 bytes, thiscall, `ret 4`, frame-pointer size module.
// Identity (proven): its only caller is the NUISystem::CDeletion copy constructor 0x0042CD84 (Method, then `AssociatedParents(other.AssociatedParents)` at +4; Ego_r.pdb CDeletion::AssociatedParents is list<NUISystem::CComponent*>); the base constructor 0x0042ABAF is bound ??0?$list@PAVCComponent@NUISystem@@... and the insert 0x0042B945 is the landed std::list<NUISystem::CComponent*>::insert.
// Callees: _List_base<NUISystem::CComponent*>(const allocator&) 0x0042ABAF (sentinel node), list<NUISystem::CComponent*>::insert<const_iterator>(iterator,
//   const_iterator, const_iterator) 0x0042B945.
// Shape (family M022, representative 0x0042BF06): `optimize("y", off)` + `optimize("s", on)` (push ebp frame, unaligned
//   entry). The pos iterator has STLport's user copy constructor `_List_iterator(const iterator&)`, so begin() is built in
//   the argument area (`push ecx ; mov eax,esp ; mov [eax],ecx`); const_iterator's copy is implicit (plain pushes).
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace NUISystem { class CComponent; }
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp> struct _Nonconst_traits { };
template <class _Tp> struct _Const_traits { };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
struct _List_iterator_base {
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_node_base* _M_node;
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
};
template <class _Tp, class _Alloc> class _List_base {
public:
    _List_base(const _Alloc& __a);                        // 0x0042ABAF
    _List_node<_Tp>* _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    _Alloc get_allocator() const { return _Alloc(); }
    iterator begin() { return iterator(this->_M_node->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node); }
    list(const list& __x) : _List_base<_Tp, _Alloc>(__x.get_allocator()) { insert(begin(), __x.begin(), __x.end()); }
    template <class _InputIter> void insert(iterator __pos, _InputIter __first, _InputIter __last);   // 0x0042B945
};
template list<NUISystem::CComponent* >::list(const list<NUISystem::CComponent* >&);
}
