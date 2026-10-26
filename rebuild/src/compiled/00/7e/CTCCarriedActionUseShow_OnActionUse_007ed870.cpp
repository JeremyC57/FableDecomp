// CTCCarriedActionUseShow::OnActionUse(CThingAICreature* user) -- a creature shows its carried item to a targeted creature (type 2), applying the item's bonus-item def to the user's hero stats; otherwise report the failed use to the item's interface-0x1A component
// Leaf: vfunc_28
// Retail 0x007ED870, 384 bytes, `ret 4` (vtable slot 10, +0x28; catalogued as vfunc_28).
// Callees: the base use hook 0x007EAB90, CThing::SetAsGameStatePersisted(bool) 0x004C7880, CVectorMap::LowerBound const
// 0x004365B0 (interface 8) and 0x0040F020 (interfaces 4, 0x1A), CTCTargetingBase::IsTargetedThingOfType 0x007F5A70 and
// slots 38 (+0x98) / 33 (+0x84), the show-item action ctor 0x007ED780 (labelled CCreatureAction_GiveItemToThing),
// CThingCreatureBase::SetCurrentAction 0x006644F0, ~CBaseIntelligentPointer 0x00A01C10, ~CCreatureActionBase 0x00693EF0,
// CDefClassBase::GetSubDef<CBonusItemDef> 0x007E1D30, CTCHeroStats::ApplyBonusItemEffects(CBonusItemDef*) 0x0057C4F1,
// the 0x1A component's use hook 0x005D8E50; the def's slot 1 release.
// Layout: the landed CTCCarriedActionUseGive::vfunc_28 0x007ED170 model (copied); CThing def +0x70.
// Shape: the action temporary has an implicit destructor (two pointer dtors and the base); the item def follows the landed
// CTCChest::GetOpenAnimationForCreature holder pattern (the thing's def read before the out pointer is zeroed; the def
// copied out into a register-held holder released at the end) with a same-TU GetSubDef stand-in so the out slot is shared
// with the interface keys; the stats come from the out-parameter GetInterface (component in eax), reusing the user map.
class CTCBase;
class CThing;
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
    SEntry* LowerBound(const long* type) const;             // 0x004365B0
};
class CDefClassBase;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                                    // +0x70 (CDefPointer Object)
    void SetAsGameStatePersisted(bool persisted);           // 0x004C7880
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        CInterfaceMap* map = &m_interfaces;
        long type = wanted;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
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
    template<class T> __forceinline T* GetTCConst(long wanted) const
    {
        long type = wanted;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
class CCreatureActionBase { public:                                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 0x04];
    virtual ~CCreatureActionBase();                                 // 0x00693EF0
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
class CThingCreatureBase;
class CCreatureAction_GiveItemToThing : public CCreatureActionBase { public:   // 0xb8 bytes
    CBaseIntelligentPointer PItem;                          // +0xa8
    CBaseIntelligentPointer PReceiver;                      // +0xb0
    CCreatureAction_GiveItemToThing(CThingCreatureBase* giver, CThing* receiver, CThing* item);   // 0x0062D9E0
};
struct CCreatureAction_ShowItem : CCreatureActionBase {    // 0xb8 bytes, implicit destructor
    CBaseIntelligentPointer PItem;                          // +0xa8
    CBaseIntelligentPointer PViewer;                        // +0xb0
    CCreatureAction_ShowItem(CThingCreatureBase* shower, CThing* viewer, CThing* item);   // 0x007ED780
};
class CThingAICreature : public CThing { public:
    bool SetCurrentAction(CCreatureActionBase* action);     // 0x006644F0 (CThingCreatureBase)
};
class CTCTargetingBase { public:                                   // interface 8
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
class CTCCarriedActionUseBase { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    void OnUseBase(CThingAICreature* user);                 // 0x007EAB90 (empty base implementation)
};
class CDefPointeeBase { public:
    virtual void Destroy();                                 // slot 0
    virtual void Delete();                                  // slot 1 (+0x04)
    long m_refCount;                                        // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
};
class CBonusItemDef : public CDefPointeeBase { public: };
class CDefClassBase : public CDefPointeeBase { public:
    CDefPointeeBase* FindSubDef();
    bool GetSubDef(CDefPointer<CBonusItemDef>* out);        // 0x007E1D30
};
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef<CBonusItemDef> (instantiated next to this function in retail): with its body visible the
// out pointer's slot dies after the copy-out and is shared with the later interface keys, as in retail.
bool CDefClassBase::GetSubDef(CDefPointer<CBonusItemDef>* out)
{
    CDefPointeeBase* p = FindSubDef();
    out->Object = (CBonusItemDef*)p;
    if (p)
        ++p->m_refCount;
    return p != 0;
}
#pragma auto_inline(on)
class CTCHeroStats { public:
    void ApplyBonusItemEffects(CBonusItemDef* def);         // 0x0057C4F1
};
struct CTCCarriedActionUseShow : CTCCarriedActionUseBase {
    void vfunc_28(CThingAICreature* user);
};

void CTCCarriedActionUseShow::vfunc_28(CThingAICreature* user)
{
    CTCCarriedActionUseBase::OnUseBase(user);
    ParentThing->SetAsGameStatePersisted(false);
    if (user->HasTC(8)) {
        CTCTargetingBase* targeting = user->GetTCConst<CTCTargetingBase>(8);
        if (targeting->IsTargetedThingOfType(2) && targeting->CanInteractWithTarget(2)) {
            CThing* item = ParentThing;
            user->SetCurrentAction(&CCreatureAction_ShowItem((CThingCreatureBase*)user, targeting->GetTargetedThing(), item));
            CDefClassBase* itemDef = ParentThing->PDef;
            CDefPointer<CBonusItemDef> out;
            bool found = itemDef->GetSubDef(&out);
            CDefHolder<CBonusItemDef> def(out.Object);
            CTCHeroStats* stats;
            if (found && user->GetInterface(&stats, 4))
                stats->ApplyBonusItemEffects(def.Object);
            return;
        }
    }
    if (ParentThing->HasTC(0x1a))
        ParentThing->GetTC<CTCInterface1A>(0x1a)->OnUseFailed();
}