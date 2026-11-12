// std::list<NUISystem::CList*>::insert(iterator __position, NUISystem::CList* const& __x) -- STLport 4.6 list insert: create a node holding x and link it in before position; returns the new node's iterator
// Leaf: ?insert@?$list@PAVCList@NUISystem@@V?$allocator@PAVCList@NUISystem@@@std@@@std@@QAE?AU?$_List_iterator@PAVCList@NUISystem@@U?$_Nonconst_traits@PAVCList@NUISystem@@@std@@@2@U32@ABQAVCList@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053C97C, 37 bytes, thiscall, `ret 0Ch` (hidden iterator result, position by value, x by reference).
// Identity: its only caller is list<NUISystem::CList*>::push_front 0x0053CA3C (agentFAM11: the landed real-typed NUISystem::CList::ChangeState
//   0x0053A0A1 binds that push_front; FableWin.pdb / Ego_r.pdb carry list<NUISystem::CList*>), whose body is STLport 4.6
//   `insert(begin(), __x)` and binds this exact spelling. Was the stand-in `_Construct_single` (FAM11 gap).
// Body: STLport 4.6 _list.h insert(iterator, const _Tp&): _M_create_node(x) (0x0053C879, `this` passed through in ecx), then the four
//   link stores in retail's order.
#pragma optimize("s",on)
extern "C" void* __cdecl malloc(unsigned int __size);   // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* __p) { return __p; }
inline void __cdecl operator delete(void*, void*) { }
namespace NUISystem { class CList; }
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> struct _Nonconst_traits { };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp, class _Traits> struct _List_iterator {
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
    _List_node_base* _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator insert(iterator __position, const _Tp& __x);
protected:
    _Node* _M_create_node(const _Tp& __x);
    _List_node_base* _M_node;
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
template list<NUISystem::CList*>::iterator list<NUISystem::CList*>::insert(list<NUISystem::CList*>::iterator, NUISystem::CList* const&);
}
