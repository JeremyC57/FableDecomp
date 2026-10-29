// CTCCarriedActionUseGiveBeer::OnActionUse(CThingAICreature* user) -- an idle creature uses carried beer: give it to a targeted drinker (type 2 target with the drinker interface 0xC3, which also gets 1.0 drunkenness), else drink it itself
// Leaf: vfunc_28
// Retail 0x007ED2C0, 352 bytes, `ret 4` (vtable slot 10, +0x28; catalogued as vfunc_28).
// Callees: CThingCreatureBase::IsPerformingAnyAction 0x006622A0, the base use hook 0x007EAB90, CVectorMap::LowerBound const
// 0x004365B0 (interface 8, targeting) and 0x0040F020 (interface 0xC3 on the target), CTCTargetingBase::IsTargetedThingOfType
// 0x007F5A70 and slots 38 (+0x98) / 33 (+0x84, the targeted thing), CCreatureAction_GiveItemToThing ctor 0x0062D9E0 / dtor
// 0x00628EE0, CCreatureAction_Drink ctor 0x00843510, CThingCreatureBase::SetCurrentAction 0x006644F0, ~CBaseIntelligentPointer
// 0x00A01C10, ~CCreatureActionBase 0x00693EF0; globals: the beer drunkenness unit 0x0126DEAC, 10.0f 0x01237F0C.
// Layout: the landed CTCCarriedActionUseGive::vfunc_28 0x007ED170 model (copied); the drinker component's drunkenness +0x10.
// Shape: as CTCCarriedActionUseGive: actions are temporaries passed straight to SetCurrentAction (the give action's
// destructor is out of line here, the drink action's implicit one inlines two pointer dtors and the base dtor); the drinker
// is `T* p = 0; if (HasTC) p = GetTC;` on a fresh targeted-thing call; HasTC must be `__forceinline` (else the second use is
// called out of line); the drink is a named local read before the pushes (an inline ParentThing read uses edx).
class CTCBase;
class CThing;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                   // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;             // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        CInterfaceMap* map = &m_interfaces;
        ETCInterfaceType type = (ETCInterfaceType)(wanted);
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((it == map->last || wanted < it->first) ? map->last : it)->second;
    }
    template<class T> __forceinline T* GetTCConst(long wanted) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(wanted);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((it == map->last || wanted < it->first) ? map->last : it)->second;
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
    ~CCreatureAction_GiveItemToThing();                     // 0x00628EE0 (out of line in this TU)
};
class CCreatureAction_Drink : public CCreatureActionBase { public:        // 0xb8 bytes, implicit destructor
    CBaseIntelligentPointer PItem;                          // +0xa8
    CBaseIntelligentPointer PDrink;                         // +0xb0
    CCreatureAction_Drink(CThingCreatureBase* drinker, CThing* drink);   // 0x00843510
};
class CThingAICreature : public CThing { public:
    bool SetCurrentAction(CCreatureActionBase* action);     // 0x006644F0 (CThingCreatureBase)
    bool IsPerformingAnyAction();                           // 0x006622A0 (CThingCreatureBase)
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
extern float g_BeerDrunkennessUnit;                         // 0x0126DEAC
struct CTCDrinker {                                         // interface 0xc3
    unsigned char _pad00[0x10];
    float         Drunkenness;                              // +0x10
};
class CTCCarriedActionUseBase { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    void OnUseBase(CThingAICreature* user);                 // 0x007EAB90 (empty base implementation)
};
struct CTCCarriedActionUseGiveBeer : CTCCarriedActionUseBase {
    void vfunc_28(CThingAICreature* user);
};

void CTCCarriedActionUseGiveBeer::vfunc_28(CThingAICreature* user)
{
    if (user->IsPerformingAnyAction())
        return;
    CTCCarriedActionUseBase::OnUseBase(user);
    if (user->HasTC(8)) {
        CTCTargetingBase* targeting = user->GetTCConst<CTCTargetingBase>(8);
        if (targeting->IsTargetedThingOfType(2) && targeting->CanInteractWithTarget(2)) {
            if (!targeting->GetTargetedThing()->HasTC(0xc3))
                return;
            CThing* item = ParentThing;
            user->SetCurrentAction(&CCreatureAction_GiveItemToThing((CThingCreatureBase*)user,
                                                                    targeting->GetTargetedThing(), item));
            CTCDrinker* drinker = 0;
            CThing* target = targeting->GetTargetedThing();
            if (target->HasTC(0xc3))
                drinker = target->GetTC<CTCDrinker>(0xc3);
            drinker->Drunkenness += g_BeerDrunkennessUnit * 10.0f;
            return;
        }
    }
    CThing* drink = ParentThing;
    user->SetCurrentAction(&CCreatureAction_Drink((CThingCreatureBase*)user, drink));
}