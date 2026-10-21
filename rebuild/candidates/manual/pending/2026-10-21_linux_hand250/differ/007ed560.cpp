// Grade: DIFFER(354v354) -- frame only: retail shares one key slot for all four interface lookups (frame B8h); ours gives the first (interface 8) lookup its own slot (frame BCh, offsets +4)
// CTCCarriedActionUseGoldBag::OnActionUse(CThingAICreature* user) -- a creature with gold uses a carried gold bag on a targeted creature (type 2): give it the default gift (50) or the receiver's own asking amount; then report the use to the bag's interface-0x1A component
// Leaf: vfunc_28
// Retail 0x007ED560, 368 bytes, `ret 4` (vtable slot 10, +0x28; catalogued as vfunc_28).
// Callees: the base use hook 0x007EAB90, CVectorMap::LowerBound const 0x004365B0 (interfaces 8, 4, 0x80) and 0x0040F020
// (0x1A), CTCTargetingBase::IsTargetedThingOfType 0x007F5A70 and slots 38 (+0x98) / 33 (+0x84), the default gold gift
// 0x007898F0 (returns 50), the receiver's 0x80 component gift amount 0x00789900, CCreatureAction_GiveMoneyToThing ctor
// 0x007ED450 / dtor 0x007ED420, CThingCreatureBase::SetCurrentAction 0x006644F0, the 0x1A component's use hook 0x005D8E50.
// Layout: the landed CTCCarriedActionUseGive::vfunc_28 0x007ED170 model (copied); hero stats gold +0x3C.
// Shape: HasTC + const ternary GetTC lookups on the user (the map pointer stays in esi for the stats lookup); the action is
// a temporary passed straight to SetCurrentAction; the 0x1A report runs on every path.
struct CTCBase;
struct CThing;
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
    SEntry* LowerBound(const long* type) const;             // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTCConst(long wanted) const
    {
        long type = wanted;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
struct CCreatureActionBase {                                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 0x04];
    ~CCreatureActionBase();                                 // 0x00693EF0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                          // +0x04
    ~CBaseIntelligentPointer();                             // 0x00A01C10
};
struct CThingCreatureBase;
struct CCreatureAction_GiveMoneyToThing : CCreatureActionBase {
    unsigned char _body[0xb4 - 0xa8];
    CCreatureAction_GiveMoneyToThing(CThingCreatureBase* giver, CThing* receiver, long amount);   // 0x007ED450
    ~CCreatureAction_GiveMoneyToThing();                    // 0x007ED420
};
struct CThingAICreature : CThing {
    bool SetCurrentAction(CCreatureActionBase* action);     // 0x006644F0 (CThingCreatureBase)
};
struct CTCTargetingBase {                                   // interface 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetTargetedThing();                     // slot 33 (+0x84)
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual bool CanInteractWithTarget(long type);          // slot 38 (+0x98)
    bool IsTargetedThingOfType(long type);                  // 0x007F5A70
};
struct CTCInterface1A {                                     // interface 0x1a component of the carried thing
    void OnUseFailed();                                     // 0x005D8E50
};
struct CTCCarriedActionUseBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    void OnUseBase(CThingAICreature* user);                 // 0x007EAB90 (empty base implementation)
};
long GetDefaultGoldGift();                                 // 0x007898F0
struct CTCHeroStats {                                       // interface 4
    unsigned char _pad00[0x3c];
    long          Gold;                                     // +0x3c
};
struct CTCGoldReceiver {                                    // interface 0x80
    long GetGoldGiftAmount();                               // 0x00789900
};
struct CTCCarriedActionUseGoldBag : CTCCarriedActionUseBase {
    void vfunc_28(CThingAICreature* user);
};

void CTCCarriedActionUseGoldBag::vfunc_28(CThingAICreature* user)
{
    CTCCarriedActionUseBase::OnUseBase(user);
    if (user->HasTC(8)) {
        CTCTargetingBase* targeting = user->GetTCConst<CTCTargetingBase>(8);
        CTCHeroStats* stats;
        if (targeting->IsTargetedThingOfType(2) && targeting->CanInteractWithTarget(2)
            && user->GetInterface(&stats, 4) && stats->Gold > 0) {
            long amount = GetDefaultGoldGift();
            CThing* receiver = targeting->GetTargetedThing();
            if (receiver->HasTC(0x80))
                amount = receiver->GetTCConst<CTCGoldReceiver>(0x80)->GetGoldGiftAmount();
            if (amount > 0)
                user->SetCurrentAction(&CCreatureAction_GiveMoneyToThing((CThingCreatureBase*)user,
                                                                         targeting->GetTargetedThing(), amount));
        }
    }
    if (ParentThing->HasTC(0x1a))
        ParentThing->GetTC<CTCInterface1A>(0x1a)->OnUseFailed();
}
