// CTCShop::GetFreeSaleItemPlaces(bool allow_blocked) -- rebuild the shop's goods places from the parent thing and return them, skipping (unless allow_blocked) every place with another thing within 0.2 of it
// Grade: RELOCATION_MATCH (hand4d 2026-11-19, re-port of the landed local model; coordinator lever 5 of row 0x00774D20). The landed row
//   models the class, the vector and the search locally (same-TU auto_inline(off) stand-ins, a descriptive In2DRange filter). This is
//   FableWin.pdb's text (0x0261ABB0, ?GetFreeSaleItemPlaces@CTCShop@@QAE?AV?$vector@UGoodsPlace@CTCShop@@...@std@@_N@Z) over the
//   shared headers, with Ego_d.pdb's locals (allow_blocked; places; n; BLOCKED_RADIUS, area, p_thing): GoodsPlaces.clear(),
//   AddSaleItemPlacesFromThing(&GetParentThing()), the indexed loop over size(), GFFilter_Bind2(CThingFilter_IsNot(GetParentThing()),
//   CThingFilter_In3DRange(pos, BLOCKED_RADIUS)) into PeekWorld().PeekThingSearchTools().GetPFirstThing, push_back, return places.
//   The dead C2DBoxI area (FableWin builds it from pos.ToC2DCoordI() and an __ftol2 of a float) emits no code in retail; kept as
//   FableWin writes it, its radius spelled (long)BLOCKED_RADIUS (descriptive: the converted float is not visible in the release build).
// Inferred: `(long)BLOCKED_RADIUS` as the area's radius (dead local; FableWin only shows an __ftol2 of a float). Without `area` and with
//   `erase(begin(), end())` for clear() the grade is the same RELOCATION_MATCH.
// Leaf: ?GetFreeSaleItemPlaces@CTCShop@@QAE?AV?$vector@UGoodsPlace@CTCShop@@V?$allocator@UGoodsPlace@CTCShop@@@std@@@std@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00774B40, 274 bytes, thiscall `ret 8` (hidden result + bool). Callees: vector<GoodsPlace>::erase
//   0x00777A90, CTCShop::AddSaleItemPlacesFromThing 0x007748E0, CTCBase::PeekWorld 0x00686A80, CThingSearchTools::GetPFirstThing<
//   CFilter_Bind2<CThingFilter_IsNot, CThingFilter_In3DRange>, CThing> 0x00777000, vector<GoodsPlace>::push_back 0x00778210,
//   vector<GoodsPlace> copy ctor 0x00777430, free 0x00BFEA14.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CSTOCKITEMDEF
#define FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE
#define FABLE_RETAIL_CTCSHOP_BUYITEM
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCSHOP_ADDSALEITEMPLACES
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTTHING
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_MORALITY
#define FABLE_RETAIL_CTCHEROSTATS_MORALITYMAX
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CTCINVENTORYBASE
#include "fable_string.h"
#include <math.h>
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CShopDef.h"
#include "engine/CStockItemDef.h"
#include "engine/CShopItem.h"
#include "engine/CTCShop.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCInventoryItem.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/ThingAlive.h"
#include "engine/GFPower.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_In3DRange.h"
#include "engine/CFilter_Bind2.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DBoxI.h"

std::vector<CTCShop::GoodsPlace> CTCShop::GetFreeSaleItemPlaces(bool allow_blocked)
{
    GoodsPlaces.clear();
    AddSaleItemPlacesFromThing(&GetParentThing());
    std::vector<GoodsPlace> places;
    for (long n = 0; n < GoodsPlaces.size(); ++n) {
        if (!allow_blocked) {
            const float BLOCKED_RADIUS = 0.2f;
            C2DBoxI area(GoodsPlaces[n].pos.ToC2DCoordI(), (long)BLOCKED_RADIUS);
            CThing* p_thing;
            if (PeekWorld().PeekThingSearchTools().GetPFirstThing(GFFilter_Bind2(CThingFilter_IsNot(GetParentThing()), CThingFilter_In3DRange(GoodsPlaces[n].pos, BLOCKED_RADIUS)), &p_thing))
                continue;
        }
        places.push_back(GoodsPlaces[n]);
    }
    return places;
}
