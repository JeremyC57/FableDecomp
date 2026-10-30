// CAIStateGroup_SellToBuyer::GetBuyersOrder() -- the newest message of type 0x13 (buy order) targetted at this group's creature
// Leaf: ?GetBuyersOrder@CAIStateGroup_SellToBuyer@@AAEPBVCMessageEvent@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094BFC0, 69 bytes (+ int3 padding), thiscall, `ret`.
// Identity (proven): label correction -- the row is labelled CAIStateGroup_MinionReactToPlayerInteraction::FindPlayerInteractionEvent;
//   Ego_r.pdb donor order puts ?GetBuyersOrder@CAIStateGroup_SellToBuyer@@AAEPBVCMessageEvent@@XZ here and its sole caller is
//   CAIStateGroup_SellToBuyer::IsRunnable 0x0094C010 (the twin 0x0092EDC0 uses type 0x14).
// Callees: CMessageEventFilter_IsTargettedAtThing::CMessageEventFilter_IsTargettedAtThing(const CThing&) 0x006AF290,
//   CAIStateGroupBase::PeekWorld() const 0x008FCFA0, CMessageEventManager::FindMostRecentMessage
//   <CFilter_Bind2<CMessageEventFilter_IsOfType,CMessageEventFilter_IsTargettedAtThing> >(const ...&) const 0x008D46D0.
// Shape: creature local, the bound filter as a named local (IsOfType(0x13) && IsTargettedAtThing(*creature)), a named
//   manager local, plus the same-TU FindMostRecentMessage stand-in: retail's F1-before-ctor store and eax manager load.
// ---- shared AI state-group model (retail STLport layout; the generated engine headers are flat Ego_r layouts) ----
// CAIStateGroupBase (RTTI: : CBaseClass): vptr +0, PBrain +4, ScriptAIPriority +8, Name +0xC, Terminated +0x10, Priority
// +0x11, Enabled +0x12 (0x14 bytes). CAIBrain::Creature +0x18 (engine/CAIBrain.h agrees). CThing (retail): UsingComponents
// bitset<274> +0x20, Components CVectorMap +0x44, Dead bit 0 of +0x91 (Ego_r +0xA1 minus 0x10: bitset, CVectorMap, list).
// Getter identities (Ego_r.pdb donor order = retail order 0x008FCF40..0x008FD010): Terminate 0x008FCF40, PeekWorld
// 0x008FCFA0, GetWorld 0x008FCFC0, GetPWorldSeed 0x008FCFD0, PeekThingSearchTools 0x008FD000, IsStopping 0x008FD010.
#include "engine/CAIBrain.h"                           // Creature +0x18
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseObject : public CBaseClass { };
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
private:
    void* PData;                                        // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
};
template <class T> class CCountedPointer {
public:
    T* Data;                                            // +0x00
    T* operator->() const { return Data; }
};
class CClass : public CBaseObject { };
class CThing : public CClass {
public:
    unsigned char _pad04[0x1C];
    unsigned long UsingComponents[9];                   // +0x20 bitset<274>
    unsigned char Components[0x10];                     // +0x44 CVectorMap<ETCInterfaceType, CTCBase*, ...>
    unsigned char _pad54[0x91 - 0x54];
    unsigned char Dead : 1;                             // +0x91 bit 0
};
class CThingPhysical : public CThing { };
class CThingGameObject : public CThingPhysical { };
class CThingCreatureBase : public CThingGameObject {
public:
    bool IsPerformingAnyAction() const;                 // 0x006622A0
};
class CThingAICreature : public CThingCreatureBase { };
class CThingObject : public CThingGameObject { };
class CThingBuilding;

