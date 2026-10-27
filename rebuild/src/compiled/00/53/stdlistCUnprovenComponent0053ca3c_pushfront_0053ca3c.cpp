// std::list<CUnprovenComponent_0053ca3c*>::push_front(const CUnprovenComponent_0053ca3c*& __x) -- insert x before the first element
// Leaf: ?push_front@?$list@PAVCUnprovenComponent_0053ca3c@@V?$allocator@PAVCUnprovenComponent_0053ca3c@@@std@@@std@@QAEXABQAVCUnprovenComponent_0053ca3c@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053CA3C, 28 bytes, thiscall, `ret 4` (frame-pointer size module).
// Identity (descriptive): its only caller is NUISystem::CList::ChangeState 0x0053A0A1 (Ego_r.pdb ?ChangeState@CList@NUISystem@@UAEXK@Z), which pushes `this` onto the global list at 0x013B8A98 (also walked by CTCBoastUI / CTCQuickAccessMenu::GlobalUpdate); the insert 0x0053C97C builds 0xC-byte nodes (a 4-byte element); no symbol names the global or its element type.
// Callees: list<CUnprovenComponent_0053ca3c*>::insert(iterator, const CUnprovenComponent_0053ca3c*&) 0x0053C97C (thiscall, the iterator returned through a hidden pointer).
// Shape (family M122, representative 0x0053CA3C): STLport 4.6 `void push_front(const _Tp& __x) { insert(begin(), __x); }` --
//   `optimize("y", off)` + `optimize("s", on)`; begin() is built in its argument slot by the iterator's user copy constructor
//   (`push ecx ; mov eax,esp ; mov [eax],edx`), the discarded result is homed in the dead x slot.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CUnprovenComponent_0053ca3c;
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
template void list<CUnprovenComponent_0053ca3c* >::push_front(CUnprovenComponent_0053ca3c* const&);
}