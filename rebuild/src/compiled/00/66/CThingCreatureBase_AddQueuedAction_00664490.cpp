// CThingCreatureBase::AddQueuedAction(const CCreatureActionBase& action) -- append an empty counted pointer to the queued-action list, then point it at a clone of the action
// Leaf: ?AddQueuedAction@CThingCreatureBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00664490, 85 bytes + 11 int3, thiscall `ret 4`. Name: Ego_r.pdb
// `?AddQueuedAction@CThingCreatureBase@@QAEXABVCCreatureActionBase@@@Z` (name set; it sits between the landed
// CThingCreatureBase::UpdateAction 0x006643C0 and SetCurrentAction 0x006644F0). Callers: 0x00712A18 / 0x00712B11
// (NTCScriptedControl::CActionPlayAnimation family), CTCCombat::SetPossibleQueuedAction 0x007332B7.
// Callees: malloc @ 0x00BFEA0E (the 16-byte list node), the action's virtual Clone() const (slot 11, +0x2C; e.g. the
// landed 0x0066A7B0), CCountedPointer<CCreatureActionBase>::Reset(T*) @ 0x00668D90 (manifest label
// "CCountedPointer<const CIPhysicsFacesSegmentBase>::Reset" is an identical-body label).
// Label correction: the catalog "PhysicsSegment_AssignSegment" is descriptive.
// Layout: retail PQueuedActions (STLport list: the sentinel pointer) at +0x108 (PDB +0x124); node next +0, prev +4,
// CCountedPointer {Data, Info} +8.
// Shape: `PQueuedActions.push_back(CCountedPointer<CCreatureActionBase>()); PQueuedActions.back().Reset(action.Clone());`
// with STLport's inline push_back -> insert(end(), x) -> _M_create_node: the empty temporary's copy (count bump) and
// destruction fold away into the two zero stores behind the placement null test; back() re-reads the sentinel.
// No pragma (speed TU: `add esp,4` after malloc).
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);      // 0x00BFEA0E
void __cdecl operator delete(void* block);         // 0x00BFE9BC
inline void* operator new(size_t, void* p) { return p; }

class CCreatureActionBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual CCreatureActionBase* Clone() const;     // slot 11 (+0x2C)
};

struct CCountedPointerInfo {
    long  Count;                                    // +0x00
    void (__fastcall* Release)(void* object);       // +0x04 (called with the object in ecx)
    void* Object;                                   // +0x08
};

template <class T> class CCountedPointer {
public:
    T*                   Data;                      // +0x00
    CCountedPointerInfo* Info;                      // +0x04
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->Count; }
    ~CCountedPointer()
    {
        if (Info && --Info->Count == 0) {
            Info->Release(Info->Object);
            operator delete(Info);
        }
    }
    void Reset(T* data);                            // 0x00668D90
};

namespace std {
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(size_t __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

struct _List_node_base {
    _List_node_base* _M_next;                       // +0x00
    _List_node_base* _M_prev;                       // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                    // +0x08
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator--() { _M_node = _M_node->_M_prev; return *this; }
};

template <class _Tp> class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;   // the sentinel (retail: 4 bytes)
    iterator begin() { return iterator(_M_node._M_data->_M_next); }
    iterator end() { return iterator(_M_node._M_data); }
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
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
    _Tp& back() { iterator __tmp = end(); --__tmp; return *__tmp; }
};
}

class CThingCreatureBase {
public:
    unsigned char _pad000[0x108];
    std::list<CCountedPointer<CCreatureActionBase> > PQueuedActions;   // +0x108 (PDB +0x124)
    void AddQueuedAction(const CCreatureActionBase& action);
};

void CThingCreatureBase::AddQueuedAction(const CCreatureActionBase& action)
{
    PQueuedActions.push_back(CCountedPointer<CCreatureActionBase>());
    PQueuedActions.back().Reset(action.Clone());
}