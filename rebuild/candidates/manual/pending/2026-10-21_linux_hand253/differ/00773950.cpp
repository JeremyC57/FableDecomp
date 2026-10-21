// Grade: DIFFER(324v324) -- register/scheduling only: retail forms the villager lookup key address in ecx before `lea esi,[eax+44h]`, ours forms the map pointer first and takes eax for the key
// CTCShop::IsOpeningHours() -- false with no live shopkeeper; true when the shop def says the shop is always open; otherwise a villager shopkeeper keeps the village's hours (IsCookTime), anyone else is always open
// Leaf: IsOpeningHours
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00773950, 324 bytes (plain `ret`).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50; CDefClassBase::GetSubDef(CDefPointer<const CShopDef>*)
//          0x005130A0 (manifest label is a propagated byte twin; landed GetName 0x00772610 names it); CVectorMap<
//          ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (interface 0x23 = word 1 bit 3, villager);
//          GFGetPVillageInterface(CTCVillager*) 0x007EE540 (`__fastcall`; manifest "IS_THING_ALIVE<...>" is propagated);
//          CTCVillage::IsCookTime() 0x00658B20; the defs' virtual slot 1 on release.
// Layout (retail CTCShop, as the landed IsShopKeeperAvailable 0x00773AA0): ParentThing +0x04, PShopKeeper
//          (CIntelligentPointer<CThingCreatureBase>) +0x10; CThing PDef +0x70; CShopDef always-open flag +0x9c (name
//          unverified); the thing's interface bitset +0x20, map +0x44, dead flag bit 0 of +0x91.
// Shape: `GetDef() && GetDef()->GetSubDef(&shopDef) && shopDef->AlwaysOpen` with GetDef returning a CDefPointer BY VALUE:
//        the two conditionally built temporaries get construction flags in ebx (1, then 3) tested before their
//        releases; the shop def is read once from its out slot (GetSubDef stand-in below) and held in ebp.
struct CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CDefPointeeBase {
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1 (+0x04)
    long m_refCount;                            // +0x04
    void ReleaseReference() { if (--m_refCount == 0) Delete(); }
};
struct CShopDef : CDefPointeeBase {
    unsigned char _pad08[0x9c - 0x08];
    bool AlwaysOpen;                            // +0x9c (name unverified)
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& o) : Object(o.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer() { if (Object) Object->ReleaseReference(); }
    T* operator->() const { return Object; }
    operator bool() const { return Object != 0; }
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CShopDef>* out); // 0x005130A0 (CDefPointer<const CShopDef>*)
};
void* FindDef_CDefClassBase_GetSubDef_1();      // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed GetName 0x00772610 / 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CShopDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefPointer<CDefClassBase> PDef;            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool HasTC(long interfaceType) const
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
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +4
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCVillage {
    bool IsCookTime();                          // 0x00658B20
};
struct CTCVillager;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);   // 0x007EE540

struct CTCShop {
    virtual void v00();
    CThing*                 ParentThing;        // +0x04 (CTCBase)
    bool                    ToBeRemoved;        // +0x08
    bool                    IsInGlobalUpdateTCs;// +0x09
    long                    StockState;         // +0x0c
    CBaseIntelligentPointer PShopKeeper;        // +0x10
    CDefPointer<CDefClassBase> GetDef() const { return ParentThing->PDef; }
    bool IsOpeningHours();
};

bool CTCShop::IsOpeningHours()
{
    if (!IS_THING_ALIVE(PShopKeeper))
        return false;
    CDefPointer<CShopDef> shopDef;
    bool alwaysOpen = GetDef() && GetDef()->GetSubDef(&shopDef) && shopDef->AlwaysOpen;
    if (!alwaysOpen) {
        CThing* keeper = PShopKeeper.GetPItem();
        if (keeper->HasTC(0x23)) {
            if (CTCVillage* village = GFGetPVillageInterface(keeper->GetTC<CTCVillager>(0x23)))
                return village->IsCookTime();
        }
    }
    return true;
}
