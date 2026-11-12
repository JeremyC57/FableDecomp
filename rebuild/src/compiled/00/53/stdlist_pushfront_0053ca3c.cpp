// std::list<NUISystem::CList*>::push_front(NUISystem::CList* const& __x) -- insert x before the first element
// Leaf: ?push_front@?$list@PAVCList@NUISystem@@V?$allocator@PAVCList@NUISystem@@@std@@@std@@QAEXABQAVCList@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053CA3C, 28 bytes, thiscall, `ret 4` (frame-pointer size module).
// Types (proven, agentFAM11, 2026-11-12): list<NUISystem::CList*>. Sources: (1) retail: the landed real-typed NUISystem::CList::ChangeState
//   0x0053A0A1 calls this push_front and its relocation binds exactly this spelling (rebuild/oracles/reloc_bindings.tsv); (2) FableWin.pdb /
//   Ego_r.pdb carry list<NUISystem::CList*> (?clear@?$list@PAVCList@NUISystem@@...). Was the placeholder element CUnprovenComponent_0053ca3c;
//   name-only change, the same body.
// Context: its only caller is NUISystem::CList::ChangeState 0x0053A0A1 (Ego_r.pdb ?ChangeState@CList@NUISystem@@UAEXK@Z), which pushes `this` onto the global list at 0x013B8A98 (also walked by CTCBoastUI / CTCQuickAccessMenu::GlobalUpdate); the insert 0x0053C97C builds 0xC-byte nodes (a 4-byte element); no symbol names the global or its element type.
// Callees: list<NUISystem::CList*>::insert(iterator, NUISystem::CList* const&) 0x0053C97C (landed as the stand-in _Construct_single) (thiscall, the iterator returned through a hidden pointer).
// Shape (family M122, representative 0x0053CA3C): STLport 4.6 `void push_front(const _Tp& __x) { insert(begin(), __x); }` --
//   `optimize("y", off)` + `optimize("s", on)`; begin() is built in its argument slot by the iterator's user copy constructor
//   (`push ecx ; mov eax,esp ; mov [eax],edx`), the discarded result is homed in the dead x slot.
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace NUISystem { class CList; }
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
    iterator insert(iterator __position, const _Tp& __x);  // 0x0053C97C
    void push_front(const _Tp& __x) { insert(begin(), __x); }
    _List_node_base* _M_node;
};
template void list<NUISystem::CList* >::push_front(NUISystem::CList* const&);
}