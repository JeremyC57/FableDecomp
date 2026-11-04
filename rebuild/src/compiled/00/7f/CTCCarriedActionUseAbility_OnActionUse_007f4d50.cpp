// CTCCarriedActionUseAbility::OnActionUse(CThing& user) -- base OnActionUse; a user with an ability inventory is told the carried item's ability was used; then the item's showable component is notified
// Leaf: ?OnActionUse@CTCCarriedActionUseAbility@@UAEXAAVCThing@@@Z
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
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
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
class CTCBase;
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
class CAbilityDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x28 - 0x08];
    long Ability;                               // +0x28
};
class CDefClassBase { public:
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
    unsigned char _pad54[0x1c];
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
        CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
        return (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
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