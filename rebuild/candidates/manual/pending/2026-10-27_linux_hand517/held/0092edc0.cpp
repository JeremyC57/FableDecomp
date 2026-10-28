// CAIStateGroup_MinionReactToPlayerInteraction::FindPlayerInteractionEvent() const -- the newest message of type 0x14 (player interaction) targetted at this group's creature
// Leaf: ?FindPlayerInteractionEvent@CAIStateGroup_MinionReactToPlayerInteraction@@ABEPBVCMessageEvent@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092EDC0, 69 bytes (+ int3 padding), thiscall, `ret`.
// Identity (proven): Ego_r.pdb names ?FindPlayerInteractionEvent@CAIStateGroup_MinionReactToPlayerInteraction@@ABEPBVCMessageEvent@@XZ;
//   the label agrees; the body is the twin of 0x0094BFC0 (type 0x13) with type 0x14.
// Callees: CMessageEventFilter_IsTargettedAtThing::CMessageEventFilter_IsTargettedAtThing(const CThing&) 0x006AF290,
//   CAIStateGroupBase::PeekWorld() const 0x008FCFA0, CMessageEventManager::FindMostRecentMessage
//   <CFilter_Bind2<CMessageEventFilter_IsOfType,CMessageEventFilter_IsTargettedAtThing> >(const ...&) const 0x008D46D0.
// Shape: creature local, the bound filter as a named local (IsOfType(0x14) && IsTargettedAtThing(*creature)), a named
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
class CAIStateGroup_MinionReactToPlayerInteraction : public CAIStateGroupBase {
private:
    const CMessageEvent* FindPlayerInteractionEvent() const;
};

const CMessageEvent* CAIStateGroup_MinionReactToPlayerInteraction::FindPlayerInteractionEvent() const
{
    const CThingAICreature* creature = PBrain->Creature;
    CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_IsTargettedAtThing> filter =
        CMessageEventFilter_IsOfType(0x14) && CMessageEventFilter_IsTargettedAtThing(*creature);
    const CMessageEventManager* manager = PeekWorld().PMessageEventManager_Data;
    return manager->FindMostRecentMessage(filter);
}
