// Behaviour test for 0x00774D20 CTCShop::BuyItem(long def): the row's defines, includes and body (with its same-TU
// GetFreeSaleItemPlaces) verbatim over the shared headers, on zeroed CTCShop / CThing / CThingPlayerCreature images with a real
// STLport Stock map and real interface maps (UsingComponents + Components). Doubles: the def lookups (GetSubDef<CShopDef> /
// <CStockItemDef> and GetDef<CThingBaseDef> specialisations, GetPDefFromGlobalIndex), FindDefaultStockItem, AddSaleItem,
// AddSaleItemPlacesFromThing / PeekWorld / GetPFirstThing, the player chain, the inventory / experience / stats TCs, the price
// functions. Covers: early return without a default item; the default copied in; quantity +1; max stock += change, clamped at 0;
// display gates (sub defs, price limit, displayable, a free place) and AddSaleItem(def, places[0]); bought price from the inventory
// TC or 0, profit, experience max(profit/20, 1) to SKILL only when profit > 0; donation gates (MaxGold > 0, hero alive, stats TC),
// GoldDonated, the Min gate (>=), the Max clamp, and the logarithmic morality formula with exact power-of-two values.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
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
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
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

// ---- fakes the specialisations below need (declared before the row body) ----
static double g_shopdef_store[(sizeof(CShopDef) + 7) / 8];
static double g_itemdef_store[(sizeof(CStockItemDef) + 7) / 8];
static double g_classdef_store[64];             // the item's CDefClassBase (GetPDefFromGlobalIndex(def))
static double g_thingdef_store[64];             // pdef (GetDef(parent's global index))
static double g_basedef_store[64];              // the parent thing's PeekBaseDef()
static int g_shop_subdef = 1, g_item_subdef = 1, g_getdef_ok = 1;
static int g_subdef_calls_base = 0, g_subdef_calls_pdef = 0, g_subdef_calls_item = 0;
static long g_getdef_index = -1;
static void set_def(CDefPointeeBase*& obj, void* p) { obj = (CDefPointeeBase*)p; ++((CDefPointeeBase*)p)->IVRefCount; }
template<> bool CDefClassBase::GetSubDef<CShopDef>(CDefPointer<const CShopDef>* def) const
{
    if (this == (const CDefClassBase*)g_basedef_store) ++g_subdef_calls_base;
    else if (this == (const CDefClassBase*)g_thingdef_store) ++g_subdef_calls_pdef;
    else return false;
    if (!g_shop_subdef) return false;
    set_def(def->Object, g_shopdef_store);
    return true;
}
template<> bool CDefClassBase::GetSubDef<CStockItemDef>(CDefPointer<const CStockItemDef>* def) const
{
    if (this != (const CDefClassBase*)g_classdef_store) return false;
    ++g_subdef_calls_item;
    if (!g_item_subdef) return false;
    set_def(def->Object, g_itemdef_store);
    return true;
}
template<> bool CDefinitionManager::GetDef<CThingBaseDef>(long index, CDefPointer<const CThingBaseDef>* out) const
{
    g_getdef_index = index;
    if (!g_getdef_ok) return false;
    set_def(out->Object, g_thingdef_store);
    return true;
}

// Fidelity: same-TU strong definition of 0x00774B40 (retail TU-mate)
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

