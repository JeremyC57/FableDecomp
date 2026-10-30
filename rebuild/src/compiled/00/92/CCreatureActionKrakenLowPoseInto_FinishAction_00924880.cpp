// CCreatureAction_KrakenLowPoseInto::FinishAction() -- base FinishAction, then make the low charge loop (loop 0) the owner's current action while the target is alive, else the low attack "out of" action
// Leaf: ?FinishAction@
// Grade: RELOCATION_MATCH -- retry wave 2 (sec4): the target's dead test through CThing::IsDead() const (Ego_r.pdb
//   ?IsDead@CThing@@QBE_NXZ, a bool over the +0x91 bit) resolves the &PTarget edi / ebx register tie (shrink-wrapped push ebx).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00924880, length 172 bytes (+ int3 padding), vtable slot 4 of 0x01290854
// (RTTI `?A0x29922355::CCreatureAction_KrakenLowPoseInto`; FableWin.pdb ?FinishAction@CCreatureAction_KrakenLowPoseInto@?A0xe2035276@@UAEXXZ).
// Callees: CCombatActionBase::FinishAction @ 0x00859A20 (first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PTarget at +0xa8
// in edi, PCreature at +8), CCreatureAction_KrakenLowChargeLoop::CCreatureAction_KrakenLowChargeLoop(CThingCreatureBase&, CThing&,
// long) @ 0x009254C0 (landed), CCreatureAction_KrakenLowAttackOutOf::CCreatureAction_KrakenLowAttackOutOf(CThingCreatureBase&)
// @ 0x00925680 (landed), CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CCombatActionBase @ 0x0062E900.
// Shape: as 0x00924A90 with a three-argument loop constructor (one `push 0`; ebx is pushed only inside the alive arm).
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
    unsigned char Dead : 1;                     // +0x91 bit 0
    bool IsDead() const { return Dead; }        // Ego_r.pdb ?IsDead@CThing@@QBE_NXZ
    unsigned char _pad92[0xb4 - 0x92];
    float         m_health;                     // +0xb4
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
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
struct CBaseIntelligentPointer {
    virtual void v00();
    void*   m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    virtual void FinishAction();                // 0x00859A20 (slot 4)
    unsigned char           _pad04[4];          // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;            // +0xa8 CIntelligentPointer<CThing> (Ego_r.pdb +0xb4)
    unsigned char           _padb0[0x114 - 0xb0];
    ~CCombatActionBase();                       // 0x0062E900
    __forceinline CThingCreatureBase& Creature() const { return *static_cast<CThingCreatureBase*>(PCreature.GetPItem()); }
    __forceinline bool IsTargetAlive() const    // two fetches: present, then not dead
    {
        return PTarget.GetPItem() && !PTarget.GetPItem()->IsDead();
    }
};
struct CCreatureAction_KrakenLowChargeLoop : CCombatActionBase {    // vtable 0x012909C4
    long  LoopNum;                              // +0x114 (PDB +0x12c)
    float StartHealth;                          // +0x118 (PDB +0x130)
    CCreatureAction_KrakenLowChargeLoop(CThingCreatureBase& creature, CThing& target, long loopNum);   // 0x009254C0
};
struct CCreatureAction_KrakenLowAttackOutOf : CCombatActionBase {   // vtable 0x01290DBC
    CCreatureAction_KrakenLowAttackOutOf(CThingCreatureBase& creature);   // 0x00925680
};
struct CCreatureAction_KrakenLowPoseInto : CCombatActionBase {      // vtable 0x01290854
    virtual void FinishAction();
};

void CCreatureAction_KrakenLowPoseInto::FinishAction()
{
    CCombatActionBase::FinishAction();
    if (IsTargetAlive())
        Creature().SetCurrentAction(CCreatureAction_KrakenLowChargeLoop(Creature(), *PTarget.GetPItem(), 0));
    else
        Creature().SetCurrentAction(CCreatureAction_KrakenLowAttackOutOf(Creature()));
}