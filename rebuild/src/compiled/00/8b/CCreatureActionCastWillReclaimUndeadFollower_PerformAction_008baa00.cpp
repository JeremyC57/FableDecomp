// CCreatureAction_CastWillReclaimUndeadFollower::PerformAction() -- if the undead follower is alive, hand it back to the caster's special-abilities component (interface 0xa7) to reclaim
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BAA00, length 138 bytes, plain `ret`. Vtable slot of
//   CCreatureAction_CastWillReclaimUndeadFollower (vtable 0x0127D5BC, slot referenced from 0x0127D5C8).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020,
//          CTCSpecialAbilities::ReclaimUndeadFollower(CThingGameObject&) @ 0x0072B3B0 (Ego_r.pdb
//          `?ReclaimUndeadFollower@CTCSpecialAbilities@@QAEXAAVCThingGameObject@@@Z`).
// Shape: PUndeadFollower's address is taken once (edi); IS_THING_ALIVE inlined; the out-parameter GetInterface
//   template on PCreature (+8) for bit 0xa7 (`mov cl,[eax+34h] ; test cl,cl ; jns`), the out pointer left
//   uninitialised (the absent path reads the dead frame word into esi); the follower is re-fetched for the reference
//   argument after the lookup and the call is tail-duplicated into both paths.
// Layout: CCombatActionBase (retail 0x114 bytes, PCreature at +8); Ego_r.pdb puts PUndeadFollower at +0x12c (PDB base
//   0x12c), retail +0x114 (see the constructor 0x008BC5E0).
struct CTCBase;

struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    void Kill(bool removeNow);              // 0x004C9B80
    template <class T> __forceinline void GetInterface(T** ppOut, long interfaceType)   // leaves *ppOut alone when absent
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
};

struct CBaseIntelligentPointer {            // CIntelligentPointer<T>: vptr + item
    void*   __vftable;                      // +0x00
    CThing* m_pItem;                        // +0x04
    CThing* GetPItem() const;               // 0x00A01B50
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CThingGameObject;

struct CTCSpecialAbilities {                // interface type 0xa7 component
    void ReclaimUndeadFollower(CThingGameObject& follower);   // 0x0072B3B0
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];
};

struct CCreatureAction_CastWillReclaimUndeadFollower : CCombatActionBase {
    CBaseIntelligentPointer PUndeadFollower;   // +0x114 CIntelligentPointer<CThingGameObject>
    virtual void PerformAction();
};

void CCreatureAction_CastWillReclaimUndeadFollower::PerformAction()
{
    if (IS_THING_ALIVE(PUndeadFollower)) {
        CTCSpecialAbilities* abilities;
        PCreature.GetPItem()->GetInterface(&abilities, 0xa7);
        abilities->ReclaimUndeadFollower(*(CThingGameObject*)PUndeadFollower.GetPItem());
    }
}