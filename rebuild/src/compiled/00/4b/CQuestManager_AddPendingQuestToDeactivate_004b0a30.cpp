// CQuestManager::AddPendingQuestToDeactivate(const CCharString& quest_name, unsigned long flags) -- queue a quest for deactivation: push a (name, flags) pair onto the back of the pending-deactivation list
// Leaf: ?AddPendingQuestToDeactivate@CQuestManager@@QAEXABVCCharString@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B0A30, 102 bytes (112 with padding), `ret 8` (__fastcall).
// Identity: Ego_r.pdb `?AddPendingQuestToDeactivate@CQuestManager@@QAEXABVCCharString@@K@Z`; callers 0x004B0F87 (a
//   quest-manager loop passing a quest's name at +0x28 and 2) and the script-interface forwarder 0x00892F10
//   (`mov ecx,[g_pQuestManager] ; jmp`).
// Callees: CCharString copy ctor 0x0099EC30 (the pair temporary, then the node's value), malloc 0x00BFEA0E (STLport
//   malloc allocator: the 16-byte list node), ~CCharString 0x0099EAE0 (the temporary).
// Layout (retail): CQuestManager PendingQuestsToDeactivate (STLport list<pair<CCharString, unsigned long> >, head-node
//   pointer) +0x3C, as the landed CQuestManager::Update 0x004B4490; nodes {_M_next +0, _M_prev +4, pair +8}.
// Shape: `PendingQuestsToDeactivate.push_back(pair<CCharString, unsigned long>(quest_name, flags));` with STLport
//   push_back -> insert(end(), x): `_M_create_node` (allocate(1) folds to malloc(16), placement `_Construct`
//   null-tested) and the link order `n = pos._M_node; p = n->_M_prev; tmp->_M_next = n; tmp->_M_prev = p;
//   p->_M_next = tmp; n->_M_prev = tmp;`.
// Label correction: catalogued as `ListNode_InsertEntry`; the real routine is CQuestManager::AddPendingQuestToDeactivate.
#include <stdlib.h>                                   // malloc @ 0x00BFEA0E
#include <new>

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __p) : first(__p.first), second(__p.second) { }
};

struct _List_node_base {
    _List_node_base* _M_next;                        // +0x00
    _List_node_base* _M_prev;                        // +0x04
};
template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;                                     // +0x08
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
};

template <class _Tp> struct allocator {
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> struct list {                   // STLport list (the head node pointer: 4 bytes in retail)
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator end() { return iterator(this->_M_node._M_data); }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};

class CQuestManager {
public:
    void*         __vftable;                         // +0x00
    unsigned char _pad04[0x3C - 0x04];               // PGameScriptThingManager .. ActiveQuests (+0x38)
    list<pair<CCharString, unsigned long> > PendingQuestsToDeactivate;   // +0x3C

    void AddPendingQuestToDeactivate(const CCharString& quest_name, unsigned long flags);
};

void CQuestManager::AddPendingQuestToDeactivate(const CCharString& quest_name, unsigned long flags)
{
    PendingQuestsToDeactivate.push_back(pair<CCharString, unsigned long>(quest_name, flags));
}