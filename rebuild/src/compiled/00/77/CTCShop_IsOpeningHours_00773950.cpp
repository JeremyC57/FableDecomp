// CTCShop::IsOpeningHours() -- false with no live shopkeeper; true when the shop def says the shop is always open; otherwise a villager shopkeeper keeps the village's hours (IsCookTime), anyone else is always open
// Leaf: ?IsOpeningHours@CTCShop@@QBE_NXZ
// Lever (chunk 840): `if (alwaysOpen) return true;` ahead of the villager block (instead of nesting the block under
//   `if (!alwaysOpen)`) flips VC7.1's tie-break in the inline GetTC: the key address `lea ecx,[esp+18h]` now precedes
//   `lea esi,[eax+44h]` as in retail (an IR-numbering tie; no GetTC/LowerBound spelling alone moved it). Same bytes otherwise.
// Names (chunk 840): IsOpeningHours is const (Ego_r.pdb ?IsOpeningHours@CTCShop@@QBE_NXZ, engine/CTCShop.h); interface 0x23 is
//   TCI_VILLAGE_MEMBER and 0x007EE540 is ?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ (engine/CTCVillageMember.h; the
//   parked rows' free `GFGetPVillageInterface(CTCVillager*)` was descriptive: NAME_FIXES); CTCVillage::IsCookTime is const
//   (?IsCookTime@CTCVillage@@QBE_NXZ); the always-open byte is Ego_r.pdb CShopDef::AlwaysOpen (+0xAC in Ego_r, +0x9C in retail
//   whose STLport containers are 12 bytes).
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
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CDefPointeeBase {
public:
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1 (+0x04)
    long m_refCount;                            // +0x04
    void ReleaseReference() { if (--m_refCount == 0) Delete(); }
};
class CShopDef : public CDefPointeeBase {
public:
    unsigned char _pad08[0x9c - 0x08];
    bool AlwaysOpen;                            // +0x9c retail (Ego_r.pdb +0xAC)
};
template <class T> class CDefPointer {
public:
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
class CDefClassBase : public CDefPointeeBase {
public:
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

class CThing {
public:
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
class CBaseIntelligentPointer {
public:
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
class CTCVillage {
public:
    bool IsCookTime() const;                    // 0x00658B20 ?IsCookTime@CTCVillage@@QBE_NXZ
};
class CTCVillageMember {
public:                       // interface TCI_VILLAGE_MEMBER = 0x23 (engine/CTCVillageMember.h)
    CTCVillage* GetPVillage() const;            // 0x007EE540 ?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ
};

class CTCShop {
public:
    virtual void v00();
    CThing*                 ParentThing;        // +0x04 (CTCBase)
    bool                    ToBeRemoved;        // +0x08
    bool                    IsInGlobalUpdateTCs;// +0x09
    long                    StockState;         // +0x0c
    CBaseIntelligentPointer PShopKeeper;        // +0x10
    CDefPointer<CDefClassBase> GetDef() const { return ParentThing->PDef; }
    bool IsOpeningHours() const;              // ?IsOpeningHours@CTCShop@@QBE_NXZ
};

bool CTCShop::IsOpeningHours() const
{
    if (!IS_THING_ALIVE(PShopKeeper))
        return false;
    CDefPointer<CShopDef> shopDef;
    bool alwaysOpen = GetDef() && GetDef()->GetSubDef(&shopDef) && shopDef->AlwaysOpen;
    if (alwaysOpen)
        return true;
    CThing* keeper = PShopKeeper.GetPItem();
    if (keeper->HasTC(0x23)) {
        if (CTCVillage* village = keeper->GetTC<CTCVillageMember>(0x23)->GetPVillage())
            return village->IsCookTime();
    }
    return true;
}