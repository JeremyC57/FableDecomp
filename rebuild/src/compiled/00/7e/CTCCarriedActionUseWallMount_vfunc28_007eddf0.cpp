// CTCCarriedActionUseWallMount::vfunc_28(CThingCreatureBase* user) -- run the base use hook; a live user declares the holder a criminal, otherwise the holder's interface-0x1a component is notified
//
// Retail 0x007EDDF0, 158 bytes (`ret 4`). Vtable 0x0123FE6C (CTCCarriedActionUseWallMount) slot 28.
// Callees: the base hook @ 0x007EAB90 (empty `ret 4` in another TU),
// CCreatureAction_DeclarePlayerACriminal::CCreatureAction_DeclarePlayerACriminal(CThingCreatureBase*,
// CThingPlayerCreature*) @ 0x00843610, CThingCreatureBase::SetCurrentAction @ 0x006644F0,
// ~CBaseIntelligentPointer @ 0x00A01C10, ~CCreatureActionBase @ 0x00693EF0,
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, the interface-0x1a component's
// member @ 0x005D8E50 (manifest "CChunkCollectionIndicator::OnPredicateFail", propagated; no arguments).
// Shape: as the landed-shape UseFish 0x007EDCD0 (IsThingAlive on the raw pointer, the holder a named
// local in edi, the 0xb0-byte action a temporary); the other arm tests the holder's bit 26 of
// bitset word 0 (`test dword [eax+20h],4000000h`) and calls the component straight off the
// GetInterface result (`mov ecx,[eax+4]`).
struct CTCBase;
struct CCreatureActionBase;
struct CThingPlayerCreature;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

struct CTCInterface1A {
    void Notify();                                          // 0x005D8E50
};

struct CThingCreatureBase {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_stateFlags;                             // +0x91 (bit 0 = dead)
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetInterface(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    bool SetCurrentAction(CCreatureActionBase* action);     // 0x006644F0
};

inline bool IsThingAlive(const CThingCreatureBase* thing)
{
    return thing && !(thing->m_stateFlags & 1);
}

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                  // +0x04
    ~CBaseIntelligentPointer();                     // 0x00A01C10
};
template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer { };

struct CCreatureActionBase {                        // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 0x04];
    ~CCreatureActionBase();                         // 0x00693EF0
};
struct CCreatureAction_DeclarePlayerACriminal : CCreatureActionBase {
    CIntelligentPointer<CThingPlayerCreature> PCriminal;   // +0xa8
    CCreatureAction_DeclarePlayerACriminal(CThingCreatureBase* owner, CThingPlayerCreature* criminal);   // 0x00843610
};

struct CTCCarriedActionUseBase {
    virtual void v00();
    CThingCreatureBase* ParentThing;                // +0x04 (CTCBase)
    void OnUseBase(CThingCreatureBase* user);       // 0x007EAB90 (empty base implementation)
};

struct CTCCarriedActionUseWallMount : CTCCarriedActionUseBase {
    void vfunc_28(CThingCreatureBase* user);
};

void CTCCarriedActionUseWallMount::vfunc_28(CThingCreatureBase* user)
{
    CTCCarriedActionUseBase::OnUseBase(user);
    if (IsThingAlive(user))
    {
        CThingCreatureBase* holder = ParentThing;
        user->SetCurrentAction(&CCreatureAction_DeclarePlayerACriminal(user, (CThingPlayerCreature*)holder));
    }
    else if (ParentThing->HasInterface(0x1a))
        ((CTCInterface1A*)ParentThing->GetInterface(0x1a))->Notify();
}