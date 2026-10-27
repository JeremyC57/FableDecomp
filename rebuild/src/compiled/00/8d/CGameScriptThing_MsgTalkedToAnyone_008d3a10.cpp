// CGameScriptThing::MsgTalkedToAnyone() const -- true when the live thing's entity-event list holds an event of type 0x21 stamped inside the running quest thread's (last, current] script-frame window
// Leaf: ?MsgTalkedToAnyone@CGameScriptThing@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D3A10, 219 bytes, thiscall, plain `ret`.
// Identity (proven): CGameScriptThing vtable 0x012852CC slot 54; the manifest label is its FableWin.pdb mangled name.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x3), CVectorMap<ETCInterfaceType, CTCBase*, ...>::LowerBound(const
//   ETCInterfaceType&) const 0x004365B0, the event's virtual GetType (slot 1).
// Shape (family K042, representative 0x008D2700): IS_THING_ALIVE(PThing); the running quest's last / current script frames
//   (null quest or script -> 0); the const GetTC<CTCEntityEvents> (interface 0x42: the bitset word & mask tested in the `if`
//   itself -- through operator[]'s bool VC7.1 emits shr/test -- and the
//   const lower bound); the event list walk with the type and frame-window test -- the landed sibling
//   MsgPerformedAnyAggressiveSpecialAbility 0x008D27E0 without its Aggressive flag, on Ego_r.pdb types. Header debt:
//   engine/CGameScriptThing.h / CTCEntityEvents.h / CQuestManager.h flatten the vptr or hold PDB-width containers.
#include <stddef.h>
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {
    _Value _M_data;
};
template <class _Tp, class _Alloc> class vector {           // retail STLport vector (12 bytes)
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    _Tp* _M_start;                                            // +0x00
    _Tp* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    const_iterator end() const { return this->_M_finish; }
};
template <size_t _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t __pos) { return __pos / 32; }
    static _WordT _S_maskbit(size_t __pos) { return 1UL << (__pos % 32); }
    _WordT _M_getword(size_t __pos) const { return _M_w[_S_whichword(__pos)]; }
};
template <size_t _Nb> class bitset : public _Base_bitset<(_Nb + 31) / 32> {
public:
    bool operator[](size_t __pos) const { return _Unchecked_test(__pos); }
    bool _Unchecked_test(size_t __pos) const { return (this->_M_getword(__pos) & this->_S_maskbit(__pos)) != 0; }
};
struct _List_node_base {
    _List_node_base* _M_next;                                 // +0x00
    _List_node_base* _M_prev;                                 // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                              // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {   // STLport list, 4 bytes
public:
    struct const_iterator {
        _List_node_base* _M_node;
        const_iterator(_List_node_base* __x) : _M_node(__x) { }
        const _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
        const _Tp* operator->() const { return &((_List_node<_Tp>*)_M_node)->_M_data; }
        const_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const const_iterator& __y) const { return _M_node != __y._M_node; }
    };
    _STLP_alloc_proxy<_List_node<_Tp>*, _Tp, allocator<_List_node<_Tp> > > _M_node;   // +0x00
    const_iterator begin() const { return const_iterator(_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(_M_node._M_data); }
};
}   // namespace std
template <class T> class CArray : public std::vector<T, std::allocator<T> > { };
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    typedef typename CArray<value_type>::const_iterator const_iterator;
    CArray<value_type> Data;                                  // +0x00
    TCompare           Compare;                               // +0x0C
    bool               Dirty;                                 // +0x0D
    const_iterator LowerBound(const K& key) const;            // 0x004365B0 for the TC map (the const overload)
};
class CCPPointerInfo;
template <class T> class CCountedPointer {                    // Ego_r.pdb: Data, Info
public:
    T* Data;                                                  // +0x00
    CCPPointerInfo* Info;                                     // +0x04
    T* operator->() const { return Data; }
};
enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };
class CTCBase;
class CThing {
public:
    unsigned char      _pad00[0x20];
    std::bitset<274>   UsingComponents;                                   // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    unsigned char      _pad54[0x91 - 0x54];
    unsigned char      LifeFlags;                                         // +0x91 bit 0: dead
    template <class T> __forceinline bool GetTC(T** out) const
    {
        if (UsingComponents._M_getword(T::INTERFACE_TYPE) & UsingComponents._S_maskbit(T::INTERFACE_TYPE)) {
            ETCInterfaceType type = (ETCInterfaceType)T::INTERFACE_TYPE;
            const CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >::const_iterator it = map->LowerBound(type);
            if (it == map->Data.end() || T::INTERFACE_TYPE < it->first)
                it = map->Data.end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                               // Ego_r: vptr + PData, 8 bytes
public:
    CBaseIntelligentPointer();                                // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                       // 0x00A01C10
    CBaseObject* GetPItem() const;                            // 0x00A01B50
private:
    CBaseObjectPointer* PData;                                // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->LifeFlags & 1))
            return true;
    }
    return false;
}
namespace NEntityEvents {
enum EEventType { EEventType_Force32 = 0x7fffffff };
class CEventBase {                                            // Ego_r.pdb: vptr + WorldFrame (8 bytes)
public:
    virtual ~CEventBase();                                    // slot 0
    virtual EEventType GetType() const;                       // slot 1
    long WorldFrame;                                          // +0x04
};
}
class CTCEntityEvents {                                       // interface 0x42; Ego_r.pdb Events after the CTCBase
public:
    enum { INTERFACE_TYPE = 0x42 };
    unsigned char _base[0x0c];                                // CTCBase (vptr, ParentThing, flags)
    std::list<CCountedPointer<NEntityEvents::CEventBase> > Events;   // +0x0C
};
class CSpawnedFuncBase {                                      // Ego_r.pdb (no STL member before +0x20)
public:
    unsigned char _pad00[0x18];
    long LastScriptFrame;                                     // +0x18
    long CurrentScriptFrame;                                  // +0x1C
};
class CScriptBase {                                           // Ego_r.pdb 84 bytes; retail PActiveThread +0x2C (PDB +60)
public:
    unsigned char _pad00[0x2c];
    CSpawnedFuncBase* PActiveThread;                          // +0x2C
};
class CQuestManager {                                         // Ego_r.pdb 276 bytes; retail PQuestCurrentlyRunning +0x88 (PDB +228)
public:
    class CActiveQuest {                                      // Ego_r.pdb
    public:
        long ID;                                              // +0x00
        void* PScriptInfo;                                    // +0x04 CScriptInfo*
        CCountedPointer<CScriptBase> PScript;                 // +0x08
    };
    unsigned char _pad00[0x88];
    CActiveQuest* PQuestCurrentlyRunning;                     // +0x88
};
extern CQuestManager* g_pQuestManager;                        // 0x013B89FC (reloc_bindings' ?g_pQuestManager@@3PAVCQuestManager@@A)
class CGameScriptThing {                                      // Ego_r.pdb 72 bytes
public:
    virtual ~CGameScriptThing();
    virtual bool MsgTalkedToAnyone() const;
    unsigned char _pad04[8];
    CIntelligentPointer<CThing> PThing;                       // +0x0C
};

bool CGameScriptThing::MsgTalkedToAnyone() const
{
    if (IS_THING_ALIVE(PThing)) {
        CQuestManager::CActiveQuest* quest = g_pQuestManager->PQuestCurrentlyRunning;
        long lastFrame = (quest && quest->PScript.Data) ? quest->PScript->PActiveThread->LastScriptFrame : 0;
        long currentFrame = (quest && quest->PScript.Data) ? quest->PScript->PActiveThread->CurrentScriptFrame : 0;
        CTCEntityEvents* events;
        if (PThing.GetPItem()->GetTC(&events)) {
            for (std::list<CCountedPointer<NEntityEvents::CEventBase> >::const_iterator it = events->Events.begin();
                 it != events->Events.end(); ++it) {
                const NEntityEvents::CEventBase* event = it->Data;
                if (event->GetType() == 0x21 && event->WorldFrame > lastFrame && event->WorldFrame <= currentFrame)
                    return true;
            }
        }
    }
    return false;
}