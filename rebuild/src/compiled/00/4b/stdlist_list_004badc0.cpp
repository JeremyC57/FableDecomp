// std::list::list(const list& x) -- (list<CCountedPointer<CQuestManager::CActiveQuest> >) STLport copy constructor: zero the node proxy, malloc the 0x10-byte sentinel and link it to itself, then insert(begin(), x.begin(), x.end()) through the out-of-line _M_insert_dispatch
// Leaf: ??0?$list@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BADC0, 63 bytes (+ int3 padding to 64), thiscall, `ret 4`, returns this.
// Identity: list<CCountedPointer<CQuestManager::CActiveQuest> > -- proven; evidence: its _M_insert_dispatch 0x004B92D0 (copies an 8-byte {Data, Info} element and bumps Info's count) is also called by CQuestManager::DeactivateEventQuest 0x004B2AD0, which copies the list at CQuestManager+0x38 = Ego_r.pdb ActiveQuests (list<CCountedPointer<CQuestManager::CActiveQuest> >; retail offset after the 12-byte STLport containers) and walks it testing CActiveQuest::PScript.Data (+8) and passing &QuestName (+0x30 with retail's 12-byte Regions vector); Ego_r.pdb names list<CCountedPointer<CQuestManager::CActiveQuest> >::remove (same list type); no direct caller (unreferenced COMDAT).
// Label: the manifest's "StdList_InitializeFromRange" is propagated across byte-identical bodies; this is the STLport 4.6 list copy
//   constructor (`ret 4`, one `const list&` argument whose node pointer gives x.begin() / x.end()).
// Callees: malloc @ 0x00BFEA0E (_STLP_alloc_proxy::allocate(1) of the 0x10-byte _List_node), then
//   list::_M_insert_dispatch<const_iterator>(iterator pos, const_iterator first, const_iterator last, const __false_type&)
//   @ 0x004B92D0 (`ret 10h`).
// Body: STLport 4.6 `list(const list& x) : _List_base(x.get_allocator()) { insert(begin(), x.begin(), x.end()); }` with
//   `_List_base(a) : _M_node(a, (_Node*)0) { _Node* n = _M_node.allocate(1); n->_M_next = n; n->_M_prev = n;
//   _M_node._M_data = n; }`. The non-const iterator's user copy constructor (`_List_iterator(const iterator&)`) builds
//   `pos` in its argument slot (`push ecx ; mov ecx,esp ; mov [ecx],eax`); the const_iterators (implicit copy) are pushed
//   as plain words; the empty __false_type dispatch tag is homed in the dead `x` slot.
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);           // 0x00BFEA0E
class CQuestManager {
public:
    class CActiveQuest;
};
template <class T> class CCountedPointer {         // 8 bytes: the object + its shared CCPPointerInfo count block
public:
    T*    Data;
    void* Info;
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    _Tp* allocate(size_t __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Tp* allocate(size_t __n) { return _MaybeReboundAlloc::allocate(__n); }
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp* pointer; typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp* pointer; typedef const _Tp& reference; };
template <class _Tp, class _Traits>
struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
};
template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Node> _Node_allocator_type;
    typedef _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _AllocProxy;
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type((const _Node_allocator_type&)_M_node); }
    _List_base(const allocator_type& __a) : _M_node(_Node_allocator_type(__a), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    _AllocProxy _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    const_iterator begin() const { return const_iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    list(const list<_Tp, _Alloc>& __x) : _List_base<_Tp, _Alloc>(__x.get_allocator())
    {
        insert(begin(), __x.begin(), __x.end());
    }
    template <class _InputIterator>
    void insert(iterator __pos, _InputIterator __first, _InputIterator __last)
    {
        _M_insert_dispatch(__pos, __first, __last, __false_type());
    }
    template <class _InputIter>                                  // 0x004B92D0 (out of line in retail)
    void _M_insert_dispatch(iterator __position, _InputIter __first, _InputIter __last, const __false_type&);
};
template list<CCountedPointer<CQuestManager::CActiveQuest> >::list(const list<CCountedPointer<CQuestManager::CActiveQuest> >&);
}