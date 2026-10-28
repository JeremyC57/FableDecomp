// CAIStateGroup_MinionReactToCrime::GetSomeoneElseGettingHelpMessage(long extraData) -- the newest type-0xA message from a living originator other than this group's creature carrying the given extra data
// Leaf: ?GetSomeoneElseGettingHelpMessage@CAIStateGroup_MinionReactToCrime@@AAEPBVCMessageEvent@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00958770, 95 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?GetSomeoneElseGettingHelpMessage@CAIStateGroup_MinionReactToCrime@@AAEPBVCMessageEvent@@J@Z
//   (Ego_r 0x0097D370); the label agrees and the callee 0x00959040 is the landed FindMostRecentMessage<CFilter_Bind4<IsOfType,
//   IsOriginatorAlive, std::unary_negate<IsOriginatedByThing>, HasExtraData> > (Ego_r.pdb layouts of all four filters).
// Callees: CAIStateGroupBase::PeekWorld() const 0x008FCFA0, CMessageEventManager::FindMostRecentMessage<CFilter_Bind4<...> >
//   (const ...&) const 0x00959040. The IsOriginatedByThing ctor (out of line elsewhere at 0x006AF2B0) is inlined here:
//   the creature's UID (+0x18/+0x1C) is copied straight into the negated filter.
// Shape: the whole chain is written inline in the call (its temporaries: type +0, extra data +4, UID +8, Bind4 +0x10; the
//   empty IsOriginatorAlive temporary is homed in the dead `extraData` slot), the manager is reached through an inline
//   accessor (stand-in for CCountedPointer<CMessageEventManager>::operator-> on the flat engine/CWorld.h member: its
//   load follows the filter stores and goes through eax), plus the same-TU FindMostRecentMessage stand-in.
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
class CBaseClass {
public:
    virtual ~CBaseClass();
};
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
    unsigned char _pad04[0x14];
    unsigned __int64 UID;                               // +0x18
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
    long Type;                                          // +0x00 EMessageEventType
    CMessageEventFilter_IsOfType(long type) : Type(type) { }
};
class CMessageEventFilter_IsTargettedAtThing {
public:
    unsigned __int64 UID;                               // +0x00
    CMessageEventFilter_IsTargettedAtThing(const CThing& thing);   // 0x006AF290
};
template <class A, class B> class CFilter_Bind2 {
public:
    const A* F1;                                        // +0x00
    const B* F2;                                        // +0x04
    CFilter_Bind2(const A& a, const B& b) : F1(&a), F2(&b) { }
};
template <class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
class CMessageEventFilter_IsOriginatorAlive {          // empty (1 byte): a user ctor, so the slot takes no store
public:
    CMessageEventFilter_IsOriginatorAlive() { }
};
template <class A, class B, class C> class CFilter_Bind3 {
public:
    const A* F1;                                        // +0x00
    const B* F2;                                        // +0x04
    const C* F3;                                        // +0x08
    CFilter_Bind3(const A& a, const B& b, const C& c) : F1(&a), F2(&b), F3(&c) { }
};
template <class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(*ab.F1, *ab.F2, c); }
class CMessageEventFilter_IsOriginatedByThing {
public:
    typedef CMessageEvent argument_type;
    unsigned __int64 UID;                               // +0x00
    CMessageEventFilter_IsOriginatedByThing(const CThing& thing) : UID(thing.UID) { }   // out of line elsewhere: 0x006AF2B0
};
class CMessageEventFilter_HasExtraData {
public:
    long ExtraData;                                     // +0x00
    CMessageEventFilter_HasExtraData(long extraData) : ExtraData(extraData) { }
};
namespace std {
template <class Predicate> class unary_negate {       // STLport: the predicate held by value
protected:
    Predicate _M_pred;
public:
    explicit unary_negate(const Predicate& x) : _M_pred(x) { }
};
template <class Predicate> inline unary_negate<Predicate> not1(const Predicate& pred) { return unary_negate<Predicate>(pred); }
}
template <class A, class B, class C, class D> class CFilter_Bind4 {
public:
    const A* F1;                                        // +0x00
    const B* F2;                                        // +0x04
    const C* F3;                                        // +0x08
    const D* F4;                                        // +0x0C
    CFilter_Bind4(const A& a, const B& b, const C& c, const D& d) : F1(&a), F2(&b), F3(&c), F4(&d) { }
};
template <class A, class B, class C, class D> inline CFilter_Bind4<A, B, C, D> operator&&(const CFilter_Bind3<A, B, C>& abc, const D& d) { return CFilter_Bind4<A, B, C, D>(*abc.F1, *abc.F2, *abc.F3, d); }
class CMessageEventManager {
public:
    template <class TFilter> const CMessageEvent* FindMostRecentMessage(const TFilter& filter) const;
};
// Same-TU stand-in for the FindMostRecentMessage instantiation (a template body is compiled in its caller's TU; the
// retail instantiation is out of line and is not graded here): it reads the manager and the bound filter's first
// word through the const reference (the landed 0x009226B0 stand-in), so VC7.1 stores the Bind2 first pointer before
// the filter constructor call and loads the manager into eax as retail does. The sink is outside the graded bytes.
long FindMostRecentMessage_Sink(const void* manager, long first);
#pragma auto_inline(off)
template <class TFilter> __declspec(noinline) const CMessageEvent* CMessageEventManager::FindMostRecentMessage(const TFilter& filter) const
{
    long r = FindMostRecentMessage_Sink(this, 0 + *(const long*)&filter);
    return (const CMessageEvent*)r;
}
#pragma auto_inline(on)
// Inline stand-in for the CCountedPointer<CMessageEventManager>::operator-> that engine/CWorld.h flattens to a raw pointer.
inline const CMessageEventManager* MessageEventManagerOf(const CWorld& world) { return world.PMessageEventManager_Data; }

class CAIStateGroup_MinionReactToCrime : public CAIStateGroupBase {
private:
    const CMessageEvent* GetSomeoneElseGettingHelpMessage(long extraData);
};

const CMessageEvent* CAIStateGroup_MinionReactToCrime::GetSomeoneElseGettingHelpMessage(long extraData)
{
    return MessageEventManagerOf(PeekWorld())->FindMostRecentMessage(
        CMessageEventFilter_IsOfType(0xA) && CMessageEventFilter_IsOriginatorAlive()
        && std::not1(CMessageEventFilter_IsOriginatedByThing(*PBrain->Creature)) && CMessageEventFilter_HasExtraData(extraData));
}