void CTCShop::BuyItem(long def)
{
    if (Stock.find(def) == Stock.end()) {
        const CShopItem* pshop_item = FindDefaultStockItem(def);
        if (!pshop_item)
            return;
        Stock[def] = *pshop_item;
    }
    CDefPointer<const CShopDef> shop_def;
    GetParentThing().PeekBaseDef().GetSubDef(&shop_def);
    Stock[def].Quantity += 1.0f;
    Stock[def].MaxStock += shop_def->MaxStockChangePerBuy;
    if (Stock[def].MaxStock < 0.0f)
        Stock[def].MaxStock = 0.0f;
    CDefPointer<const CThingBaseDef> pdef;
    GetDefinitionManager().GetDef(ParentThing->GetDefGlobalIndex(), &pdef);
    CDefPointer<const CShopDef> pshop_def;
    CDefPointer<const CDefClassBase> def_class_base = GetDefinitionManager().GetPDefFromGlobalIndex(def);
    CDefPointer<const CStockItemDef> stock_item_def;
    if (pdef->GetSubDef(&pshop_def) && def_class_base->GetSubDef(&stock_item_def)
        && stock_item_def->DefaultPrice <= pshop_def->MaxPriceToDisplay
        && stock_item_def->CanBeDisplayedInShop) {
        std::vector<GoodsPlace> places = GetFreeSaleItemPlaces(false);
        if (places.size() > 0)
            AddSaleItem(def, places[0]);
    }
    CThingPlayerCreature* phero = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    CTCHeroExperience* tc_hero_experience;
    if (IS_THING_ALIVE(phero) && phero->GetTC(&tc_hero_experience)) {
        long bought_price = 0;
        const NInventory::CTCInventoryBase* ptc_inventory_base;
        if (phero->PeekTC(CTCInventoryItem::GetInventoryType(def), &ptc_inventory_base))
            bought_price = ptc_inventory_base->GetBoughtPrice(def);
        long profit = GetCurrentBuyPrice(def, 1) - bought_price;
        if (profit > 0) {
            long experience = (std::max)(profit / 20, 1L);
            tc_hero_experience->AddTrainableStatExperience(HERO_STAT_SKILL, experience);
        }
    }
    CTCHeroStats* ptc_stats;
    if (pshop_def->MaxGoldForDonation > 0 && IS_THING_ALIVE(phero) && phero->GetTC(&ptc_stats)) {
        float value = GetCurrentBasicPrice(def, 1);
        GoldDonated = (long)((float)GoldDonated + value);
        if (value >= pshop_def->MinGoldForDonation) {
            if (value > pshop_def->MaxGoldForDonation) {
                value = (float)pshop_def->MaxGoldForDonation;
            }
            value -= pshop_def->MinGoldForDonation - 1;
            float max_value = (float)(pshop_def->MaxGoldForDonation + 1 - pshop_def->MinGoldForDonation);
            float max_log_value = GFPower(max_value, pshop_def->LogarithmicPower);
            float log_value = GFPower(value, pshop_def->LogarithmicPower);
            float morality_adjustment = pshop_def->MaxMoralityForDonation / max_log_value;
            float morality = (morality_adjustment * log_value);    // Bracket: see the header
            float immorality = 1.0f - ptc_stats->GetMoralityPercentage();
            float new_morality = 1.0f - immorality * (1.0f - morality);
            long max_morality = ptc_stats->GetMoralityMax();
            ptc_stats->SetMorality((long)(ceil((double)(max_morality * 2) * new_morality) - max_morality));
        }
    }
}

