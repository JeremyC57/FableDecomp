// std::list<NUISystem::CComponent*>::list(const allocator& a) -- STLport list constructor: allocate the sentinel node and
//   link it to itself
// Leaf: ??0?$list@PAVCComponent@NUISystem@@V?$allocator@PAVCComponent@NUISystem@@@std@@@std@@QAE@ABV?$allocator@PAVCComponent@NUISystem@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042ABAF, 27 bytes, `ret 4` (the allocator reference), returns this.
// Identity: STLport 4.6 `explicit list(const allocator_type& = allocator_type())` with _List_base's constructor inline
//   (`_M_node(__a, 0)` -> `and [esi],0`, then allocate(1), self-linked); the landed stand-in was a free function `list`.
//   Instantiation: the run 0x0042A008 (_M_create_node), 0x0042AB8A (insert), 0x0042ABAF (list()), 0x0042ABCA (~list) is one
//   std::list<NUISystem::CComponent*> (no ICF in retail): ~list is the landed list<CComponent*> destructor, insert is the
//   callee of the landed list<CComponent*>::_M_insert_dispatch 0x0042B4C3 (Ego_r NUISystem::CDeletion::AssociatedParents
//   list<CComponent*>), and the constructor's callers include NUISystem::CDeletion::CDeletion 0x0042CD84 / 0x0042BED4.
// Callees: malloc 0x00BFEA0E (allocate(1)).
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
    explicit list(const allocator<_Tp>& __a);
};

template <class _Tp, class _Alloc>
list<_Tp, _Alloc>::list(const allocator<_Tp>&) : _M_node(0)
{
    _Node* __n = allocator<_Node>().allocate(1);
    __n->_M_next = __n;
    __n->_M_prev = __n;
    _M_node = __n;
}
template class list<NUISystem::CComponent*, allocator<NUISystem::CComponent*> >;
}
