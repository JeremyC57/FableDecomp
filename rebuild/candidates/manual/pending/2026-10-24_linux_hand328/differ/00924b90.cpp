// Grade: DIFFER(219v219) -- same length; retail increments LoopNum in esi (this in edi, kraken in ecx) and loads PKrakenDef AFTER the store; ours (named def local) loads the def before the inc and swaps esi/edi. Without the local: 215v219 (compare folded into one cmp, tails shared).
// CCreatureAction_KrakenRoarLoop::FinishAction() -- the creature-action base FinishAction, count the loop, then queue another roar loop while fewer than CKrakenDef::RoarLoops have played, else the roar "out of" action
// Leaf: ?FinishAction@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00924B90, length 219 bytes (+ int3 padding), vtable slot 4 of 0x01291494
// (RTTI `?A0x29922355::CCreatureAction_KrakenRoarLoop`; FableWin.pdb ?FinishAction@CCreatureAction_KrakenRoarLoop@?A0xe2035276@@UAEXXZ).
// Callees: CCreatureActionBase::FinishAction @ 0x00694430 (first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8,
// address CSE'd in ebx), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xe2 = CTCKraken, bit 2 of the byte at +0x3c),
// CCreatureAction_KrakenRoarLoop::CCreatureAction_KrakenRoarLoop(CThingCreatureBase&, long) @ 0x00925920,
// CCreatureAction_KrakenRoarOutOf::CCreatureAction_KrakenRoarOutOf(CThingCreatureBase&) @ 0x009259B0 (landed),
// CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CCombatActionBase @ 0x0062E900.
// Shape: the UNCHECKED kraken accessor (absent arm reads the key word into ecx); `++LoopNum` keeps the new count in esi for the
// compare against PKrakenDef->RoarLoops, and the loop arm re-reads the member for the constructor argument.
// Layout: CTCKraken PKrakenDef +0x0c (CDefPointer<const CKrakenDef>); CKrakenDef RoarLoops +0x48 (Ego_r.pdb, same offset).
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

struct CCreatureActionBase {                    // retail sizeof 0xa8 (Ego_r.pdb 0xb4)
    virtual void v00();
    virtual void FinishAction();                // 0x00694430 (slot 4)
    unsigned char           _pad04[4];          // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0xa8 - 0x10];
    __forceinline CThingCreatureBase& Creature() const { return *static_cast<CThingCreatureBase*>(PCreature.GetPItem()); }
};
struct CCombatActionBase : CCreatureActionBase {   // retail sizeof 0x114
    CBaseIntelligentPointer PTarget;            // +0xa8 CIntelligentPointer<CThing>
    unsigned char           _padb0[0x114 - 0xb0];
    ~CCombatActionBase();                       // 0x0062E900
};
struct CKrakenDef {
    unsigned char _pad00[0x48];
    long          RoarLoops;                    // +0x48
};
struct CTCKraken {                              // interface type 0xe2 component
    unsigned char     _pad00[0x0c];
    const CKrakenDef* PKrakenDef;               // +0x0c CDefPointer<const CKrakenDef>
};
struct CCreatureAction_KrakenRoarLoop : CCombatActionBase {   // vtable 0x01291494
    long LoopNum;                               // +0x114 (PDB +0x12c)
    CCreatureAction_KrakenRoarLoop(CThingCreatureBase& creature, long loopNum);   // 0x00925920
    virtual void FinishAction();
    __forceinline CTCKraken* GetKraken() const
    {
        CTCKraken* kraken;
        PCreature.GetPItem()->GetInterface(&kraken, 0xe2);
        return kraken;
    }
};
struct CCreatureAction_KrakenRoarOutOf : CCombatActionBase {  // vtable 0x012915EC
    CCreatureAction_KrakenRoarOutOf(CThingCreatureBase& creature);   // 0x009259B0
};

void CCreatureAction_KrakenRoarLoop::FinishAction()
{
    CCreatureActionBase::FinishAction();
    CTCKraken* kraken = GetKraken();
    const CKrakenDef* def = kraken->PKrakenDef;
    if (++LoopNum < def->RoarLoops)
        Creature().SetCurrentAction(CCreatureAction_KrakenRoarLoop(Creature(), LoopNum));
    else
        Creature().SetCurrentAction(CCreatureAction_KrakenRoarOutOf(Creature()));
}
