// CTCShop::SellItem(long item) -- one unit of a stocked item leaves the shop: clear it as the special item, drop its quantity by one (not below 0), raise its max stock by the shop def's per-sell change (not below 0), and kill the first living display thing of that def owned by the shop's thing
// Leaf: ?SellItem@CTCShop@@QAEXJ@Z
// Retail: Fable.exe 0x00774730, 419 bytes, thiscall `ret 4` (Ego_r.pdb name above).
// Callees: CDefClassBase::GetSubDef<CShopDef> 0x005130A0 (out of line), map<long,CShopItem>'s tree find 0x00777120 (out of line, hidden
//   iterator result) and operator[] 0x00778280, CVectorMap::LowerBound 0x004365B0 (the expanded PeekTC<CTCThingOwner>),
//   CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::Kill 0x004C9B80, the def's Delete (slot 1) on the last release.
// Shape: the tree find is the STLport member-template find (FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE), called out of line as at
//   0x00774770; a named reference to the owned pointer keeps &node->data in esi from the alive test to the Kill (0x007748AB `mov ecx,esi`),
//   where `(*it)->` re-forms it from the node (`lea ecx,[esi+8]`, 420 v 419 bytes).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CShopDef.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCShop.h"
#include "engine/ThingAlive.h"

void CTCShop::SellItem(long item)
{
    CDefPointer<const CShopDef> shop_def;
    GetParentThing().PeekBaseDef().GetSubDef(&shop_def);
    if (item == SpecialItem) {
        SpecialItem = -1;
        GoldDonated = 0;
    }
    if (Stock.find(item) == Stock.end())
        return;
    float quantity = Stock[item].Quantity;
    Stock[item].Quantity = quantity < 1.0f ? 0.0f : quantity - 1.0f;
    Stock[item].MaxStock += shop_def->MaxStockChangePerSell;
    if (Stock[item].MaxStock < 0.0f)
        Stock[item].MaxStock = 0.0f;
    const CTCThingOwner* owner;
    if (GetParentThing().PeekTC(&owner)) {
        for (std::list<CIntelligentPointer<CThing> >::const_iterator it = owner->GetObjectList().begin(); it != owner->GetObjectList().end(); ++it) {
            const CIntelligentPointer<CThing>& thing = *it;
            if (IS_THING_ALIVE(thing) && thing->GetDefGlobalIndex() == item) {
                thing->Kill(true);
                break;
            }
        }
    }
}