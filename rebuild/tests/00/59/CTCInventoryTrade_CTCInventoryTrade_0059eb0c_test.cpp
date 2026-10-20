// behaviour test for 0059eb0c CTCInventoryTrade::CTCInventoryTrade: both bases built (the inventory base with the thing),
// every container constructed, warning cleared, trophy use -1, the trade counters and menu pointers zero
#include <cstdio>
class CThing;
namespace NUISystem { class CComponent; }
static int g_ctors = 0; static CThing* g_baseThing = 0; static int g_heroBase = 0;
struct SAllocator { SAllocator() { } };
struct CPrimitiveHandleVector {
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    CPrimitiveHandleVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CLongList { void* _M_node; explicit CLongList(const SAllocator& = SAllocator()) : _M_node(0) { ++g_ctors; } };
struct CComponentPairMap { void* _M_header; CComponentPairMap() : _M_header(0) { ++g_ctors; } };
struct CComponentListMap { void* _M_header; CComponentListMap() : _M_header(0) { ++g_ctors; } };
struct CLongLongMap { void* _M_header; CLongLongMap() : _M_header(0) { ++g_ctors; } };
struct CComponentStringMap { void* _M_header; CComponentStringMap() : _M_header(0) { ++g_ctors; } };
struct CComponentMap { void* _M_header; CComponentMap() : _M_header(0) { ++g_ctors; } };
struct CWideString { void* m_pData; CWideString() : m_pData(0) { ++g_ctors; } };
namespace NInventory {
class CTCInventoryBase {
public:
    CTCInventoryBase(CThing* thing) { g_baseThing = thing; }
    virtual void v00() {}
};
}
namespace NHeroInformationScreens {
class CBase {
public:
    CBase() { ++g_heroBase; }
    virtual void v01() {}
};
}
class CTCInventoryTrade : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase {
public:
    CPrimitiveHandleVector TradeAnimPrimitiveHandles;
    CLongList              NotInStock;
    CComponentPairMap      ItemsToBuy;
    CComponentPairMap      ItemsToSell;
    bool  IsShoppingValid;
    long  CategoryCount;
    long  ItemSellCount;
    long  ItemBuyCount;
    long  ItemSellUniqueCount;
    long  CategoryIdent;
    long  PreviousScreenType;
    bool  WarningNeeded;
    long  TrophyUseNeeded;
    CComponentListMap      SellListsByCategory;
    CComponentListMap      BuyListsByCategory;
    CComponentListMap      WantedListsByCategory;
    CLongLongMap           SpecialItemsToDelete;
    CComponentStringMap    NumBuyableThings;
    CComponentStringMap    NumSellableThings;
    CComponentStringMap    NumWantedThings;
    long  TotNumClothesToSell;
    long  TotNumWeaponsToSell;
    long  TotNumItemsToSell;
    long  TotNumClothesToBuy;
    long  TotNumWeaponsToBuy;
    long  TotNumItemsToBuy;
    long  TotNumClothesWanted;
    long  TotNumWeaponsWanted;
    long  TotNumItemsWanted;
    long  TotSpecialItemsToBuy;
    CWideString            SpecialItemString;
    CComponentMap          ItemBuyLists;
    CComponentMap          ItemSellLists;
    bool  InventoryTradeOpen;
    bool  InventoryTradeInitialised;
    NUISystem::CComponent* PMouse_Pointer;
    NUISystem::CComponent* PTop_Level_List;
    NUISystem::CComponent* PCurrentOpenTradeMenu;
    float AccumFadeTime;
    CTCInventoryTrade(CThing* thing);
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
int main()
{
    CThing* thing = (CThing*)&g_ctors;
    CTCInventoryTrade* trade = new CTCInventoryTrade(thing);
    bool ok = g_baseThing == thing && g_heroBase == 1 && g_ctors == 13 && trade->TradeAnimPrimitiveHandles._M_start == 0 &&
              !trade->WarningNeeded && trade->TrophyUseNeeded == -1 && trade->TotNumClothesToSell == 0 &&
              trade->TotSpecialItemsToBuy == 0 && !trade->InventoryTradeOpen && !trade->InventoryTradeInitialised &&
              trade->PMouse_Pointer == 0 && trade->PTop_Level_List == 0 && trade->PCurrentOpenTradeMenu == 0;
    if (!ok) { std::printf("0059eb0c_TEST FAIL\n"); return 1; }
    std::printf("0059eb0c_TEST PASS\n");
    return 0;
}