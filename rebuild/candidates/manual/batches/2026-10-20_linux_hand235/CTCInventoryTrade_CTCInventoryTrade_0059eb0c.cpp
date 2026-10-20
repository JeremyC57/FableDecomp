// CTCInventoryTrade::CTCInventoryTrade(CThing* thing) -- constructor: inventory base and hero-information-screen base, own vtables, empty trade containers, cleared counters, no warning and no trophy use pending, an empty special-item string and no open trade menu
// Leaf: ??0CTCInventoryTrade
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059EB0C, 316 bytes (`ret 4`); the manifest row says "CTCVillage::CTCVillage"
// (wrong: the two vtable stores are CTCInventoryTrade's primary 0x01253034 and its NHeroInformationScreens::CBase side
// 0x0125301C at +0x130, as in the landed dtor 0x0059EC48 that follows it).
// Callees: NInventory::CTCInventoryBase::CTCInventoryBase(CThing*) 0x005BFF07; NHeroInformationScreens::CBase::CBase()
//          0x0066E734; list<long>::list(const allocator&) 0x0042AB58; the STLport map default ctors (manifest
//          "CDisplayManager::CopyBackBufferToTexture" is propagated across the byte twins): CComponent-to-(long, long)
//          0x005B2706, CComponent-to-CList 0x005B2724, map<long, long> 0x004DF86C, CComponent-to-(CWideString, long)
//          0x005B2E6C, CComponent-to-CComponent 0x005B2747; CWideString() 0x0099AED0.
// Layout: the landed dtor's retail model (CTCInventoryBase 0x130 bytes, containers 12 bytes, list 4).
// Shape: frame-pointer size TU (`#pragma optimize("y", off)` + `optimize("s", on)`, the trade module's ApplyHairCard
//        0x0059D9C2 idiom); members in declaration order through the init list, the vector by its inline zeroing ctor,
//        the list's allocator temporary homed in the dead `thing` argument slot; the unlisted members stay unset.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CThing;
namespace NUISystem { class CComponent; }

struct SAllocator {                             // empty std::allocator stand-in
    SAllocator() { }
};
struct CPrimitiveHandleVector {                 // vector<CEnginePrimitiveHandle> (12 bytes)
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    CPrimitiveHandleVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CLongList {                              // list<long> (one sentinel pointer)
    void* _M_node;
    explicit CLongList(const SAllocator& allocator = SAllocator());   // 0x0042AB58
};
struct CComponentPairMap { void* _M_header; unsigned long _M_node_count; long _M_key_compare; CComponentPairMap(); };       // 0x005B2706
struct CComponentListMap { void* _M_header; unsigned long _M_node_count; long _M_key_compare; CComponentListMap(); };       // 0x005B2724
struct CLongLongMap { void* _M_header; unsigned long _M_node_count; long _M_key_compare; CLongLongMap(); };                 // 0x004DF86C
struct CComponentStringMap { void* _M_header; unsigned long _M_node_count; long _M_key_compare; CComponentStringMap(); };   // 0x005B2E6C
struct CComponentMap { void* _M_header; unsigned long _M_node_count; long _M_key_compare; CComponentMap(); };               // 0x005B2747
struct CWideString { void* m_pData; CWideString(); };                                                                      // 0x0099AED0

namespace NInventory {
class CTCInventoryBase {                        // retail sizeof 0x130
public:
    CTCInventoryBase(CThing* thing);            // 0x005BFF07
    virtual void v00();
    unsigned char _pad04[0x130 - 0x04];
};
}
namespace NHeroInformationScreens {
class CBase {                                   // vtable 0x0123A3E0
public:
    CBase();                                    // 0x0066E734
    virtual void v00();
};
}
class CTCInventoryTrade : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
    CPrimitiveHandleVector TradeAnimPrimitiveHandles;   // +0x134
    CLongList              NotInStock;          // +0x140
    CComponentPairMap      ItemsToBuy;          // +0x144
    CComponentPairMap      ItemsToSell;         // +0x150
    bool  IsShoppingValid;                      // +0x15c
    long  CategoryCount;                        // +0x160
    long  ItemSellCount;                        // +0x164
    long  ItemBuyCount;                         // +0x168
    long  ItemSellUniqueCount;                  // +0x16c
    long  CategoryIdent;                        // +0x170
    long  PreviousScreenType;                   // +0x174
    bool  WarningNeeded;                        // +0x178
    long  TrophyUseNeeded;                      // +0x17c
    CComponentListMap      SellListsByCategory; // +0x180
    CComponentListMap      BuyListsByCategory;  // +0x18c
    CComponentListMap      WantedListsByCategory;   // +0x198
    CLongLongMap           SpecialItemsToDelete;    // +0x1a4
    CComponentStringMap    NumBuyableThings;    // +0x1b0
    CComponentStringMap    NumSellableThings;   // +0x1bc
    CComponentStringMap    NumWantedThings;     // +0x1c8
    long  TotNumClothesToSell;                  // +0x1d4
    long  TotNumWeaponsToSell;                  // +0x1d8
    long  TotNumItemsToSell;                    // +0x1dc
    long  TotNumClothesToBuy;                   // +0x1e0
    long  TotNumWeaponsToBuy;                   // +0x1e4
    long  TotNumItemsToBuy;                     // +0x1e8
    long  TotNumClothesWanted;                  // +0x1ec
    long  TotNumWeaponsWanted;                  // +0x1f0
    long  TotNumItemsWanted;                    // +0x1f4
    long  TotSpecialItemsToBuy;                 // +0x1f8
    CWideString            SpecialItemString;   // +0x1fc
    CComponentMap          ItemBuyLists;        // +0x200
    CComponentMap          ItemSellLists;       // +0x20c
    bool  InventoryTradeOpen;                   // +0x218
    bool  InventoryTradeInitialised;            // +0x219
    NUISystem::CComponent* PMouse_Pointer;      // +0x21c
    NUISystem::CComponent* PTop_Level_List;     // +0x220
    NUISystem::CComponent* PCurrentOpenTradeMenu;   // +0x224
    float AccumFadeTime;                        // +0x228
    CTCInventoryTrade(CThing* thing);
    virtual void v00();
};

CTCInventoryTrade::CTCInventoryTrade(CThing* thing)
    : NInventory::CTCInventoryBase(thing),
      WarningNeeded(false), TrophyUseNeeded(-1),
      TotNumClothesToSell(0), TotNumWeaponsToSell(0), TotNumItemsToSell(0),
      TotNumClothesToBuy(0), TotNumWeaponsToBuy(0), TotNumItemsToBuy(0),
      TotNumClothesWanted(0), TotNumWeaponsWanted(0), TotNumItemsWanted(0), TotSpecialItemsToBuy(0),
      InventoryTradeOpen(false), InventoryTradeInitialised(false),
      PMouse_Pointer(0), PTop_Level_List(0), PCurrentOpenTradeMenu(0)
{
}