// ---- doubles (out-of-line callees of the row) ----
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
CShopItem::CShopItem() { memset(this, 0, sizeof *this); }
static CShopItem g_default_item;
static const CShopItem* g_default_ptr = 0;
static int g_find_default_calls = 0;
const CShopItem* CTCShop::FindDefaultStockItem(long d) const { ++g_find_default_calls; return g_default_ptr; }
static double g_mgr_store[64];
static int g_getdefmgr_calls = 0;
const CGameDefinitionManager& CTCBase::GetDefinitionManager() const { ++g_getdefmgr_calls; return *(const CGameDefinitionManager*)g_mgr_store; }
static long g_pdef_index = -1;
CDefPointer<const CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long index) const
{
    g_pdef_index = index;
    CDefPointer<const CDefClassBase> p;
    set_def(p.Object, g_classdef_store);
    return p;
}
CDefPointer<CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long index) { CDefPointer<CDefClassBase> p; return p; }
// GetFreeSaleItemPlaces' callees
static int g_place_count = 2;
void CTCShop::AddSaleItemPlacesFromThing(const CThing* thing)
{
    for (int i = 0; i < g_place_count; ++i) {
        GoodsPlace p; memset(&p, 0, sizeof p);
        p.pos.X = 10.0f * (i + 1);
        GoodsPlaces.push_back(p);
    }
}
static double g_world_store[(sizeof(CWorld) + 7) / 8];
const CWorld& CTCBase::PeekWorld() const { return *(const CWorld*)g_world_store; }
static int g_search_calls = 0;
template<class Filter, class T> bool CThingSearchTools::GetPFirstThing(const Filter& filter, T** out) const { ++g_search_calls; return false; }
static int g_addsale_calls = 0; static long g_addsale_def = -1; static float g_addsale_x = 0.0f;
void CTCShop::AddSaleItem(long goods, const GoodsPlace& place) { ++g_addsale_calls; g_addsale_def = goods; g_addsale_x = place.pos.X; }
// the hero
static double g_player_store[64];
static CThingPlayerCreature* g_hero = 0;
CPlayer& CPlayerManager::GetMainPlayer() { return *(CPlayer*)g_player_store; }
CThingPlayerCreature* CPlayer::GetPControlledCreature() const { return g_hero; }
static ETCInterfaceType g_inv_type = (ETCInterfaceType)50;
static long g_invtype_def = -1;
ETCInterfaceType __fastcall CTCInventoryItem::GetInventoryType(long d) { g_invtype_def = d; return g_inv_type; }
static long g_bought = 0, g_bought_def = -1;
long NInventory::CTCInventoryBase::GetBoughtPrice(long d) const { g_bought_def = d; return g_bought; }
static long g_buy_price = 0, g_buy_def = -1, g_buy_count = -1;
long CTCShop::GetCurrentBuyPrice(long d, long n) const { g_buy_def = d; g_buy_count = n; return g_buy_price; }
static int g_xp_calls = 0; static long g_xp = 0; static int g_xp_cat = -1;
void CTCHeroExperience::AddTrainableStatExperience(EHeroExperienceStatCategory c, long x) { ++g_xp_calls; g_xp = x; g_xp_cat = (int)c; }
static float g_basic_price = 0.0f; static int g_basic_calls = 0; static long g_basic_def = -1, g_basic_count = -1;
float CTCShop::GetCurrentBasicPrice(long d, long n) const { ++g_basic_calls; g_basic_def = d; g_basic_count = n; return g_basic_price; }
static float g_pct = 0.5f; static long g_maxmor = 100; static int g_setmor_calls = 0; static long g_setmor = 0;
float CTCHeroStats::GetMoralityPercentage() const { return g_pct; }
long CTCHeroStats::GetMoralityMax() const { return g_maxmor; }
void CTCHeroStats::SetMorality(long m) { ++g_setmor_calls; g_setmor = m; }
// STLport _Rb_global<bool>::_Rebalance (out of line in retail): the test's maps hold one key, so no recolouring / rotation is needed;
// the root is coloured black as the real body does.
namespace std { template<class Dummy> void __cdecl _Rb_global<Dummy>::_Rebalance(_Rb_tree_node_base* x, _Rb_tree_node_base*& root) { root->_M_color = true; } }   // black (STLport: red = false)
// ---- harness ----
CMainGameComponent* CMainGameComponent::PMainGameComponent = 0;
static double g_shop_store[(sizeof(CTCShop) + 7) / 8];
static double g_thing_store[(sizeof(CThing) + 7) / 8];
static double g_mgc_store[(sizeof(CMainGameComponent) + 7) / 8];
static double g_pm_store[64];
static double g_hero_store[(sizeof(CThingPlayerCreature) + 7) / 8];
static double g_xptc_store[16], g_stats_store[16], g_inv_store[16];
static CTCShop* g_shop = 0;
static void add_tc(CThing* t, ETCInterfaceType type, void* tc)
{
    ((unsigned long*)&t->UsingComponents)[type >> 5] |= 1ul << (type & 31);
    std::pair<ETCInterfaceType, CTCBase*> e(type, (CTCBase*)tc);
    size_t i = 0;
    while (i < t->Components.size() && t->Components[i].first < type) ++i;
    t->Components.insert(t->Components.begin() + i, e);
}
struct Cfg {
    int stocked; int has_default; float default_qty, default_max; float change;
    int shop_subdef, item_subdef; long price, max_display; int displayable; int places;
    int hero, hero_dead, has_xp, has_inv; long bought, buy_price;
    long max_gold, min_gold; float basic, maxmor, logpow; int has_stats;
};
static void reset_counts()
{
    g_find_default_calls = g_getdefmgr_calls = g_search_calls = g_addsale_calls = g_xp_calls = g_basic_calls = g_setmor_calls = 0;
    g_subdef_calls_base = g_subdef_calls_pdef = g_subdef_calls_item = 0;
    g_getdef_index = g_pdef_index = g_addsale_def = g_invtype_def = g_bought_def = g_buy_def = g_buy_count = g_basic_def = g_basic_count = -1;
    g_xp = 0; g_xp_cat = -1; g_setmor = -12345; g_addsale_x = 0.0f;
}
static void setup(const Cfg& c)
{
    reset_counts();
    memset(g_shop_store, 0, sizeof g_shop_store);
    g_shop = (CTCShop*)g_shop_store;
    new (&g_shop->Stock) std::map<long, CShopItem>();
    memset(g_thing_store, 0, sizeof g_thing_store);
    CThing* parent = (CThing*)g_thing_store;
    set_def(parent->PBaseDef.Object, g_basedef_store);
    parent->DefGlobalIndex = 77;
    g_shop->ParentThing = parent;
    g_shop->GoldDonated = 1000;
    if (c.stocked) { CShopItem it; it.Quantity = 5.0f; it.MaxStock = 9.0f; it.RestockPeriod = 3; g_shop->Stock[42] = it; }
    memset(&g_default_item, 0, sizeof g_default_item);
    g_default_item.Quantity = c.default_qty; g_default_item.MaxStock = c.default_max; g_default_item.RestockPeriod = 11; g_default_item.SalesPerDay = 2.5f;
    g_default_ptr = c.has_default ? &g_default_item : 0;
    CShopDef* sd = (CShopDef*)g_shopdef_store;
    sd->MaxStockChangePerBuy = c.change; sd->MaxPriceToDisplay = (float)c.max_display;
    sd->MinGoldForDonation = c.min_gold; sd->MaxGoldForDonation = c.max_gold; sd->MaxMoralityForDonation = c.maxmor; sd->LogarithmicPower = c.logpow;
    CStockItemDef* id = (CStockItemDef*)g_itemdef_store; id->DefaultPrice = c.price; id->CanBeDisplayedInShop = c.displayable != 0;
    g_shop_subdef = c.shop_subdef; g_item_subdef = c.item_subdef; g_place_count = c.places;
    memset(g_mgc_store, 0, sizeof g_mgc_store);
    CMainGameComponent::PMainGameComponent = (CMainGameComponent*)g_mgc_store;
    *(CPlayerManager**)((CMainGameComponent*)g_mgc_store)->PPlayerManager = (CPlayerManager*)g_pm_store;
    memset(g_hero_store, 0, sizeof g_hero_store);
    CThingPlayerCreature* h = (CThingPlayerCreature*)g_hero_store;
    g_hero = c.hero ? h : 0;
    if (c.hero) {
        h->Dead = c.hero_dead ? 1 : 0;
        if (c.has_stats) add_tc(h, CTCHeroStats::StatGetInterfaceType(), g_stats_store);
        if (c.has_xp) add_tc(h, CTCHeroExperience::StatGetInterfaceType(), g_xptc_store);
        if (c.has_inv) add_tc(h, g_inv_type, g_inv_store);
    }
    g_bought = c.bought; g_buy_price = c.buy_price; g_basic_price = c.basic;
}
static void init_defs()
{
    double* s[] = { g_shopdef_store, g_itemdef_store, g_classdef_store, g_thingdef_store, g_basedef_store };
    for (int i = 0; i < 5; ++i) { memset(s[i], 0, 8 * 16); ((CDefPointeeBase*)s[i])->IVRefCount = 1000; }
}

