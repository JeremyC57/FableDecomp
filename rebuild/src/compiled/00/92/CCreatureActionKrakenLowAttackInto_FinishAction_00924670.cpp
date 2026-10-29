// CCreatureAction_KrakenLowAttackInto::FinishAction() -- base FinishAction, then make the low attack loop (loop 0) the owner's current action while the target is alive, else the low attack "out of" action
// Leaf: ?FinishAction@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00924670, length 175 bytes (+ int3 padding), vtable slot 4 of 0x01290B0C
// (RTTI `?A0x29922355::CCreatureAction_KrakenLowAttackInto`; FableWin.pdb ?FinishAction@CCreatureAction_KrakenLowAttackInto@?A0xe2035276@@UAEXXZ).
// Callees: CCombatActionBase::FinishAction @ 0x00859A20 (first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PTarget at +0xa8
// in ebx, PCreature at +8), CCreatureAction_KrakenLowAttackLoop::CCreatureAction_KrakenLowAttackLoop(CThingCreatureBase&, CThing&,
// long, bool) @ 0x009255E0 (landed), CCreatureAction_KrakenLowAttackOutOf::CCreatureAction_KrakenLowAttackOutOf(CThingCreatureBase&)
// @ 0x00925680 (landed), CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CCombatActionBase @ 0x0062E900.
// Shape: the twin of CCreatureAction_KrakenHighAttackInto::FinishAction 0x00924A90 -- an inline two-fetch alive test on PTarget,
// each arm builds its temporary at the frame base inside the SetCurrentAction call.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // a thing's sorted {type, component} vector map at +0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char _pad92[0xb4 - 0x92];
    float         m_health;                     // +0xb4
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase;
class CCombatActionBase;
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    virtual void FinishAction();                // 0x00859A20 (slot 4)
    unsigned char           _pad04[4];          // +0x04
    CIntelligentPointer<CThing> PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0xa8 - 0x10];
    CIntelligentPointer<CThing> PTarget;            // +0xa8 CIntelligentPointer<CThing> (Ego_r.pdb +0xb4)
    unsigned char           _padb0[0x114 - 0xb0];
    virtual ~CCombatActionBase();                       // 0x0062E900
    __forceinline CThingCreatureBase& Creature() const { return *static_cast<CThingCreatureBase*>(PCreature.GetPItem()); }
    __forceinline bool IsTargetAlive() const    // two fetches: present, then not dead
    {
        return PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1);
    }
};
struct CCreatureAction_KrakenLowAttackLoop : CCombatActionBase {    // vtable 0x01290C64
    bool Interrupt;                             // +0x114 (PDB +0x12c)
    long LoopNum;                               // +0x118 (PDB +0x130)
    CCreatureAction_KrakenLowAttackLoop(CThingCreatureBase& creature, CThing& target, long loopNum, bool interrupt);   // 0x009255E0
};
struct CCreatureAction_KrakenLowAttackOutOf : CCombatActionBase {   // vtable 0x01290DBC
    CCreatureAction_KrakenLowAttackOutOf(CThingCreatureBase& creature);   // 0x00925680
};
struct CCreatureAction_KrakenLowAttackInto : CCombatActionBase {    // vtable 0x01290B0C
    virtual void FinishAction();
};

void CCreatureAction_KrakenLowAttackInto::FinishAction()
{
    CCombatActionBase::FinishAction();
    if (IsTargetAlive())
        Creature().SetCurrentAction(CCreatureAction_KrakenLowAttackLoop(Creature(), *PTarget.GetPItem(), 0, false));
    else
        Creature().SetCurrentAction(CCreatureAction_KrakenLowAttackOutOf(Creature()));
}