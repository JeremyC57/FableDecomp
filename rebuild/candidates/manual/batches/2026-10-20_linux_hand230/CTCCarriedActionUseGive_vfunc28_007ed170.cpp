// CTCCarriedActionUseGive::vfunc_28(CThingAICreature* user) -- run the base use hook; when the user is targeting a creature (targeting type 2) it may give to, start a give-item action towards it; otherwise tell the carried thing's interface-0x1a component that the use failed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ED170, 250 bytes (+ int3 padding), `ret 4`. Vtable 0x0123FAB4
//   (CTCCarriedActionUseGive) slot 28.
// Callees: the base hook @ 0x007EAB90 (empty `ret 4`), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @
//   0x004365B0 (the user's targeting TC, interface 8) and @ 0x0040F020 (the carried thing's interface 0x1a),
//   CTCTargetingBase::IsTargetedThingOfType(ETargetingType) @ 0x007F5A70, the targeting TC's virtual slots 38 (+0x98, a
//   bool test on the type) and 33 (+0x84, the targeted thing), CCreatureAction_GiveItemToThing(CThingCreatureBase*,
//   CThing*, CThing*) @ 0x0062D9E0, CThingCreatureBase::SetCurrentAction @ 0x006644F0, ~CBaseIntelligentPointer @
//   0x00A01C10 (x2), ~CCreatureActionBase @ 0x00693EF0, the 0x1a component's use-failed hook @ 0x005D8E50 (manifest
//   "CChunkCollectionIndicator::OnPredicateFail" is propagated; named descriptively).
// Layout: CTCBase ParentThing +0x04; CThing presence bitset +0x20, interface map +0x44; the give action is 0xb8 bytes
//   (CCreatureActionBase 0xa8 + two CIntelligentPointer<CThing> members at +0xa8/+0xb0).
// Shape: the landed CTCCarriedActionUseDrop::vfunc_28 0x007ED0A0 pattern: the action is a temporary passed straight from
//   its constructor (its compiler-generated destructor: the two pointer members out of line, then the base); the targeting
//   lookup is HasTC + the ternary GetTC (const LowerBound) with the early return inside; the carried thing is a named
//   local read before the targeted-thing vcall (frees eax for the vptr: `mov eax,[esi]`); the non-const GetTC forms the
//   map pointer before the key local (`lea ecx,[key]` after `lea esi,[thing+44h]` order flips otherwise).
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
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        CInterfaceMap* map = &m_interfaces;
        long type = wanted;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
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
struct CCreatureAction_GiveItemToThing : CCreatureActionBase {   // 0xb8 bytes
    CBaseIntelligentPointer PItem;                          // +0xa8
    CBaseIntelligentPointer PReceiver;                      // +0xb0
    CCreatureAction_GiveItemToThing(CThingCreatureBase* giver, CThing* receiver, CThing* item);   // 0x0062D9E0
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
struct CTCCarriedActionUseGive : CTCCarriedActionUseBase {
    void vfunc_28(CThingAICreature* user);
};

void CTCCarriedActionUseGive::vfunc_28(CThingAICreature* user)
{
    CTCCarriedActionUseBase::OnUseBase(user);
    if (user->HasTC(8)) {
        CTCTargetingBase* targeting = user->GetTCConst<CTCTargetingBase>(8);
        if (targeting->IsTargetedThingOfType(2) && targeting->CanInteractWithTarget(2)) {
            CThing* item = ParentThing;
            user->SetCurrentAction(&CCreatureAction_GiveItemToThing((CThingCreatureBase*)user,
                                                                    targeting->GetTargetedThing(), item));
            return;
        }
    }
    if (ParentThing->HasTC(0x1a))
        ParentThing->GetTC<CTCInterface1A>(0x1a)->OnUseFailed();
}
