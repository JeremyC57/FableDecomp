// std::list<long>::push_front(const long& __x) -- insert x before the first element
// Leaf: ?push_front@?$list@JV?$allocator@J@std@@@std@@QAEXABJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00655ECE, 28 bytes, thiscall, `ret 4` (frame-pointer size module).
// Identity (proven): its insert is list<long>::insert 0x0042AB33 (landed); callers NPlayerGui::CDrawQuestInfo::OracleSymbolPushFront 0x0064A3C7 (OracleSymbols, list<long>, Ego_r.pdb) and CTCCreatureOpinionOfHero 0x00683770 (PendingDeeds, list<long>).
// Callees: list<long>::insert(iterator, const long&) 0x0042AB33 (thiscall, the iterator returned through a hidden pointer).
// Shape (family M122, representative 0x0053CA3C): STLport 4.6 `void push_front(const _Tp& __x) { insert(begin(), __x); }` --
//   `optimize("y", off)` + `optimize("s", on)`; begin() is built in its argument slot by the iterator's user copy constructor
//   (`push ecx ; mov eax,esp ; mov [eax],edx`), the discarded result is homed in the dead x slot.
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp> struct _Nonconst_traits { };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp, class _Traits> struct _List_iterator {
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
    _List_node_base* _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator insert(iterator __position, const _Tp& __x);  // 0x0042AB33
    void push_front(const _Tp& __x) { insert(begin(), __x); }
    _List_node_base* _M_node;
};
template void list<long >::push_front(long const&);
}