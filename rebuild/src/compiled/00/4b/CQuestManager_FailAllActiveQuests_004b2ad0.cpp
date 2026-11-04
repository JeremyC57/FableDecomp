// CQuestManager::FailAllActiveQuests() -- fail (silently) every active quest that is running a script, working on a copy of the active-quest list
// Leaf: ?FailAllActiveQuests@CQuestManager@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B2AD0, 149 bytes. Identity: Ego_r.pdb ?FailAllActiveQuests@CQuestManager@@QAEXXZ; the script
// forwarder CGameScriptInterface::FailAllActiveQuests (0x00891520, RTTI vtable) is `mov ecx,[0x013B89FC] ; jmp 0x004B2AD0`. The catalog's
// DeactivateEventQuests is a forge-autoname guess (no such PDB name). Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (the copy's sentinel),
// list::_M_insert_dispatch<const_iterator> 0x004B92D0, CWideString(const wchar_t*) 0x0099B6B0 (L"" 0x0122D70C),
// CQuestManager::SetQuestAsFailed 0x004B2280, ~CWideString 0x0099B510, _List_base::clear 0x004B9DA0.
// Shape: `list<CCountedPointer<CActiveQuest> > quests(ActiveQuests);` -- the landed STLport copy-constructor model (0x004BADC0) expanded
// in place, an iterator walk with a named CWideString reason per failed quest, and the list destructor (clear, free the sentinel).
// The out-of-line callees _M_insert_dispatch (0x004B92D0) and _List_base::clear (0x004B9DA0) are same-TU __declspec(noinline)
// bodies (EXTRA_RULES chunk 422): with them visible VC7.1 knows the copy's sentinel pointer is not changed by either call and
// keeps it in esi (retail), sinking the sentinel store to the call; declared-only they force a reload after every call (156 v 149).
// Header debt: engine/CQuestManager.h's retail branch has no ActiveQuests and stlport/fable_containers.h's list inlines both
// callees, so the manager and the list are local models (as the landed 0x004BADC0 / 0x004B92D0 rows).
#include <stddef.h>
#include <new>
#include "fable_string.h"
extern "C" void* __cdecl malloc(size_t size);           // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
#include "engine/CCountedPointer.h"
class CScriptInfo;
class CScriptBase;
class CScriptDataBase;
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    _Tp* allocate(size_t __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
    void deallocate(_Tp* __p, size_t) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Tp* allocate(size_t __n) { return _MaybeReboundAlloc::allocate(__n); }
    void deallocate(_Tp* __p, size_t __n) { _MaybeReboundAlloc::deallocate(__p, __n); }
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
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
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
    typename _Traits::reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
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
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    // _List_base::clear 0x004B9DA0: out of line in retail; a same-TU noinline body (EXTRA_RULES chunk 422).
    __declspec(noinline) void clear()
    {
        _Node* __cur = (_Node*)_M_node._M_data->_M_next;
        while (__cur != _M_node._M_data) {
            _Node* __tmp = __cur;
            __cur = (_Node*)__cur->_M_next;
            __tmp->_M_data.~_Tp();
            _M_node.deallocate(__tmp, 1);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
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
    iterator end() { return iterator(this->_M_node._M_data); }
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
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = this->_M_node.allocate(1);
        new ((void*)&__tmp->_M_data) _Tp(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    // 0x004B92D0: out of line in retail; a same-TU noinline body (the landed 0x004B92D0 loop).
    template <class _InputIter>
    __declspec(noinline) void _M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last, const __false_type&)
    {
        for (; __first != __last; ++__first)
            insert(__pos, *__first);
    }
};
}
class CQuestManager {
public:
    // Ego_r.pdb CActiveQuest (retail offsets: the 12-byte STLport vector puts QuestName at +0x30).
    class CActiveQuest {
    public:
        long                            ID;                  // +0x00
        CScriptInfo*                    PScriptInfo;         // +0x04
        CCountedPointer<CScriptBase>    PScript;             // +0x08
        unsigned char                   Regions[0xc];        // +0x10 vector<const CRegion*>
        unsigned char                   MarkerPosition[8];   // +0x1c C2DCoordI
        bool                            Persistent;          // +0x24
        bool                            LoadedResources;     // +0x25
        CCountedPointer<CScriptDataBase> PScriptData;        // +0x28
        CCharString                     QuestName;           // +0x30
    };
    bool SetQuestAsFailed(const CCharString& name, bool show_screen, const CWideString& reason, bool silent);   // 0x004B2280
    void FailAllActiveQuests();
private:
    unsigned char _pad_0x00[0x38];
    std::list<CCountedPointer<CActiveQuest> > ActiveQuests;  // +0x38
};
void CQuestManager::FailAllActiveQuests()
{
    std::list<CCountedPointer<CActiveQuest> > quests(ActiveQuests);
    for (std::list<CCountedPointer<CActiveQuest> >::iterator it = quests.begin(); it != quests.end(); ++it) {
        CActiveQuest* quest = (*it).Data;
        if (quest->PScript.Data) {
            CWideString reason(L"");
            SetQuestAsFailed(quest->QuestName, false, reason, true);
        }
    }
}