// CTCShop::GetSaleItemsToPlace() const -- the stocked item global indices a shop may put on display (key > 0, Quantity > 0,
//   the item's CStockItemDef DefaultPrice <= the shop def's MaxPriceToDisplay and CanBeDisplayedInShop), in a random order;
//   an empty vector when the parent thing has no CShopDef sub-def.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00773EE0, 538 bytes (540 with pad), __fastcall, `ret 4` (vector<long> by hidden pointer).
// Leaf: ?GetSaleItemsToPlace@CTCShop@@QBE?AV?$vector@JV?$allocator@J@std@@@std@@XZ
// Callees: CDefClassBase::GetSubDef<CShopDef> 0x005130A0, CTCBase::GetDefinitionManager 0x006869D0,
//   CDefinitionManager::GetPDefFromGlobalIndex(long) const 0x009AD9E0, CDefClassBase::GetSubDef<CStockItemDef> 0x0059350C,
//   vector<long>::push_back 0x00485FF3 (out of line, visible body noinline), std::random_shuffle<long*> 0x00777080 (__fastcall),
//   vector<long> copy ctor 0x00514160 (out of line, by-value return), free 0x00BFEA14.
// Shape: the named-bool def test over two by-value GetPBaseDef() copies (as the twin 0x00774100), the Stock map const_iterator
//   loop, a by-value CDefPointer<const CDefClassBase> temporary per item, STLport two-argument random_shuffle, copy-out return.
//   Matched once ported onto the shared headers (GetSubDef NOINLINE, PUSHBACK_NOINLINE, VECTOR_COPY_OUTOFLINE opt-ins).
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
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CShopDef.h"
#include "engine/CStockItemDef.h"
#include "engine/CTCShop.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefClassBase_GetSubDef.h"
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