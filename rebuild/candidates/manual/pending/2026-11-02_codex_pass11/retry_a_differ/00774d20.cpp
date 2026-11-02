// CTCShop::BuyItem(long) -- buy an item into shop stock; display it and apply hero rewards.
// Leaf: ?BuyItem@CTCShop@@QAEXJ@Z
// Grade: DIFFER(1149v1149) -- saved-register roles (esi/edi/ebp swapped) and the donation minimum cached before the compare
// Context: grades against the Codex row context $S/codex11_dl/x/rows/00774d20/selected_context/hdr (Codex per-feature macros; NOT rebased on rebuild/include); source = retry11a probe p05; 00774d20 probes in probe/00774d20.log
#define FABLE_RETAIL_SHOP
#define FABLE_RETAIL_SHOP_PLACES
#define FABLE_RETAIL_SHOP_MAP_BODY
#define FABLE_RETAIL_QUEST_COMPLETION
#define FABLE_STLPORT_HTTP
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEFINITION_MANAGER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CPLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CHEROCOMBATDEF
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_TYPED_GETTC
#define FABLE_RETAIL_VISIBLE_LOWERBOUND
#define FABLE_RETAIL_FORCE_HASTC
#define FABLE_RETAIL_FORCE_GETTC
#include "engine/CTCShop.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CShopDef.h"
#include "engine/CStockItemDef.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCInventoryItem.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_In3DRange.h"
#include <math.h>
#pragma intrinsic(pow)
#pragma auto_inline(off)
namespace std {
template<class K,class V,class F,class C,class A> typename _Rb_tree<K,V,F,C,A>::iterator _Rb_tree<K,V,F,C,A>::find(const K& key) { return iterator(_M_find(key)); }
template<class K,class V,class C,class A> V& map<K,V,C,A>::operator[](const K& key) {
    iterator it=lower_bound(key);
    if(it==end() || key_comp()(key,(*it).first)) it=insert(it,value_type(key,V()));
    return (*it).second;
}
}
template<class T> bool CGameDefinitionManager::GetDef(long index,CDefPointer<T>& result)const {
    if(index>0) {
        CDefPointer<const CDefClassBase> def=GetPDefFromGlobalIndex(index);
        if(!def.IsNull()) {result.SetDynamicCast(def);return true;}
    }
    return false;
}
template<class T> bool CDefClassBase::GetSubDef(CDefPointer<const T>* result)const {
    const CSubDefInfo* info=GetPSubDefInfo(T::StatGetTypeName());
    if(info) {
        CDefPointer<const CDefClassBase> def=PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        result->SetDynamicCast(def);
        return true;
    }
    return false;
}

std::vector<CTCShop::GoodsPlace> CTCShop::GetFreeSaleItemPlaces(bool allow_blocked)
{
    GoodsPlaces.erase(GoodsPlaces.begin(),GoodsPlaces.end());
    AddSaleItemPlacesFromThing(&ParentThing);
    std::vector<GoodsPlace> places;
    for(long n=0;n<GoodsPlaces.size();++n) {
        if(!allow_blocked) {
            const float BLOCKED_RADIUS=0.2f;
            CThing* p_thing;
            if(PeekWorld().PeekThingSearchTools().GetPFirstThing(CFilter_Bind2<CThingFilter_IsNot,CThingFilter_In3DRange>(CThingFilter_IsNot(ParentThing),CThingFilter_In3DRange(GoodsPlaces[n].pos,BLOCKED_RADIUS)),&p_thing))continue;
        }
        places.push_back(GoodsPlaces[n]);
    }
    return places;
}

#pragma auto_inline(on)
void CTCShop::BuyItem(long def)
{
    if(Stock.find(def)==Stock.end()) {
        const CShopItem* pshop_item=FindDefaultStockItem(def);
        if(!pshop_item) return;
        Stock[def]=*pshop_item;
    }
    CDefPointer<const CShopDef> shop_def;
    ParentThing.PeekBaseDef().GetSubDef(&shop_def);
    Stock[def].Quantity+=1.0f;
    Stock[def].MaxStock+=shop_def->MaxStockChangePerBuy;
    if(Stock[def].MaxStock<0.0f) Stock[def].MaxStock=0.0f;
    CDefPointer<const CThingBaseDef> pdef;
    GetDefinitionManager().GetDef(ParentThing.GetDefGlobalIndex(),pdef);
    CDefPointer<const CShopDef> pshop_def;
    CDefPointer<const CStockItemDef> stock_item_def;
    CDefPointer<const CDefClassBase> def_class_base=GetDefinitionManager().GetPDefFromGlobalIndex(def);
    if(pdef->GetSubDef(&pshop_def) && def_class_base->GetSubDef(&stock_item_def)) {
        if(stock_item_def->DefaultPrice<=pshop_def->MaxPriceToDisplay && stock_item_def->CanBeDisplayedInShop) {
            std::vector<GoodsPlace> places=GetFreeSaleItemPlaces(false);
            if(places.size()>0) AddSaleItem(def,places[0]);
        }
    }
    CThingPlayerCreature* phero=CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    CTCHeroExperience* tc_hero_experience;
    if(phero && !phero->IsDead() && phero->GetTC(&tc_hero_experience)) {
        long bought_price=0;
        const NInventory::CTCInventoryBase* ptc_inventory_base;
        if(phero->PeekTC(CTCInventoryItem::GetInventoryType(def),&ptc_inventory_base)) {
            bought_price=ptc_inventory_base->GetBoughtPrice(def);
        }
        long profit=GetCurrentBuyPrice(def,1)-bought_price;
        if(profit>0) {
            long experience=(std::max)(profit/20,1L);
            tc_hero_experience->AddTrainableStatExperience(HERO_STAT_SKILL,experience);
        }
    }
    CTCHeroStats* ptc_stats;
    if(pshop_def->MaxGoldForDonation>0 && phero && !phero->IsDead() && phero->GetTC(&ptc_stats)) {
        float value=GetCurrentBasicPrice(def,1);
        GoldDonated=(long)((float)GoldDonated+value);
        if(value>=pshop_def->MinGoldForDonation) {
            if(value>pshop_def->MaxGoldForDonation) value=(float)pshop_def->MaxGoldForDonation;
            float max_value=(float)(pshop_def->MaxGoldForDonation-pshop_def->MinGoldForDonation+1);
            float max_log_value=(float)pow((double)max_value,(double)pshop_def->LogarithmicPower);
            float morality_adjustment=pshop_def->MaxMoralityForDonation/max_log_value;
            float log_value=(float)pow((double)(value-(pshop_def->MinGoldForDonation-1)),(double)pshop_def->LogarithmicPower);
            float morality=morality_adjustment*log_value;
            float immorality=1.0f-ptc_stats->GetMoralityPercentage();
            float new_morality=1.0f-immorality*(1.0f-morality);
            long max_morality=ptc_stats->GetMoralityMax();
            ptc_stats->SetMorality((long)(ceil((double)(max_morality*2)*new_morality)-max_morality));
        }
    }
}
