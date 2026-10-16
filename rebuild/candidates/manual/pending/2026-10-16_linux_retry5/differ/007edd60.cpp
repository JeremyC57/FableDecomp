// CTCCarriedActionUsePotion::vfunc_28(CThing* user) -- drink the carried potion: run the base use hook, apply the potion's bonus-item def to the user's hero stats (when it has them), then kill the potion thing
// Grade: DIFFER(133v133) -- retail homes the GetSubDef out pointer in the dead `user` argument slot and then reuses that slot for the interface key; ours opens a `push ecx` frame word for the out pointer, and the component is taken through eax (`mov eax,[eax+4] ; push ebx ; mov ecx,eax`, the out-parameter GetInterface form) where ours loads ecx directly (a by-value forceinline out-pointer helper and the out-parameter GetInterface were tried: 119v133 / 145v133) | retry batch 5: a __forceinline GetBonusDef(def) helper returning the sub-def object (releasing it when not found) still opens the push ecx frame word (133v133); the out pointer in the dead user slot is the dead-argument-slot lane gap, parked
//
// Retail 0x007EDD60, 133 bytes (`ret 4`). Vtable 0x0123FDE4 (CTCCarriedActionUsePotion) slot 28.
// Callees: the base hook @ 0x007EAB90 (empty `ret 4` in another TU), CDefClassBase::GetSubDef
// <CBonusItemDef>(CDefPointer<const CBonusItemDef>*) @ 0x007E1D30 (landed), CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020, CTCHeroStats::ApplyBonusItemEffects(const CBonusItemDef*)
// @ 0x0057C4F1, CThing::Kill(bool) @ 0x004C9B80, the def's virtual slot 1 (release at zero).
// Layout: CThing PDef_Object +0x70 (the parent thing's def), the interface bitset +0x20 and map +0x44.
// Shape: the CDefPointer local starts null and is homed in the dead `user` argument slot (user rides
// in esi); GetSubDef runs on the raw def object; the hero-stats interface (4: `test byte [esi+20h],10h`)
// comes from the inline GetInterface straight into ecx; the pointer's `__forceinline` destructor
// releases with a null test and no null store (the object is in ebx).
struct CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

struct CDefPointeeBase {
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1: `call [eax+4]`
    long m_refCount;                            // +0x04
};

template <class T>
struct CDefPointer {                            // GetSubDef's out pointer (released by hand below)
    CDefPointeeBase* Object;                    // +0x00
};

struct CBonusItemDef;

struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<const CBonusItemDef>* out);  // 0x007E1D30 GetSubDef<CBonusItemDef>
};

struct CTCHeroStats {
    void ApplyBonusItemEffects(const CBonusItemDef* def);   // 0x0057C4F1
};

struct CThing {
    unsigned char    _pad0[0x20];
    unsigned long    m_presentBits[9];                      // +0x20 interface-present bitset
    CInterfaceMap    m_interfaces;                          // +0x44
    unsigned char    _pad4c[0x70 - 0x4c];
    CDefPointeeBase* PDef_Object;                           // +0x70 CDefPointer<const CThingDef>::Object
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
    void Kill(bool removeNow);                              // 0x004C9B80
};

struct CTCCarriedActionUseBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    void OnUseBase(CThing* user);                           // 0x007EAB90 (empty base implementation)
};

struct CTCCarriedActionUsePotion : CTCCarriedActionUseBase {
    void vfunc_28(CThing* user);
};

void CTCCarriedActionUsePotion::vfunc_28(CThing* user)
{
    CTCCarriedActionUseBase::OnUseBase(user);
    CDefPointer<const CBonusItemDef> out;
    out.Object = 0;
    bool found = ((CDefClassBase*)ParentThing->PDef_Object)->GetSubDef(&out);
    CDefPointeeBase* bonus = out.Object;
    if (found && user->HasInterface(4))
        ((CTCHeroStats*)user->GetInterface(4))->ApplyBonusItemEffects((const CBonusItemDef*)bonus);
    ParentThing->Kill(true);
    if (bonus && --bonus->m_refCount == 0)
        bonus->Delete();
}
