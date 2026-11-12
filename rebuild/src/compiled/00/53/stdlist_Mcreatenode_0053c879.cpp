// std::list<NUISystem::CList*>::_M_create_node(NUISystem::CList* const& __x) -- STLport 4.6 list node factory: malloc one 0xC-byte node and placement-copy x into it
// Leaf: ?_M_create_node@?$list@PAVCList@NUISystem@@V?$allocator@PAVCList@NUISystem@@@std@@@std@@IAEPAU?$_List_node@PAVCList@NUISystem@@@2@ABQAVCList@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053C879, 26 bytes, thiscall (`this` unused), `ret 4`.
// Identity: its only caller is list<NUISystem::CList*>::insert 0x0053C97C (only caller push_front 0x0053CA3C, bound by the landed
//   real-typed NUISystem::CList::ChangeState 0x0053A0A1); FableWin.pdb / Ego_r.pdb carry list<NUISystem::CList*>. STLport 4.6 _list.h
//   `_Node* _M_create_node(const _Tp& __x) { _Node* __p = this->_M_node.allocate(1); _Construct(&__p->_M_data, __x); return __p; }`
//   (protected). Was the stand-in CRetailList_node0xc_dword::Buynode (FAM12, the callee of the respelled 0x0053C97C).
// Body: malloc 0x00BFEA0E (STLport's malloc allocator), the placement-new null test on &node->_M_data (+8).
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
typename list<_Tp, _Alloc>::_Node* list<_Tp, _Alloc>::_M_create_node(const _Tp& __x)
{
    _Node* __p = allocator<_Node>().allocate(1);
    _Construct(&__p->_M_data, __x);
    return __p;
}
template list<NUISystem::CList*>::_Node* list<NUISystem::CList*>::_M_create_node(NUISystem::CList* const&);
}
