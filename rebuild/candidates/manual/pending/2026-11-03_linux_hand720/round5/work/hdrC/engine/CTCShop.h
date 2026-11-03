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
    void SellItem(long);
    float GetCurrentBasicPrice(long,long) const;
    long GetCurrentBuyPrice(long,long) const;
#ifdef FABLE_RETAIL_CTCSHOP
    // Retail opt-in (w22 agent720c, row 0x005C6B26): StatGetInterfaceType is inline (FableWin.pdb
    // ?StatGetInterfaceType@CTCShop@@SI?AW4ETCInterfaceType@@XZ; GetTC<CTCShop> keys the lookup on 0x53 in place);
    // IsValidStockItem is Ego_r.pdb ?IsValidStockItem@CTCShop@@QBE_NJ@Z (Ego_r 0x0077F3D0 sits between FindDefaultStockItem
    // 0x0077F200 and IsShopKeeperNearerThan 0x0077F3F0; retail 0x007723D0 sits between FindDefaultStockItem 0x00772250 and
    // IsShopKeeperNearerThan 0x007723F0, the same +0x1D0 / +0x20 spacing; manifest label "HasDefaultStockItem").
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SHOP; }
    bool IsValidStockItem(long) const;                                   // 0x007723D0
#endif
    std::vector<GoodsPlace> GetFreeSaleItemPlaces(bool);
    void AddSaleItem(long,const GoodsPlace&);
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
