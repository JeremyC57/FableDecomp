// CTCCarriedActionUseFish::OnActionUse(CThing& user) -- base OnActionUse; aimed at a wall mount, a live creature user gets the action of mounting the fish; otherwise the fish's bonus-item def is applied to the user's hero stats and the fish is killed
// Leaf: ?OnActionUse@CTCCarriedActionUseFish@@UAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EDBE0, length 235 bytes (+ int3 padding). Vtable slot 28 ("vfunc_28"), `ret 4`.
// Callees: CTCActionUseBase::OnActionUse @ 0x007EAB90 (qualified; manifest "$E2"), CTCCarriedActionUseFish::IsWallMountTargeted
// (const CThing&) const @ 0x007EDB80, CCreatureAction_UseWallMount::CCreatureAction_UseWallMount @ 0x00843610 (vtable 0x0127498C
// per RTTI; the manifest's "CCreatureAction_DeclarePlayerACriminal" is propagated), CThingCreatureBase::SetCurrentAction @ 0x006644F0,
// ~CBaseIntelligentPointer @ 0x00A01C10, ~CCreatureActionBase @ 0x00693EF0, CDefClassBase::GetSubDef<CBonusItemDef> @ 0x007E1D30
// (on the fish's def, +0x70), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for CTCHeroStats,
// type 4 = bit 4 of CThing+0x20), CTCHeroStats::ApplyBonusItemEffects(const CBonusItemDef&) @ 0x0057C4F1, CThing::Kill(bool)
// @ 0x004C9B80, the def's virtual Destroy (slot 1).
// Shape: the wall-mount arm returns on its own; the 0xB0-byte action temporary (CCreatureActionBase 0xA8 + an intelligent
// pointer) is passed by reference straight from the ctor and destroyed inline (novtable leaf); the bonus def is read from its out
// pointer right after GetSubDef (ebx) and released by hand at the end.
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
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1 (+0x04)
    long RefCount;                              // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
class CBonusItemDef : public CDefPointeeBase { public:
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CBonusItemDef>* out) const;   // 0x007E1D30
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CBonusItemDef>* out) const
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
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    void Kill(bool immediately);                // 0x004C9B80
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
};
class CCreatureActionBase;
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
inline CThingCreatureBase* CastToCreature(CThing* thing) { return (CThingCreatureBase*)thing; }
class CTCHeroStats {
public:
    void ApplyBonusItemEffects(const CBonusItemDef& def);   // 0x0057C4F1
};
struct CBaseIntelligentPointer {
    void* __vftable;
    void* m_pItem;
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
class CCreatureActionBase {                     // retail 0xA8 bytes
public:
    virtual ~CCreatureActionBase();             // 0x00693EF0
    unsigned char _pad04[0xa8 - 4];
};
class __declspec(novtable) CCreatureAction_UseWallMount : public CCreatureActionBase {
public:
    CBaseIntelligentPointer PWallMount;         // +0xa8
    CCreatureAction_UseWallMount(CThingCreatureBase& creature, CThing& fish);   // 0x00843610
};
class CTCActionUseBase {
public:
    virtual ~CTCActionUseBase();
    CThing* ParentThing;                        // +0x04
    CThing& GetParentThing() const { return *ParentThing; }
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    virtual void OnActionUse(CThing& user);     // 0x007EAB90
};
class CTCCarriedActionUseFish : public CTCActionUseBase {
public:
    bool IsWallMountTargeted(const CThing& user) const;   // 0x007EDB80
    virtual void OnActionUse(CThing& user);
};

void CTCCarriedActionUseFish::OnActionUse(CThing& user)
{
    CTCActionUseBase::OnActionUse(user);
    if (IsWallMountTargeted(user)) {
        CThingCreatureBase* creature = CastToCreature(&user);
        if (creature && !creature->IsDead())
            creature->SetCurrentAction(CCreatureAction_UseWallMount(*creature, GetParentThing()));
        return;
    }
    CDefPointer<CBonusItemDef> out;
    const CDefClassBase* thingDef = ParentThing->PDef;
    bool found = thingDef->GetSubDef(&out);
    CBonusItemDef* def = out.Object;
    CTCHeroStats* stats;
    if (found && user.GetInterface(&stats, 4))
        stats->ApplyBonusItemEffects(*def);
    ParentThing->Kill(true);
    if (def) {
        if (--def->RefCount == 0)
            def->Destroy();
    }
}