// CTCShop::GetCurrentBasicPrice(long def, long add) const -- 0 when the thing's def has no shop sub def; otherwise the item's default price (1 when the item def has no stock-item sub def), reduced by the shop's per-item reduction times the stock held (rounded up, plus `add`, the reduction capped), times the default stock item's price multiplier
// Leaf: ?GetCurrentBasicPrice@CTCShop@@QBEMJJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007727E0, 433 bytes, thiscall with frame pointer, `ret 8`.
// Identity: Ego_r.pdb 0x0077F8B0 / FableWin.pdb 0x0261FE90 ?GetCurrentBasicPrice@CTCShop@@QBEMJJ@Z. Parameters and locals (def, add, pdef,
//   ShopDef, price, def_class_base, stock_item_def, stock_it, reduction, pshop_item): Ego_d.pdb.
// Shape: FableWin 0x0261FE90 statement for statement with its asserts (debug only) left out: GetDef<CThingBaseDef>, GetSubDef<CShopDef>
//   (no sub def: return 0), GetPDefFromGlobalIndex(def), GetSubDef<CStockItemDef> (DefaultPrice, else 1), Stock.find(def) and, when
//   held, reduction = PriceReductionFractionPerItem * (GFFloatToLongCeil(Quantity) + add) (FableWin evaluates ShopDef-> first) capped by std::min at
//   MaxPriceReductionFractionDueToStockCount, price *= 1 - reduction; FindDefaultStockItem(def) -> price *= PriceMultiplier.
// Callees: CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C, CDefClassBase::GetSubDef<CShopDef>
//   0x005130A0 / <CStockItemDef> 0x0059350C, CDefinitionManager::GetPDefFromGlobalIndex 0x009AD9E0, map<long, CShopItem>::find 0x00777170,
//   CTCShop::FindDefaultStockItem 0x00772250.
// Bodies seen (all called out of line, as retail): map<long, CShopItem>::find (FABLE_RETAIL_STLP_TREE_CONSTFIND_NOINLINE, the landed
//   0x00777170 spelling; this TU's copy grades MATCH there): seeing that find only reads the key lets VC7.1 copy `def` into ebx before
//   GetPDefFromGlobalIndex and reuse its frame word [ebp+8] for the GFFloatToLongCeil result and `reduction` (0x007728C5..0x007728F6);
//   declaration-only find keeps def in memory (435v433). GetDef<T> / GetSubDef<T> (CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY,
//   CDEFCLASSBASE_GETSUBDEF_BODY): without either the CDefPointer release / reload shapes differ (455 / 475 v 433). Their linked
//   COMDATs are other objects' (0x0042AF3C / 0x0059350C: /Os objects; 0x005130A0 calls the non-const GetPDefFromGlobalIndex
//   0x009ADA10), so this TU's copies are not graded there.
// Retail-asm: 0x007728C1 `fistp dword ptr [ebp+8]` -- engine/GFFloatToLongCeil.h (FableWin calls ?GFFloatToLongCeil@@YIJM@Z here). This row
//   carries no asm of its own.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CSTOCKITEMDEF
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_NOINLINE
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CShopDef.h"
#include "engine/CStockItemDef.h"
#include "engine/CShopItem.h"
#include "engine/CTCShop.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/GFFloatToLongCeil.h"

float CTCShop::GetCurrentBasicPrice(long def, long add) const
{
    CDefPointer<const CThingBaseDef> pdef;
    GetDefinitionManager().GetDef(ParentThing->GetDefGlobalIndex(), &pdef);
    CDefPointer<const CShopDef> ShopDef;
    if (!pdef->GetSubDef(&ShopDef))
        return 0.0f;
    float price = 0.0f;
    CDefPointer<const CDefClassBase> def_class_base = GetDefinitionManager().GetPDefFromGlobalIndex(def);
    CDefPointer<const CStockItemDef> stock_item_def;
    if (def_class_base->GetSubDef(&stock_item_def))
        price = (float)stock_item_def->DefaultPrice;
    else
        price = 1.0f;
    std::map<long, CShopItem>::const_iterator stock_it = Stock.find(def);
    if (stock_it != Stock.end()) {
        float reduction = ShopDef->PriceReductionFractionPerItem * (GFFloatToLongCeil(stock_it->second.Quantity) + add);
        reduction = std::min(reduction, ShopDef->MaxPriceReductionFractionDueToStockCount);
        price *= 1.0f - reduction;
    }
    const CShopItem* pshop_item = FindDefaultStockItem(def);
    if (pshop_item)
        price *= pshop_item->PriceMultiplier;
    return price;
}
