// CTCWife::FindAppropriateGift() -- the wife picks the gift she would like next: a budget around her GiftGivingPriceValue (scaled up
// or down at random by 1 + r*r, r = GFFloatRandom(1.0)), then every live building of her village whose sim def has Property bit 0x10
// and a shop component is searched: each stock item that is not an ordinary gift (gift type 1) or a wedding ring is weighed by its
// DefaultPrice, and the one closest to the budget (and less than a quarter over it) wins. GiftToGiveDef takes the winner and its
// price comes off GiftGivingPriceValue.
// Leaf: ?FindAppropriateGift@CTCWife@@AAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CF530, 728 bytes (+8 int3), __fastcall, plain `ret`, no frame pointer, no EH frame.
// Identity: Ego_r.pdb 0x007060B0 / FableWin.pdb ?FindAppropriateGift@CTCWife@@AAEXXZ; called once by CTCWife::FrameUpdate 0x006D1E30.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inside CThing::GetTC, interfaces 0x23 and 0x53);
//   CTCVillageMember::GetPVillage 0x007EE540; CTCBase::GetWorld 0x00686A70; CWorld::GetWorldSeed 0x0049D810; GFFloatRandom 0x004978A0;
//   GFROR13 0x00497890 (inside the inline GFRandom); __ftol2 0x00BFEA70; CBaseIntelligentPointer::GetPItem 0x00A01B50;
//   CThingBuilding::PeekPSimDefinition 0x0082D2D0 (by value); CTCShop::GetStockList 0x00774610 (const map<long,long> by value);
//   CTCBase::GetDefinitionManager 0x006869D0; CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C; CDefClassBase::GetSubDef<CGiftDef>
//   0x006D3EF0 / GetSubDef<CStockItemDef> 0x0059350C; the map's const_iterator operator++ 0x005B16FF and ~map 0x004DF885 (both
//   STLport inline bodies VC7.1 kept out of line in this large function); the defs' virtual slot 1 when a handle's release drops the count.
// Shape: the budget is ONE local (price loaded into it, then overwritten by the scaled value: retail reuses the price's slot +0x10 for
//   the budget); the village list's end and the stock map's end are cached locals, the map iterator declared before its cached end
//   (retail stores begin to +0x28 before end to +0x34). The item def handle is released through ebp and its slot reused for the stock
//   item def: VC7.1 does that itself with the visible GetDef / GetSubDef bodies (as in sibling 0x006D0630).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGBUILDING_PEEKSIMDEF
#define FABLE_RETAIL_CSIMBUILDINGDEF
#define FABLE_RETAIL_CSTOCKITEMDEF
#define FABLE_RETAIL_CTCSHOP_GETSTOCKLIST
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCWIFE
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CThingBaseDef.h"
#include "engine/CGiftDef.h"
#include "engine/CStockItemDef.h"
#include "engine/CSimBuildingDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingBuilding.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCShop.h"
#include "engine/CWorld.h"
#include "engine/GFRandom.h"
#include "engine/GFFloatRandom.h"
#include "engine/CTCWife.h"

void CTCWife::FindAppropriateGift()
{
    CTCVillageMember* member;
    if (!GetParentThing().GetTC(TCI_VILLAGE_MEMBER, &member))
        return;
    CTCVillage* village = member->GetPVillage();
    if (!village)
        return;
    float spread = GFFloatRandom(1.0f, GetWorld().GetWorldSeed());
    spread = spread * spread + 1.0f;
    long budget = GiftGivingPriceValue;
    budget = (long)(GFRandom(0xFFFFFFFF, GetWorld().GetWorldSeed()) > 0x7FFFFFFF ? budget * spread : budget / spread);
    long bestGift = 0;
    long bestValue = 0x7FFFFFFF;
    std::list<CIntelligentPointer<CThingBuilding> >::iterator end = village->VillageBuildings.end();
    for (std::list<CIntelligentPointer<CThingBuilding> >::iterator b = village->VillageBuildings.begin(); b != end; ++b) {
        CThingBuilding* building = b->Get();
        if (!building || building->IsDead())
            continue;
        CDefPointer<const CSimBuildingDef> simDef = building->PeekPSimDefinition();
        if (simDef.IsNull())
            continue;
        CTCShop* shop;
        if ((simDef->Property & 0x10) && building->GetTC(TCI_SHOP, &shop)) {
            const std::map<long,long> stock = shop->GetStockList();
            std::map<long,long>::const_iterator it = stock.begin();
            std::map<long,long>::const_iterator stockEnd = stock.end();
            for (; it != stockEnd; ++it) {
                CDefPointer<const CThingBaseDef> item;
                GetDefinitionManager().GetDef(it->first, &item);
                CDefPointer<const CGiftDef> gift;
                if (!item->GetSubDef(&gift) || (gift->Type != 1 && !gift->IsWeddingRing)) {
                    CDefPointer<const CStockItemDef> value;
                    item->GetSubDef(&value);
                    long cost = value->DefaultPrice;
                    if (abs(cost - budget) < abs(budget - bestValue) && cost - budget < budget / 4) {
                        bestValue = cost;
                        bestGift = it->first;
                    }
                }
            }
        }
    }
    if (bestGift > 0) {
        GiftToGiveDef = bestGift;
        GiftGivingPriceValue -= bestValue;
    }
}