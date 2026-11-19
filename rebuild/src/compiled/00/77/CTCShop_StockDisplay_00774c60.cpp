// CTCShop::StockDisplay() -- put the shop's displayable stock on its free goods places: pair the free places (GetFreeSaleItemPlaces(false)) with the shuffled displayable items (GetSaleItemsToPlace()) and AddSaleItem each pair, as many as the shorter list
// Leaf: ?StockDisplay@CTCShop@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00774C60, 188 bytes, thiscall `ret`.
// Name: Ego_r.pdb / FableWin.pdb ?StockDisplay@CTCShop@@QAEXXZ (shared engine/CTCShop.h, FABLE_RETAIL_CTCSHOP). Ego_d locals goods_places,
//   goods_to_place, {n}; FableWin debug 0x0261C840 statement order.
// Callees: GetFreeSaleItemPlaces 0x00774B40, GetSaleItemsToPlace 0x00773EE0, AddSaleItem 0x00774D20, free 0x00BFEA14.
// What closed it (pair landing): with only declarations of the two vector producers VC7.1 lets the hidden-return vectors escape and
//   re-reads both sizes every pass (best standalone DIFFER(175v188)). Retail's TU defines both bodies before StockDisplay (0x00773EE0,
//   0x00774B40, 0x00774C60): with them visible (called, not inlined) c2 keeps the sizes. So this TU carries the real bodies of both
//   producers, in retail order, on shared headers:
//   - GetSaleItemsToPlace: the landed 0x00773EE0 shared-header text verbatim (grades RELOCATION_MATCH at 0x00773EE0 in this TU);
//   - GetFreeSaleItemPlaces: re-ported from the landed local model (stand-in vector internals) onto shared headers: Ego_d locals
//     allow_blocked, places, {n (long), {BLOCKED_RADIUS (const float), area (C2DBoxI), p_thing (CThing*)}} and FableWin 0x0261ABB0 order
//     (GoodsPlaces.clear(), AddSaleItemPlacesFromThing(ParentThing), the loop, `allow_blocked || !GetPFirstThing(GFFilter_Bind2(IsNot,
//     In3DRange(pos, BLOCKED_RADIUS)), &p_thing)` -> push_back). `area` is computed and unused (both Ego_d and FableWin have it; VC7.1
//     drops it). Grades RELOCATION_MATCH at 0x00774B40 in this TU (the landed local-model row could be respelled to this text).
// Header opt-ins: new overlay FABLE_RETAIL_CTCSHOP_ADDSALEITEMPLACES (hdr/engine/CTCShop.h: the AddSaleItemPlacesFromThing declaration),
//   FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_NOINLINE (retail calls vector<GoodsPlace>::erase(first, last) 0x00777A90 from clear()),
//   FABLE_RETAIL_CWORLD / _CTHINGSEARCHTOOLS / _CTHINGSEARCHTOOLS_FIRSTTHING (PeekThingSearchTools, GetPFirstThing 0x00777000), the
//   landed GetSaleItemsToPlace set. No flags.
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTCSHOP
#define FABLE_RETAIL_CSTOCKITEMDEF
#define FABLE_RETAIL_STLP_RANDOM_SHUFFLE_2ARG
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTTHING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_NOINLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTCSHOP_ADDSALEITEMPLACES
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CShopDef.h"
#include "engine/CStockItemDef.h"
#include "engine/CTCShop.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_In3DRange.h"
#include "engine/CFilter_Bind2.h"
#include "engine/C2DBoxI.h"
// ---- 0x00773EE0 CTCShop::GetSaleItemsToPlace (landed; its shared-header text, same TU, defined first as in retail) ----
std::vector<long> CTCShop::GetSaleItemsToPlace() const
{
    std::vector<long> items;
    CDefPointer<const CShopDef> shopDef;
    bool noShopDef = ParentThing->GetPBaseDef().IsNull() || !ParentThing->GetPBaseDef()->GetSubDef(&shopDef);
    if (noShopDef)
        return items;
    for (std::map<long, CShopItem>::const_iterator it = Stock.begin(); it != Stock.end(); ++it) {
        if (it->first > 0 && it->second.Quantity > 0.0f) {
            CDefPointer<const CDefClassBase> itemDef = GetDefinitionManager().GetPDefFromGlobalIndex(it->first);
            CDefPointer<const CStockItemDef> stockDef;
            if (itemDef->GetSubDef(&stockDef) &&
                stockDef->DefaultPrice <= shopDef->MaxPriceToDisplay && stockDef->CanBeDisplayedInShop)
                items.push_back(it->first);
        }
    }
    std::random_shuffle(items.begin(), items.end());
    return items;
}
// ---- 0x00774B40 CTCShop::GetFreeSaleItemPlaces (same TU; this text also grades RELOCATION_MATCH at 0x00774B40) ----
std::vector<CTCShop::GoodsPlace> CTCShop::GetFreeSaleItemPlaces(bool allow_blocked)
{
    GoodsPlaces.clear();
    AddSaleItemPlacesFromThing(ParentThing);
    std::vector<GoodsPlace> places;
    for (long n = 0; n < GoodsPlaces.size(); n++) {
        const float BLOCKED_RADIUS = 0.2f;
        C2DBoxI area(GoodsPlaces[n].pos.ToC2DCoordI(), (long)BLOCKED_RADIUS);
        CThing* p_thing;
        if (allow_blocked || !PeekWorld().PeekThingSearchTools().GetPFirstThing(GFFilter_Bind2(CThingFilter_IsNot(*ParentThing),
                CThingFilter_In3DRange(GoodsPlaces[n].pos, BLOCKED_RADIUS)), &p_thing))
            places.push_back(GoodsPlaces[n]);
    }
    return places;
}
// ---- 0x00774C60 CTCShop::StockDisplay (this row) ----
void CTCShop::StockDisplay()
{
    std::vector<GoodsPlace> goods_places = GetFreeSaleItemPlaces(false);
    std::vector<long> goods_to_place = GetSaleItemsToPlace();
    for (long n = 0; n < std::min(goods_places.size(), goods_to_place.size()); n++)
        AddSaleItem(goods_to_place[n], goods_places[n]);
}