#include "engine/CWorld.h"                            // PThingCombatManager_ptr +0x4C, PMessageEventManager_Data +0x60
class CThingSearchTools;
class CAIStateGroupBase : public CBaseClass {
public:
    CAIBrain*     PBrain;                               // +0x04
    long          ScriptAIPriority;                     // +0x08
    CCharString   Name;                                 // +0x0C
    bool          Terminated;                           // +0x10
    unsigned char Priority;                             // +0x11
    bool          Enabled;                              // +0x12
    void Terminate();                                   // 0x008FCF40
protected:
    const CWorld& PeekWorld() const;                    // 0x008FCFA0
    CWorld& GetWorld() const;                           // 0x008FCFC0
    unsigned long* GetPWorldSeed() const;               // 0x008FCFD0
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000
    bool IsStopping() const;                            // 0x008FD010
};
// ---- message-filter model (Ego_r.pdb layouts: IsOfType {Type} 4 bytes, IsTargettedAtThing {UID} 8 bytes,
// CFilter_Bind2 {F1, F2} pointers); the filters chain with an inline operator&& (the landed 0x00939D70 shape).
class CMessageEvent;
class CMessageEventFilter_IsOfType {
public:
    bool operator()(const CMessageEvent& message) const;   // 0x006AF1C0
    long Type;                                          // +0x00 EMessageEventType
    CMessageEventFilter_IsOfType(long type) : Type(type) { }
};
class CMessageEventFilter_IsTargettedAtThing {
public:
    bool operator()(const CMessageEvent& message) const;   // 0x006AF1E0
    unsigned __int64 UID;                               // +0x00
    CMessageEventFilter_IsTargettedAtThing(const CThing& thing);   // 0x006AF290
};
template <class A, class B> class CFilter_Bind2 {
public:
    const A* F1;                                        // +0x00
    const B* F2;                                        // +0x04
    CFilter_Bind2(const A& a, const B& b) : F1(&a), F2(&b) { }
    bool operator()(const CMessageEvent& message) const { return (*F1)(message) && (*F2)(message); }
};
template <class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
// The FindMostRecentMessage instantiation this TU uses is retail 0x008D46D0 (landed): its body is compiled here as retail
// compiled it (a template is instantiated in its caller's TU), so the relocation binds to the real symbol and nothing
// invented stands in. The list walk reads the bound filter's member pointers through the const reference, which keeps
// VC7.1's store of the Bind first word before the filter constructor call (the chunk 517 same-TU rule, LESSONS.md).
class CMessageEvent { public: unsigned char _body[0x58]; };     // Ego_r.pdb, 88 bytes (only its address is used here)
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {     // STLport list, 4 bytes
public:
    struct const_iterator {
        _List_node_base* _M_node;
        const_iterator(_List_node_base* __x) : _M_node(__x) { }
        const _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
        const_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const const_iterator& __y) const { return _M_node != __y._M_node; }
    };
    _STLP_alloc_proxy<_List_node<_Tp>*, allocator<_List_node<_Tp> > > _M_node;   // +0x00
    const_iterator begin() const { return const_iterator(_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(_M_node._M_data); }
};
}
class CMessageEventManager {                             // Ego_r.pdb 16 bytes; retail 8 (4-byte STLport list)
public:
    CWorld* World;                                       // +0x00
    std::list<CMessageEvent> Messages;                   // +0x04
    template <class TFilter> const CMessageEvent* FindMostRecentMessage(const TFilter& filter) const;
};
template <class TFilter> const CMessageEvent* CMessageEventManager::FindMostRecentMessage(const TFilter& filter) const
{
    for (std::list<CMessageEvent>::const_iterator it = Messages.begin(); it != Messages.end(); ++it) {
        const CMessageEvent* message = &*it;
        if (filter(*message))
            return message;
    }
    return 0;
}
class CAIStateGroup_SellToBuyer : public CAIStateGroupBase {
private:
    const CMessageEvent* GetBuyersOrder();
};

const CMessageEvent* CAIStateGroup_SellToBuyer::GetBuyersOrder()
{
    const CThingAICreature* creature = PBrain->Creature;
    CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_IsTargettedAtThing> filter =
        CMessageEventFilter_IsOfType(0x13) && CMessageEventFilter_IsTargettedAtThing(*creature);
    const CMessageEventManager* manager = PeekWorld().PMessageEventManager_Data;
    return manager->FindMostRecentMessage(filter);
}