// Grade: DIFFER(597v597) -- register allocation only: retail keeps this in ebx and &PCrate in ebp (ours swaps them) and picks ecx/edx for a few creature temporaries
// CAIStateGroup_CarryCrateAndFollow::OnReplace() -- release the crate's action point, stop the pick-up action, drop the crate and put the previously carried item back in its slot, then clear the action point state and the remembered item
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CCarryableDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
    unsigned char _pad08[0x28 - 0x08];
    long ActiveCarrySlot;                       // +0x28
    long SecondaryActiveCarrySlot;              // +0x2c
    long PassiveCarrySlot;                      // +0x30
    void Release() { if (--RefCount == 0) Destroy(); }
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (DecRefToZero(Object->RefCount)) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CDefClassBase;
CCarryableDef* FindCarryableSubDef(CDefClassBase* defClass);   // stand-in source of the sub-def (declared only)
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CCarryableDef>* out);   // 0x005CC10E
};
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<CCarryableDef>* out)
{
    out->Object = FindCarryableSubDef(this);
    return out->Object != 0;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                        // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
struct CCreatureActionBase {
    long GetActionID() const;                   // 0x00692620
};
struct CThingCreatureBase : CThing {
    unsigned char _pad94[0x100 - 0x94];
    CCreatureActionBase* CurrentAction;         // +0x100
    bool IsPerformingAnyAction();               // 0x006622A0
    void FinishCurrentAction();                 // 0x00662650
};
struct CTCCarrying {
    bool IsCarryingThing(CThing* thing);        // 0x006A1060
    void StopCarryingThing(CThing* thing);      // 0x006A19B0
    CThing* GetThingInCarrySlot(long slot);     // 0x006A11F0
    CThing* RemoveThingInCarrySlot(long slot, bool drop);   // 0x006A16A0
    void PutThingInCarrySlot(CThing* thing, long slot, bool immediately);   // 0x006A1F80
};
struct CTCCreatureModeManager {
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
    bool AddMode(unsigned long mode);           // 0x0073AE10
};
struct CTCUserActionPointBase {
    void FreeActionPoint(long index);           // 0x007FEF40
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CAIBrain {
    unsigned char _pad00[0x18];
    CThingCreatureBase* Creature;               // +0x18
};
struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
};
struct CAIStateGroup_CarryCrateAndFollow : CAIStateGroupBase {
    CIntelligentPointerThing PCrate;            // +0x14
    CIntelligentPointerThing PPreviouslyCarriedItem;   // +0x1c
    CIntelligentPointerThing PThingToFollow;    // +0x24
    bool GotActionPoint;                        // +0x2c
    long ActionPointIndex;                      // +0x30
    long PickUpActionID;                        // +0x34
    void OnReplace();
};

void CAIStateGroup_CarryCrateAndFollow::OnReplace()
{
    if (IS_THING_ALIVE(PCrate) && GotActionPoint) {
        CTCUserActionPointBase* point;
        CThing* crate = PCrate.GetPItem();
        if (crate->HasTC(0x2d))
            point = (CTCUserActionPointBase*)crate->GetTC(0x2d);
        point->FreeActionPoint(ActionPointIndex);
        if (PickUpActionID != -1 && GetCreature()->IsPerformingAnyAction()
            && GetCreature()->CurrentAction->GetActionID() == PickUpActionID)
            GetCreature()->FinishCurrentAction();
        CThingCreatureBase* creature = GetCreature();
        CTCCreatureModeManager* modes;
        if (creature->HasTC(0x31))
            modes = (CTCCreatureModeManager*)creature->GetTC(0x31);
        CTCCarrying* carrying;
        if (creature->HasTC(0x46))
            carrying = (CTCCarrying*)creature->GetTC(0x46);
        if (carrying->IsCarryingThing(PCrate.GetPItem())) {
            carrying->StopCarryingThing(PCrate.GetPItem());
            modes->RemoveMode(0x22);
        }
        if (IS_THING_ALIVE(PPreviouslyCarriedItem)) {
            CDefPointer<CCarryableDef> def;
            CThing* item = PPreviouslyCarriedItem.GetPItem();
            item->PDef->GetSubDef(&def);
            long passiveSlot = def->PassiveCarrySlot;
            long activeSlot = def->ActiveCarrySlot;
            if (carrying->GetThingInCarrySlot(passiveSlot) == PPreviouslyCarriedItem.GetPItem())
                carrying->RemoveThingInCarrySlot(passiveSlot, false);
            carrying->PutThingInCarrySlot(PPreviouslyCarriedItem.GetPItem(), activeSlot, true);
            if (PPreviouslyCarriedItem.GetPItem()->HasTC(0x48))
                modes->AddMode(4);
        }
    }
    ActionPointIndex = -1;
    GotActionPoint = false;
    PickUpActionID = -1;
    PPreviouslyCarriedItem = CIntelligentPointerThing(0);
}
