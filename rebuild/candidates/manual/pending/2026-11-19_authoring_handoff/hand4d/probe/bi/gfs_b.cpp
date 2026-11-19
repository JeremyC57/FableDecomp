#define FABLE_RETAIL_BASE_OBJECT
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

std::vector<CTCShop::GoodsPlace> CTCShop::GetFreeSaleItemPlaces(bool allow_blocked)
{
    GoodsPlaces.clear();
    AddSaleItemPlacesFromThing(&GetParentThing());
    std::vector<GoodsPlace> places;
    for (long n = 0; n < GoodsPlaces.size(); ++n) {
        if (!allow_blocked) {
            const float BLOCKED_RADIUS = 0.2f;
            CThing* p_thing;
            if (PeekWorld().PeekThingSearchTools().GetPFirstThing(GFFilter_Bind2(CThingFilter_IsNot(GetParentThing()), CThingFilter_In3DRange(GoodsPlaces[n].pos, BLOCKED_RADIUS)), &p_thing))
                continue;
        }
        places.push_back(GoodsPlaces[n]);
    }
    return places;
}
