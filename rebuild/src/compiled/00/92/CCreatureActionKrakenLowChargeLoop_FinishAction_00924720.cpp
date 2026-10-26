// CCreatureAction_KrakenLowChargeLoop::FinishAction() -- base FinishAction; while the target is alive and the kraken has not been hurt during boss phase 2, run the next low charge loop (or the low attack once the def's loop count is reached), otherwise the low attack "out of" action
// Leaf: ?FinishAction@CCreatureAction_KrakenLowChargeLoop
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00924720, 347 bytes (+ int3 padding), vtable slot 4 of 0x012909C4 (RTTI
//   `?A0x29922355::CCreatureAction_KrakenLowChargeLoop`; FableWin.pdb
//   ?FinishAction@CCreatureAction_KrakenLowChargeLoop@?A0xe2035276@@UAEXXZ). Catalog "vfunc_4".
// Callees: CCombatActionBase::FinishAction @ 0x00859A20 (first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature
//   +8, PTarget +0xa8), CVectorMap::LowerBound @ 0x0040F020 (the inline unchecked GetKraken(), interface 0xe2 = CTCKraken),
//   CCreatureAction_KrakenLowChargeLoop(CThingCreatureBase*, CThing*, long) @ 0x009254C0, CCreatureAction_KrakenLowAttackInto
//   (CThingCreatureBase*, CThing*) @ 0x00925560, CCreatureAction_KrakenLowAttackOutOf(CThingCreatureBase&) @ 0x00925680 (all
//   landed), CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CCombatActionBase @ 0x0062E900.
// Layout: the landed FrameUpdate 0x00924180 (LoopNum +0x114, StartHealth +0x118, CTCKraken Phase +0x60); the kraken's def
//   pointer at +0xC holds the low charge loop count at +0x3C.
// Shape: the landed siblings 0x00924670 / 0x00924A90 (two-fetch alive test, each arm builds its temporary at the frame
//   base inside the SetCurrentAction call; the charge loop fetches its target into a local first, as retail's fetch order
//   shows); the hurt test is the FrameUpdate one (phase first, then `health < StartHealth`).
class CTCBase;
struct CInterfaceMap {                          // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
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
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
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
class CKrakenDef { public:
    unsigned char _pad00[0x3c];
    long          LowChargeLoops;               // +0x3C (descriptive)
};
class CTCKraken { public:                              // interface type 0xe2 component
    unsigned char     _pad00[0x0c];
    const CKrakenDef* Def;                      // +0x0C
    unsigned char     _pad10[0x60 - 0x10];
    long              Phase;                    // +0x60 EKrakenBossPhase (PDB +0x70)
};

class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void FinishAction();                // 0x00859A20 (slot 4)
    unsigned char           _pad04[4];          // +0x04
    CIntelligentPointer<CThing> PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0xa8 - 0x10];
    CIntelligentPointer<CThing> PTarget;            // +0xa8 CIntelligentPointer<CThing>
    unsigned char           _padb0[0x114 - 0xb0];
    virtual ~CCombatActionBase();                       // 0x0062E900
    __forceinline CThingCreatureBase& Creature() const { return *static_cast<CThingCreatureBase*>(PCreature.GetPItem()); }
    __forceinline bool IsTargetAlive() const    // two fetches: present, then not dead
    {
        return PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1);
    }
    __forceinline CTCKraken* GetKraken() const
    {
        CTCKraken* kraken;
        PCreature.GetPItem()->GetInterface(&kraken, 0xe2);
        return kraken;
    }
};
struct CCreatureAction_KrakenLowAttackInto : CCombatActionBase {    // vtable 0x01290B0C
    CCreatureAction_KrakenLowAttackInto(CThingCreatureBase* owner, CThing* thing);   // 0x00925560
};
struct CCreatureAction_KrakenLowAttackOutOf : CCombatActionBase {   // vtable 0x01290DBC
    CCreatureAction_KrakenLowAttackOutOf(CThingCreatureBase& creature);   // 0x00925680
};
struct CCreatureAction_KrakenLowChargeLoop : CCombatActionBase {   // vtable 0x012909C4
    long  LoopNum;                              // +0x114 (PDB +0x12c)
    float StartHealth;                          // +0x118 (PDB +0x130)
    CCreatureAction_KrakenLowChargeLoop(CThingCreatureBase* owner, CThing* thing, long loopNum);   // 0x009254C0
    virtual void FinishAction();
};

void CCreatureAction_KrakenLowChargeLoop::FinishAction()
{
    CCombatActionBase::FinishAction();
    CTCKraken* kraken = GetKraken();
    if (IsTargetAlive() && !(kraken->Phase == 2 && PCreature.GetPItem()->m_health < StartHealth)) {
        ++LoopNum;
        if (LoopNum < kraken->Def->LowChargeLoops) {
            CThing* target = PTarget.GetPItem();
            Creature().SetCurrentAction(CCreatureAction_KrakenLowChargeLoop(&Creature(), target, LoopNum));
        } else
            Creature().SetCurrentAction(CCreatureAction_KrakenLowAttackInto(&Creature(), PTarget.GetPItem()));
    } else
        Creature().SetCurrentAction(CCreatureAction_KrakenLowAttackOutOf(Creature()));
}