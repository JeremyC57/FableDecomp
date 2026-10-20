// CTCShopKeeper::DetachFromShop() -- tell the alive shop's shop component it has no keeper, clear the keeper's shop pointer, and refresh the player GUI when a shop was told
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00799000, 181 bytes (plain `ret`).
// Callees: CBaseIntelligentPointer::GetPItem const @ 0x00A01B50, the interface LowerBound @ 0x0040F020, CTCShop::SetShopKeeper
// (CThingCreatureBase*) @ 0x007751A0, CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10
// (the clearing temporary; CIntelligentPointer vtable 0x01238C6C stored inline), CPlayerGui::Refresh(long) @ 0x004378C0 on
// g_pPlayerGui 0x013B8790.
// Layout (retail; Ego_r.pdb CTCShopKeeper): Shop (CIntelligentPointer<CThing>) +0x14; CThing interface bitset +0x20 (0x53 =
// bit 19 of the third word), map +0x44, dead flag bit 0 of +0x91.
// Shape: nested alive / HasTC tests (the third GetPItem in a named local serves HasTC and GetTC) with the one-ternary GetTC (component straight into ecx); the flag rides in bl across the
// clearing block (`cleared(0); item = cleared.GetPItem(); Shop.SetPItem(item);`, the temporary in its own scope).
class CThing;
class CBaseObject;
class CThingCreatureBase;
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
struct CTCShop {
    void SetShopKeeper(CThingCreatureBase* shopKeeper); // 0x007751A0
};
struct CPlayerGui {
    void Refresh(long flag);                            // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790
struct CTCShopKeeper {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    unsigned char _pad0a[2];
    CBaseIntelligentPointer Customer;                   // +0x0c CIntelligentPointer<CThingCreatureBase>
    CBaseIntelligentPointer Shop;                       // +0x14 CIntelligentPointer<CThing>
    void DetachFromShop();
};

void CTCShopKeeper::DetachFromShop()
{
    bool detached = false;
    if (Shop.GetPItem() && !(Shop.GetPItem()->m_flags91 & 1)) {
        CThing* shop = Shop.GetPItem();
        if (shop->HasTC(0x53)) {
            ((CTCShop*)shop->GetTC(0x53))->SetShopKeeper(0);
            detached = true;
        }
    }
    {
        CIntelligentPointer<CThing> cleared(0);
        CThing* item = cleared.GetPItem();
        Shop.SetPItem((CBaseObject*)item);
    }
    if (detached)
        g_pPlayerGui->Refresh(0);
}
