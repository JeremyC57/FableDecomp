#ifndef FABLE_ENGINE_CTCSHOP_H
#define FABLE_ENGINE_CTCSHOP_H
#include "engine/CTCBase.h"
#include "engine/CShopItem.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CRightHandedSet.h"
#include "stlport/fable_map.h"
class CThingCreatureBase;
// Original Ego_r TPI0x2cb84; native Stock+34 and final Alarm+54.
// Native STLport vectors and tree are twelve bytes rather than donor sixteen.
class CTCShop : public CTCBase {
public:
    enum ESTOCK_STATE { STOCKED_YES=0,STOCKED_NO=1,STOCKED_DONTKNOW=2 };
    struct GoodsPlace { C3DVector pos; CRightHandedSet rhset; };
    ESTOCK_STATE StockState;
    CTCShop(CThing&);
    virtual ~CTCShop();
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
    virtual bool HasFrameUpdate() const;
    virtual void FrameUpdate();
    virtual void OnCreate();
    virtual void OnInitialActivate();
    virtual bool IsComponentSerialisable() const;
    virtual void OnSerialise(CPersistContext&,bool);
    const CShopItem* FindDefaultStockItem(long) const;
#ifdef FABLE_RETAIL_CTCSHOP_VALIDSTOCK
    bool IsValidStockItem(long)const;
#endif
    void SellItem(long);
    float GetCurrentBasicPrice(long,long) const;
    long GetCurrentBuyPrice(long,long) const;
#ifdef FABLE_RETAIL_CTCSHOP_SELLPRICE
    // Retail opt-in (chunk 867, rows 0x005AF64E BuyItem / 0x005B01FE SellItemPC): Ego_r.pdb + FableWin.pdb
    // ?GetCurrentSellPrice@CTCShop@@QBEJJJ@Z, out of line at 0x007743B0 (the price the shop sells `count` of an item for; the
    // catalog's GetCurrentBuyPrice label for that address is propagated). Declaration only.
    long GetCurrentSellPrice(long,long) const;                         // 0x007743B0
#endif
    std::vector<GoodsPlace> GetFreeSaleItemPlaces(bool);
    void AddSaleItem(long,const GoodsPlace&);
#ifdef FABLE_RETAIL_CTCSHOP
    // Retail opt-in (chunk 755, row 0x00775B40 FrameUpdate): Ego_r.pdb public members called by FrameUpdate, all out of line in
    // retail (declarations only, no layout change): ?SetAlarm@CTCShop@@QAEX_N@Z 0x00773C90, ?RemoveStockItemsFromShop@CTCShop@@QAEXXZ
    // 0x00773E10, ?UpdateStock@CTCShop@@QAEXXZ 0x00775350 (the stock rebuild OnInitialActivate 0x00775B20 tail-jumps to),
    // ?IsPermanentlyStocked@CTCShop@@QBE_NXZ 0x007721A0, ?StockDisplay@CTCShop@@QAEXXZ 0x00774C60 (all landed but UpdateStock).
    void SetAlarm(bool);                                                 // 0x00773C90
    void RemoveStockItemsFromShop();                                     // 0x00773E10
    void UpdateStock();                                                  // 0x00775350
    bool IsPermanentlyStocked() const;                                   // 0x007721A0
    void StockDisplay();                                                 // 0x00774C60
    // Ego_r.pdb ?IsOpeningHours@CTCShop@@QBE_NXZ (Fable.exe 0x00773950; row 0x00918A40).
    bool IsOpeningHours() const;
#endif
#ifdef FABLE_RETAIL_CTCSHOP_AVAILABLE
    // Retail opt-in (chunk 795, row 0x0092FAB0): Ego_r.pdb ?IsShopKeeperAvailable@CTCShop@@QBE_NXZ, out of line in retail (0x00773AA0,
    // landed). Declaration only.
    bool IsShopKeeperAvailable() const;                                  // 0x00773AA0
#endif
#ifdef FABLE_RETAIL_CTCSHOP_SHOPKEEPER
    // Retail opt-in (chunk 794, row 0x007039A0 CThingObject::OnHit): FableWin.pdb ?GetShopKeeper@CTCShop@@QBEPAVCThingCreatureBase@@XZ,
    // inline in retail (GetPItem 0x00A01B50 on PShopKeeper +0x10, `lea ebp,[esi+10h]`), and the interface key. Bodies only.
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SHOP; }
    CThingCreatureBase* GetShopKeeper() const { return PShopKeeper.Get(); }
#endif
#if defined(FABLE_RETAIL_CTCSHOP_INTERFACETYPE) && !defined(FABLE_RETAIL_CTCSHOP_SHOPKEEPER)
    // Retail opt-in (chunk 1002b, row 0x005C6B26 CTCInventoryWeapons::ConstructWeaponDescription): the interface key alone (FableWin.pdb
    // ?StatGetInterfaceType@CTCShop@@SI?AW4ETCInterfaceType@@XZ, inline: the row's expanded GetTC<CTCShop> tests 0x53), for rows that do
    // not want the _SHOPKEEPER body (which needs the complete CThingCreatureBase).
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SHOP; }
#endif
#ifdef FABLE_RETAIL_CTCSHOP_SHOPTYPE
    // Retail opt-in (chunk 851, row 0x006001E0): Ego_r.pdb + FableWin.pdb ?IsBarberShop@CTCShop@@QBE_NXZ (0x00772490),
    // ?IsTattooShop@CTCShop@@QBE_NXZ (0x00772510) and ?IsTitleShop@CTCShop@@QBE_NXZ (0x00772590), out of line in retail
    // (CTCInventoryMap::ConstructRegionDescriptionShops calls them in this order). Declarations only.
    bool IsBarberShop() const;                                           // 0x00772490
    bool IsTattooShop() const;                                           // 0x00772510
    bool IsTitleShop() const;                                            // 0x00772590
#endif
#ifdef FABLE_RETAIL_CTCSHOP_NAME
    // Retail opt-in (chunk 867, row 0x005A8C66 CTCInventoryTrade::ConstructActualSpecialInventoryPC): Ego_r.pdb ?GetName@CTCShop@@QBEKXZ,
    // out of line in retail (0x00772610; the row tests it `> 0` unsigned and passes it to the text bank). Declaration only.
    unsigned long GetName() const;                                       // 0x00772610
#endif
#ifdef FABLE_RETAIL_CTCSHOP_WANTED
    // Retail opt-in (chunk 891, row 0x005A5B31 CTCInventoryTrade::ConstructActualWantedInventoryPC): Ego_r.pdb
    // ?GetWantedItemsList@CTCShop@@QBE?AV?$vector@JV?$allocator@J@std@@@std@@XZ, out of line in retail (0x00774100, landed; the caller
    // passes its local vector as the hidden result). Declaration only.
    std::vector<long> GetWantedItemsList() const;                        // 0x00774100
#endif
private:
    CIntelligentPointer<CThingCreatureBase> PShopKeeper;
    std::vector<CIntelligentPointer<CThingCreatureBase> > Shoppers;
    bool ShopKeeperAvailable;
    std::vector<GoodsPlace> GoodsPlaces;
    std::map<long,CShopItem> Stock;
    long SpecialItem,DaysSinceSpecialItemPlaced,DayLastChecked,GoldDonated;
    bool StallLike;
    unsigned long Alarm;
};
FABLE_STATIC_ASSERT(sizeof(CTCShop)==0x58);
FABLE_STATIC_ASSERT(sizeof(CTCShop::GoodsPlace)==36);
#endif
