// CCreatureAction_KrakenHighAttackInto::FinishAction() -- base FinishAction, then make the high attack loop (loop 0) the owner's current action while the target is alive, else the high attack "out of" action
// Leaf: ?FinishAction@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00924A90, length 175 bytes (+ int3 padding), vtable slot 4 of 0x01290F1C
// (RTTI `?A0x29922355::CCreatureAction_KrakenHighAttackInto`; FableWin.pdb ?FinishAction@CCreatureAction_KrakenHighAttackInto@?A0xe2035276@@UAEXXZ).
// Callees: CCombatActionBase::FinishAction @ 0x00859A20 (first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PTarget at +0xa8
// in ebx, PCreature at +8), CCreatureAction_KrakenHighAttackLoop::CCreatureAction_KrakenHighAttackLoop(CThingCreatureBase&, CThing&,
// long, bool) @ 0x00925780 (landed; FableWin.pdb ??0...@QAE@AAVCThingCreatureBase@@AAVCThing@@J_N@Z),
// CCreatureAction_KrakenHighAttackOutOf::CCreatureAction_KrakenHighAttackOutOf(CThingCreatureBase&) @ 0x00925820 (landed),
// CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CCombatActionBase @ 0x0062E900 (manifest "~CTCLightningOrb" is propagated).
// Shape: an inline two-fetch alive test on PTarget (`IsValid`), each arm builds its 0x11c / 0x114-byte temporary at the frame base
// inside the SetCurrentAction call and destroys it through the base destructor.
struct CTCBase;
struct CInterfaceMap {                          // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
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
struct CThingCreatureBase;
struct CCombatActionBase;
struct CThingCreatureBase : CThing {
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void*   m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CCombatActionBase {                      // retail sizeof 0x114
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
        return PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1);
    }
};
struct CCreatureAction_KrakenHighAttackLoop : CCombatActionBase {    // vtable 0x0129107C
    bool Interrupt;                             // +0x114 (PDB +0x12c)
    long LoopNum;                               // +0x118 (PDB +0x130)
    CCreatureAction_KrakenHighAttackLoop(CThingCreatureBase& creature, CThing& target, long loopNum, bool interrupt);   // 0x00925780
};
struct CCreatureAction_KrakenHighAttackOutOf : CCombatActionBase {   // vtable 0x012911DC
    CCreatureAction_KrakenHighAttackOutOf(CThingCreatureBase& creature);   // 0x00925820
};
struct CCreatureAction_KrakenHighAttackInto : CCombatActionBase {    // vtable 0x01290F1C
    virtual void FinishAction();
};

void CCreatureAction_KrakenHighAttackInto::FinishAction()
{
    CCombatActionBase::FinishAction();
    if (IsTargetAlive())
        Creature().SetCurrentAction(CCreatureAction_KrakenHighAttackLoop(Creature(), *PTarget.GetPItem(), 0, false));
    else
        Creature().SetCurrentAction(CCreatureAction_KrakenHighAttackOutOf(Creature()));
}