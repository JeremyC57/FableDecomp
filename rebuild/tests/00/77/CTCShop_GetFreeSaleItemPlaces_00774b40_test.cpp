// Behaviour test for 0x00774B40 CTCShop::GetFreeSaleItemPlaces(bool allow_blocked): the row's defines, includes and body verbatim over
// the shared headers, run on a zeroed CTCShop image whose GoodsPlaces holds stale entries. Covers: the old places cleared before the
// rebuild (not appended to), AddSaleItemPlacesFromThing called once per call with the parent thing; with allow_blocked == false every
// place searched with the IsNot(parent) / In3DRange(place pos, 0.2) pair through PeekWorld's search tools and the blocked ones skipped
// (first, middle, last); with allow_blocked == true no search and every place returned, in order, whole (position and orientation).
// Doubles: AddSaleItemPlacesFromThing, CTCBase::PeekWorld (a zeroed CWorld whose search-tools pointer is set), the GetPFirstThing
// member template (generic definition after the body).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define private public
#define protected public
// ---- row body ----
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

// ---- doubles ----
static void check_tools(const CThingSearchTools* t);
static int g_add_calls = 0;
static const CThing* g_add_thing = 0;
static int g_place_count = 4;
void CTCShop::AddSaleItemPlacesFromThing(const CThing* thing)
{
    ++g_add_calls; g_add_thing = thing;
    for (int i = 0; i < g_place_count; ++i) {
        GoodsPlace p; memset(&p, 0, sizeof p);
        p.pos.X = 10.0f * (i + 1); p.pos.Y = 1.0f; p.pos.Z = 2.0f;
        *(float*)&p.rhset = 100.0f + i;          // first word of the orientation
        GoodsPlaces.push_back(p);
    }
}
static double g_world_store[(sizeof(CWorld) + 7) / 8];
static double g_tools_store[(sizeof(CThingSearchTools) + 7) / 8];
const CWorld& CTCBase::PeekWorld() const { return *(const CWorld*)g_world_store; }
static int g_search_calls = 0;
static unsigned g_blocked_mask = 0;          // bit i: place i blocked
static const CThing* g_seen_not = 0;
static float g_seen_range = 0.0f;
static CThing* g_found = (CThing*)0x1234;
template<class Filter, class T> bool CThingSearchTools::GetPFirstThing(const Filter& filter, T** out) const
{
    ++g_search_calls;
    check_tools(this);
    g_seen_not = *(const CThing* const*)&filter.F1;   // CThingFilter_IsNot: one const CThing& (default-private)
    g_seen_range = filter.F2.Range;
    int i = (int)(filter.F2.Pos.X / 10.0f) - 1;
    if (filter.F2.Pos.Y != 1.0f || filter.F2.Pos.Z != 2.0f) i = -100;
    if (i >= 0 && (g_blocked_mask >> i & 1)) { *out = (T*)g_found; return true; }
    return false;
}
// ---- harness ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static void check_tools(const CThingSearchTools* t) { if (t != (const CThingSearchTools*)g_tools_store) { printf("FAIL search tools not the world's\n"); g_fail = 1; } }

int main()
{
    memset(g_world_store, 0, sizeof g_world_store); memset(g_tools_store, 0, sizeof g_tools_store);
    *(CThingSearchTools**)&((CWorld*)g_world_store)->PThingSearchTools_ptr = (CThingSearchTools*)g_tools_store;
    static double shop_store[(sizeof(CTCShop) + 7) / 8];
    memset(shop_store, 0, sizeof shop_store);
    CTCShop& shop = *(CTCShop*)shop_store;
    static double thing_store[64];
    CThing* parent = (CThing*)thing_store;
    shop.ParentThing = parent;
    { CTCShop::GoodsPlace stale; memset(&stale, 0, sizeof stale); stale.pos.X = 999.0f; shop.GoodsPlaces.push_back(stale); shop.GoodsPlaces.push_back(stale); }

    g_blocked_mask = 0x9;                    // places 0 and 3 blocked
    std::vector<CTCShop::GoodsPlace> free_places = shop.GetFreeSaleItemPlaces(false);
    check(g_add_calls == 1 && g_add_thing == parent, "AddSaleItemPlacesFromThing(parent) once");
    check(shop.GoodsPlaces.size() == 4, "stale places cleared before the rebuild");
    check(g_search_calls == 4, "one search per place");
    check(g_seen_not == parent, "IsNot filter on the parent thing");
    check(g_seen_range == 0.2f, "In3DRange radius 0.2");
    check(free_places.size() == 2, "two free places");
    if (free_places.size() == 2) {
        check(free_places[0].pos.X == 20.0f && free_places[1].pos.X == 30.0f, "free places in order (1, 2)");
        check(*(float*)&free_places[0].rhset == 101.0f && *(float*)&free_places[1].rhset == 102.0f, "places copied whole");
    }

    g_blocked_mask = 0x6;                    // middle ones blocked
    free_places = shop.GetFreeSaleItemPlaces(false);
    check(free_places.size() == 2 && free_places[0].pos.X == 10.0f && free_places[1].pos.X == 40.0f, "middle places skipped");

    g_search_calls = 0;
    g_blocked_mask = 0xF;
    std::vector<CTCShop::GoodsPlace> all = shop.GetFreeSaleItemPlaces(true);
    check(g_search_calls == 0, "allow_blocked: no search");
    check(all.size() == 4 && all[0].pos.X == 10.0f && all[3].pos.X == 40.0f, "allow_blocked: every place");
    check(shop.GoodsPlaces.size() == 4 && g_add_calls == 3, "rebuilt each call, not appended");

    g_place_count = 0;
    all = shop.GetFreeSaleItemPlaces(false);
    check(all.size() == 0 && shop.GoodsPlaces.size() == 0, "no places");

    if (!g_fail) printf("00774b40_TEST PASS\n");
    return g_fail;
}
