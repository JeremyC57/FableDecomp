// CAIStateGroup_MinionGuardReactToPlayerWithWeapon::RemoveMessageEventsAdded() -- withdraw every "warning" message (type 0x1B) this guard originated: find the most recent one, unlink it from the message manager's list, repeat until none is left
// Grade: DIFFER(155v150) -- retail hoists the Bind2 `m_pFirst = &IsOfType` store (`lea eax,[esp+8] ; mov [esp+10h],eax`) above the unaligned loop top and stores 0x1B after the creature push; ours keeps both stores in the loop (aligned with `mov edi,edi`)
// Leaf: ?RemoveMessageEventsAdded@CAIStateGroup_MinionGuardReactToPlayerWithWeapon@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00939DD0, 150 bytes (160 with padding), __thiscall, an `and esp,-8` frame (the 8-byte
//   UID filter). Ego_r.pdb ?RemoveMessageEventsAdded@CAIStateGroup_MinionGuardReactToPlayerWithWeapon@@AAEXXZ (private);
//   caller 0x0093A321 (this class's Update). The catalog's "..._CheckAndRemove" is descriptive.
// Callees: CMessageEventFilter_IsOriginatedByThing::CMessageEventFilter_IsOriginatedByThing(const CThing*) 0x006AF2B0 (out
//   of line here: the ctor's returned `this` becomes the Bind2's second pointer), CAIStateGroupBase::GetPThingWorld
//   0x008FCFA0 / GetPWorld 0x008FCFC0, CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<IsOfType,
//   IsOriginatedByThing>> 0x0090DEE0, free 0x00BFEA14; the event's virtual destructor (slot 0, `push 0`).
// Layout: Ego_r.pdb CMessageEventManager {World +0, Messages +4 (STLport list<CMessageEvent>, one sentinel pointer)};
//   CMessageEvent 0x58 bytes with a vtable and 8-byte UIDs, so the list node's data sits at +8.
// Shape: the landed 0x00939D70 / 0x009415B0 filter-chain model; CMessageEventManager::RemoveMessage inlined (the list scan
//   `&*it == event`, then STLport erase: unlink, _Destroy through the virtual destructor, free). The do/while form gives
//   retail's single-copy loop (a `for (;;) { ...; if (!event) break; ... }` or a goto is rotated into two Find copies,
//   216 bytes; a tail-recursive form keeps RemoveMessage out of line). Tried: for(;;)+break, do/while (best), tail
//   recursion, backward goto.
#include <stddef.h>
extern "C" void __cdecl free(void* p);          // 0x00BFEA14

class CThing;
class CWorld;

class CMessageEvent {                           // Ego_r.pdb: 0x58 bytes, vtable + 8-byte aligned UIDs
public:
    virtual ~CMessageEvent();
    unsigned char _pad[0x58 - 8];
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                // +0x08
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator==(const _List_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp> class list {               // STLport list: one sentinel pointer
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    _List_node_base* _M_node;
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        free(__n);
        return iterator(__next_node);
    }
};

struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};

struct CMessageEventFilter_IsOriginatedByThing {
    unsigned __int64 UID;                       // the originator's 64-bit UID (PDB)
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF2B0
};

template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }

typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatedByThing> COwnMessageFilter;

class CMessageEventManager {
public:
    CWorld* World;                              // +0x00
    list<CMessageEvent> Messages;               // +0x04
    CMessageEvent* FindMostRecentMessage_0090dee0(const COwnMessageFilter& filter);   // 0x0090DEE0
    void RemoveMessage(const CMessageEvent* event)
    {
        for (list<CMessageEvent>::iterator it = Messages.begin(); it != Messages.end(); ++it) {
            if (&*it == event) {
                Messages.erase(it);
                return;
            }
        }
    }
};

class CWorld {
public:
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    CWorld* GetPThingWorld();                   // 0x008FCFA0
    CWorld* GetPWorld();                        // 0x008FCFC0
};

struct CAIStateGroup_MinionGuardReactToPlayerWithWeapon : CAIStateGroupBase {
    void RemoveMessageEventsAdded();
};

void CAIStateGroup_MinionGuardReactToPlayerWithWeapon::RemoveMessageEventsAdded()
{
    const CMessageEvent* event;
    do {
        event = GetPThingWorld()->GetPMessageManager()->FindMostRecentMessage_0090dee0(
            CMessageEventFilter_IsOfType(0x1b) && CMessageEventFilter_IsOriginatedByThing(PBrain->PCreature));
        if (event)
            GetPWorld()->GetPMessageManager()->RemoveMessage(event);
    } while (event);
}
