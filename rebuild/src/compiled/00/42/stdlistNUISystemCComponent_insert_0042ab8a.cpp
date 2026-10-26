// std::list<NUISystem::CComponent*>::insert(iterator pos, CComponent* const& x) -- STLport list insert: build a node for x and
//   link it in before pos; returns the iterator to it
// Leaf: ?insert@?$list@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@QAE?AU?$_List_iterator@PAVCComponent@NUISystem@@U?$_Nonconst_traits@PAVCComponent@NUISystem@@@std@@@2@U32@ABQAVCComponent@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042AB8A, 37 bytes, `ret 0Ch` (hidden result, pos, &x); `this` stays in ecx for the
//   node factory.
// Identity: called by the landed list<NUISystem::CComponent*>::_M_insert_dispatch 0x0042B4C3 for each element; STLport 4.6
//   list::insert(iterator, const _Tp&) statement for statement. The catalog's Std_List_Insert is descriptive.
//   Instantiation: the run 0x0042A008 (_M_create_node), 0x0042AB8A (insert), 0x0042ABAF (list()), 0x0042ABCA (~list) is one
//   std::list<NUISystem::CComponent*> (no ICF in retail): ~list is the landed list<CComponent*> destructor, insert is the
//   callee of the landed list<CComponent*>::_M_insert_dispatch 0x0042B4C3 (Ego_r NUISystem::CDeletion::AssociatedParents
//   list<CComponent*>), and the constructor's callers include NUISystem::CDeletion::CDeletion 0x0042CD84 / 0x0042BED4.
// Callees: _M_create_node 0x0042A008.
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }
namespace NUISystem { class CComponent; }
namespace std {
template <class _Tp> class allocator {                       // STLport 4.6 (malloc-based): a class
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
struct _List_node_base {
    _List_node_base* _M_next;                                // +0x00
    _List_node_base* _M_prev;                                // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                             // +0x08
};
template <class _Tp> struct _Nonconst_traits { };
template <class _Tp, class _Traits> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};
template <class _Tp, class _Alloc> class list {              // _List_base folded in: one word, the sentinel node
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _Node* _M_node;
    iterator insert(iterator __position, const _Tp& __x);
protected:
    _Node* _M_create_node(const _Tp& __x);                   // 0x0042A008
};

template <class _Tp, class _Alloc>
typename list<_Tp, _Alloc>::iterator list<_Tp, _Alloc>::insert(iterator __position, const _Tp& __x)
{
    _Node* __tmp = _M_create_node(__x);
    _List_node_base* __n = __position._M_node;
    _List_node_base* __p = __n->_M_prev;
    __tmp->_M_next = __n;
    __tmp->_M_prev = __p;
    __p->_M_next = __tmp;
    __n->_M_prev = __tmp;
    return __tmp;
}
template class list<NUISystem::CComponent*, allocator<NUISystem::CComponent*> >;
}
