// CTCCarriedActionUseAbility::OnActionUse(CThing& user) -- base OnActionUse; a user with an ability inventory is told the carried item's ability was used; then the item's showable component is notified
// Leaf: ?OnActionUse@CTCCarriedActionUseAbility@@UAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F4D50, length 186 bytes (+ int3 padding). Vtable slot 28 ("vfunc_28" in the
// catalog), `ret 4`.
// Callees: CTCActionUseBase::OnActionUse @ 0x007EAB90 (qualified; manifest "$E2"), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020 (inlined lookups: the user's CTCInventoryAbilities, type 0x6F = bit 15 of CThing+0x2C; the item's showable
// component, type 0x1A = bit 26 of +0x20), CDefClassBase::GetSubDef<CAbilityDef> @ 0x005936C2 (on the item's def, +0x70),
// CTCInventoryAbilities::OnSpecialAbilityUsed(long, bool) @ 0x005EEA9B, the showable component's OnShowEnded @ 0x005D8E50 (landed
// name, unproven), the def's virtual Destroy (slot 1). The ability index is CAbilityDef +0x28.
// Shape: HasTC + one-ternary GetTC: the abilities component is a named local (esi) and the showable component lands in ecx;
// the def is read from its out pointer right after GetSubDef into edi and released by hand inside the block.
struct CTCBaseInterface;
struct CInterfaceMap {                          // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (STLport lower_bound over the 8-byte {type, component} entries): compiled above
// the caller so VC7.1 knows the key's address does not escape and packs the lookup keys as retail (the landed 0x00848730 idiom).
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
#pragma auto_inline(on)
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
struct CAbilityDef : CDefPointeeBase {
    unsigned char _pad08[0x28 - 0x08];
    long Ability;                               // +0x28
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CAbilityDef>* out) const;   // 0x005936C2
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CAbilityDef>* out) const
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 UsingComponents
    CInterfaceMap m_interfaces;                 // +0x44 Components
    unsigned char _pad4c[0x70 - 0x4c];
    const CDefClassBase* PDef;                  // +0x70
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
class CTCInventoryAbilities {
public:
    void OnSpecialAbilityUsed(long ability, bool used);   // 0x005EEA9B
};
class CTCShowableItem {                         // interface type 0x1a
public:
    void OnShowEnded();                         // 0x005D8E50
};
class CTCActionUseBase {
public:
    virtual ~CTCActionUseBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    virtual void OnActionUse(CThing& user);     // 0x007EAB90
};
class CTCCarriedActionUseAbility : public CTCActionUseBase {
public:
    virtual void OnActionUse(CThing& user);
};

void CTCCarriedActionUseAbility::OnActionUse(CThing& user)
{
    CTCActionUseBase::OnActionUse(user);
    if (user.HasTC(0x6f)) {
        CTCInventoryAbilities* abilities = user.GetTC<CTCInventoryAbilities>(0x6f);
        CDefPointer<CAbilityDef> out;
        const CDefClassBase* thingDef = ParentThing->PDef;
        bool found = thingDef->GetSubDef(&out);
        CAbilityDef* def = out.Object;
        if (found)
            abilities->OnSpecialAbilityUsed(def->Ability, true);
        if (def) {
            if (--def->RefCount == 0)
                def->Destroy();
        }
    }
    if (ParentThing->HasTC(0x1a))
        ParentThing->GetTC<CTCShowableItem>(0x1a)->OnShowEnded();
}