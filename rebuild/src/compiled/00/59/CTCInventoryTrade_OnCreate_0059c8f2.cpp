// CTCInventoryTrade::OnCreate() -- registers the trade screen with the hero-information screen manager, fetches the trade-inventory sub-def from the owner's def, points the inventory at its inventory def, runs the base OnCreate and zeroes the category/sell/buy counts
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059C8F2, length 145 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CPlayerGui::GetPHeroInfoScreensManager() const @ 0x00437CD0 (manifest "CWorld::DrawGetEnvironment", propagated;
// the body reads +0x32C), NHeroInformationScreens::CManager::RegisterScreen(const CBase*) @ 0x0066EB11,
// CDefClassBase::GetSubDef<T>(CDefPointer<const T>*) @ 0x00485815, CTCInventoryBase::SetPDef(long) @ 0x005BDB4D,
// CTCInventoryBase::OnCreate() @ 0x005C0121, the defs' slot 1 on release. Globals: the player GUI @ 0x013B8790.
// Shape: the screen argument is the null-safe conversion to the NHeroInformationScreens::CBase secondary base at +0x130
// (`neg ; sbb ; and`), pushed before the manager getter runs. The owner's def pointer is read into a local, then the sub-def
// out pointer is zeroed (`and [ebp-4],0` between the load and the null test), then the owner def is add-ref'd and released
// by hand around GetSubDef (both null-tested); the sub-def is released by hand at the end without a null test. A copied
// CDefPointer (inline copy ctor) puts the zero store before the load instead.
// Layout: CThing PDef +0x70; retail CTCInventoryBase 0x130 bytes, ParentThing +0x04; CTCInventoryTrade CategoryCount +0x160,
// ItemSellCount +0x164, ItemBuyCount +0x168 (engine header PDB +0x18C..+0x194 less 0x2C of 16-byte debug containers; the
// same shift puts CategoryIdent at the +0x170 that RemoveAllItemsFromCategory 0x0059CF49 stores); the trade def's
// inventory-def index at +0x9C (retail offset).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
struct CInventoryTradeDef : CDefPointeeBase {
    unsigned char _pad08[0x9c - 0x08];
    long InventoryDefIndex;                     // +0x9c
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CInventoryTradeDef>* out);   // 0x00485815
};
struct CThing {
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;            // +0x70
};

namespace NHeroInformationScreens {
struct CBase {                                  // vtable 0x0123A3E0
    virtual void v00();
    virtual void v01();
    virtual void v02();
};
struct CManager {
    void RegisterScreen(const CBase* screen);   // 0x0066EB11
};
}
struct CPlayerGui {
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790

struct CTCInventoryBase_Trade {               // retail 0x130 bytes
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void OnCreate();                    // slot 4
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x130 - 0x08];
    void SetPDef(long globalDefIndex);          // 0x005BDB4D
};
class CTCInventoryTrade : public CTCInventoryBase_Trade, public NHeroInformationScreens::CBase {
public:
    unsigned char _pad_0x134[0x160 - 0x134];
    long CategoryCount;                         // +0x160
    long ItemSellCount;                         // +0x164
    long ItemBuyCount;                          // +0x168
    virtual void OnCreate();
};

void CTCInventoryTrade::OnCreate()
{
    g_pPlayerGui->GetPHeroInfoScreensManager()->RegisterScreen(this);
    CDefClassBase* thingDef = ParentThing->PDef.Object;
    CDefPointer<CInventoryTradeDef> def;
    def.Object = 0;
    if (thingDef)
        ++thingDef->m_refCount;
    thingDef->GetSubDef(&def);
    if (thingDef && --thingDef->m_refCount == 0)
        thingDef->Destroy();
    CInventoryTradeDef* tradeDef = def.Object;
    SetPDef(tradeDef->InventoryDefIndex);
    CTCInventoryBase_Trade::OnCreate();
    CategoryCount = 0;
    ItemSellCount = 0;
    ItemBuyCount = 0;
    if (--tradeDef->m_refCount == 0)
        tradeDef->Destroy();
}