int main()
{
    init_defs();
    // 1. not stocked, no default stock item: returns at once
    { Cfg c = { 0, 0, 0, 0, 1.0f, 1, 1, 10, 50, 1, 2, 1, 0, 1, 1, 30, 100, 120, 50, 150.0f, 0.5f, 1.0f, 1 };
      setup(c); g_shop->BuyItem(42);
      check(g_find_default_calls == 1 && g_shop->Stock.size() == 0, "1: unknown item without default stock: nothing stocked");
      check(g_getdefmgr_calls == 0 && g_xp_calls == 0 && g_basic_calls == 0 && g_shop->GoldDonated == 1000, "1: early return"); }
    // 2. not stocked: default copied in; quantity +1; max stock change negative -> clamped to 0; displayed on the first of 2 places;
    //    profit 100-30 = 70 -> 3 experience; donation 150 clamped to Max 113, morality 0.5, percentage 0.5 -> 60
    { Cfg c = { 0, 1, 2.0f, 1.0f, -3.0f, 1, 1, 10, 50, 1, 2, 1, 0, 1, 1, 30, 100, 113, 50, 150.0f, 0.5f, 1.0f, 1 };
      setup(c); g_shop->BuyItem(42);
      CShopItem& it = g_shop->Stock[42];
      check(g_shop->Stock.size() == 1 && it.Quantity == 3.0f && it.RestockPeriod == 11 && it.SalesPerDay == 2.5f, "2: default copied, quantity +1");
      check(it.MaxStock == 0.0f, "2: max stock clamped at 0");
      check(g_subdef_calls_base == 1 && g_getdef_index == 77 && g_subdef_calls_pdef == 1 && g_pdef_index == 42 && g_subdef_calls_item == 1, "2: defs looked up");
      check(g_search_calls == 2 && g_addsale_calls == 1 && g_addsale_def == 42 && g_addsale_x == 10.0f, "2: displayed on the first free place");
      check(g_invtype_def == 42 && g_bought_def == 42 && g_buy_def == 42 && g_buy_count == 1, "2: prices of def");
      check(g_xp_calls == 1 && g_xp == 3 && g_xp_cat == (int)HERO_STAT_SKILL, "2: skill experience profit/20");
      check(g_basic_calls == 1 && g_basic_def == 42 && g_basic_count == 1 && g_shop->GoldDonated == 1150, "2: gold donated");
      check(g_setmor_calls == 1 && g_setmor == 50, "2: morality (clamped value)"); }
    // 3. stocked; change +2 (no clamp); price above the display limit; profit 10 -> 1 experience (minimum);
    //    donation 81 between Min 50 and Max 113: morality 0.25, percentage 0.5 -> new 0.625 -> ceil(125) - 100 = 25
    { Cfg c = { 1, 1, 2.0f, 1.0f, 2.0f, 1, 1, 60, 50, 1, 2, 1, 0, 1, 1, 90, 100, 113, 50, 81.0f, 0.5f, 1.0f, 1 };
      setup(c); g_shop->BuyItem(42);
      CShopItem& it = g_shop->Stock[42];
      check(g_find_default_calls == 0 && it.Quantity == 6.0f && it.MaxStock == 11.0f && it.RestockPeriod == 3, "3: stocked item updated in place");
      check(g_addsale_calls == 0 && g_search_calls == 0, "3: too dear to display");
      check(g_xp_calls == 1 && g_xp == 1, "3: minimum one experience");
      check(g_shop->GoldDonated == 1081 && g_setmor_calls == 1 && g_setmor == 25, "3: morality (unclamped value)"); }
    // 10 (audit): a non-integral product: value 61 -> morality 12/64 * 0.5 = 0.09375, new 0.546875 -> ceil(109.375) - 100 = 10 (floor: 9)
    { Cfg c = { 1, 1, 2.0f, 1.0f, 2.0f, 1, 1, 60, 50, 1, 2, 1, 0, 1, 1, 90, 100, 113, 50, 61.0f, 0.5f, 1.0f, 1 };
      setup(c); g_shop->BuyItem(42);
      check(g_setmor_calls == 1 && g_setmor == 10, "10: ceil of a non-integral morality"); }
    // 4. not displayable; no inventory TC (bought 0) and loss (profit <= 0); value below Min: gold donated, no morality
    { Cfg c = { 1, 1, 0, 0, 0.0f, 1, 1, 10, 50, 0, 2, 1, 0, 1, 0, 30, 0, 113, 50, 49.0f, 0.5f, 1.0f, 1 };
      setup(c); g_shop->BuyItem(42);
      check(g_addsale_calls == 0, "4: not displayable");
      check(g_bought_def == -1 && g_buy_def == 42 && g_xp_calls == 0, "4: no profit, no experience");
      check(g_shop->GoldDonated == 1049 && g_setmor_calls == 0, "4: below Min: donation counted, morality untouched"); }
    // 5. value exactly Min: morality applied (>=); max gold 0 path tested next
    { Cfg c = { 1, 1, 0, 0, 0.0f, 1, 1, 10, 50, 1, 0, 1, 0, 1, 1, 30, 40, 113, 50, 50.0f, 0.5f, 1.0f, 1 };
      setup(c); g_shop->BuyItem(42);
      check(g_addsale_calls == 0 && g_search_calls == 0, "5: no free place");
      check(g_xp_calls == 1 && g_xp == 1, "5: profit 10 -> 1");
      check(g_setmor_calls == 1, "5: value == Min donates morality"); }
    // 6. no donations (MaxGold 0), hero dead: nothing for the hero
    { Cfg c = { 1, 1, 0, 0, 0.0f, 1, 1, 10, 50, 1, 2, 1, 1, 1, 1, 30, 100, 0, 50, 150.0f, 0.5f, 1.0f, 1 };
      setup(c); g_shop->BuyItem(42);
      check(g_addsale_calls == 1 && g_xp_calls == 0 && g_basic_calls == 0 && g_shop->GoldDonated == 1000, "6: dead hero / no donations"); }
    // 7. MaxGold 0 with a live hero: no donation; shop def missing: no display, no donation
    { Cfg c = { 1, 1, 0, 0, 0.0f, 1, 1, 10, 50, 1, 2, 1, 0, 1, 1, 30, 100, 0, 50, 150.0f, 0.5f, 1.0f, 1 };
      setup(c); g_shop->BuyItem(42);
      check(g_xp_calls == 1 && g_basic_calls == 0, "7: MaxGoldForDonation 0: no donation"); }
    { Cfg c = { 1, 1, 0, 0, 0.0f, 1, 0, 10, 50, 1, 2, 1, 0, 1, 1, 30, 100, 113, 50, 150.0f, 0.5f, 1.0f, 1 };
      setup(c); g_shop->BuyItem(42);
      check(g_subdef_calls_item == 1 && g_addsale_calls == 0, "7: no stock item def: no display"); }
    // 8. no hero; no stats TC
    { Cfg c = { 1, 1, 0, 0, 0.0f, 1, 1, 10, 50, 1, 2, 0, 0, 1, 1, 30, 100, 113, 50, 150.0f, 0.5f, 1.0f, 1 };
      setup(c); g_shop->BuyItem(42);
      check(g_xp_calls == 0 && g_basic_calls == 0, "8: no hero"); }
    { Cfg c = { 1, 1, 0, 0, 0.0f, 1, 1, 10, 50, 1, 2, 1, 0, 0, 1, 30, 100, 113, 50, 150.0f, 0.5f, 1.0f, 0 };
      setup(c); g_shop->BuyItem(42);
      check(g_xp_calls == 0 && g_basic_calls == 0, "8: no experience / stats TCs"); }
    // 9. exactly one free place; no inventory TC with profit 40 -> 2 experience (bought price 0); MaxMorality 1.0 with the clamp:
    //    morality 64/64 = 1 -> new 1 -> ceil(200) - 100 = 100 (value - (Min - 1), not value - Min)
    { Cfg c = { 1, 1, 0, 0, 0.0f, 1, 1, 10, 50, 1, 1, 1, 0, 1, 0, 30, 40, 113, 50, 150.0f, 1.0f, 1.0f, 1 };
      setup(c); g_shop->BuyItem(42);
      check(g_addsale_calls == 1 && g_addsale_x == 10.0f, "9: one free place is used");
      check(g_xp_calls == 1 && g_xp == 2, "9: bought price 0 without an inventory TC");
      check(g_setmor_calls == 1 && g_setmor == 100, "9: morality uses value - (Min - 1)"); }
    if (!g_fail) printf("00774d20_TEST PASS\n");
    return g_fail;
}