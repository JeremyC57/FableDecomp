// CTCInventoryClothing::CalculateArmorValues(ArgPasser&,long) const -- compare prospective clothing with worn items, including defaults and shop prices.
// Leaf: ?CalculateArmorValues@CTCInventoryClothing@@ABEXAAUArgPasser@1@J@Z
// Flags:
// Retail: 0x005b4eba,3372 bytes,thiscall,ret8.
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_HEROSTATSDEF
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_ARMOUR
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_CALCULATE
#define FABLE_RETAIL_CLOTHING_DEFS
#define FABLE_RETAIL_CHEROSTATSDEF_APPEARANCESCALING
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_LIST
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCSHOP_VALIDSTOCK
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryClothing.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefPointer.h"
#include "engine/CHeroSuitDef.h"
#include "engine/CAppearanceModifierDef.h"
#include "engine/CArmourDef.h"
#include "engine/CInventoryItemDef.h"
#include "engine/CStockItemDef.h"
#include "engine/CHeroStatsDef.h"
#include "engine/CTCHeroAttachableAppearanceModifiers.h"
#include "engine/CTCVolumeContainmentTracker.h"
#include "engine/CTCShop.h"
template<class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T&);
template<> bool FABLE_FASTCALL IS_THING_ALIVE<const CThing*>(const CThing* const& thing)
{
    if(thing && !thing->IsDead())return true;
    return false;
}
template<> bool CDefinitionManager::GetDef<CThingBaseDef>(long index,CDefPointer<const CThingBaseDef>* output)const
{
    if(index<=0)return false;
    CDefPointer<const CDefClassBase> definition=GetPDefFromGlobalIndex(index);
    if(definition.IsNull())return false;
    output->SetDynamicCast(definition);
    return true;
}
template<> bool CDefClassBase::GetSubDef<CHeroSuitDef>(CDefPointer<const CHeroSuitDef>* output)const
{
    const CSubDefInfo* info=GetPSubDefInfo("CHeroSuitDef");
    if(!info)return false;
    CDefPointer<const CHeroSuitDef> definition=PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
    *output=definition;
    return true;
}
template<> bool CDefClassBase::GetSubDef<CInventoryItemDef>(CDefPointer<const CInventoryItemDef>* output)const
{
    const CSubDefInfo* info=GetPSubDefInfo("CInventoryItemDef");
    if(!info)return false;
    CDefPointer<const CInventoryItemDef> definition=PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
    *output=definition;
    return true;
}
template<> bool CDefClassBase::GetSubDef<CAppearanceModifierDef>(CDefPointer<const CAppearanceModifierDef>* output)const
{
    const CSubDefInfo* info=GetPSubDefInfo("CAppearanceModifierDef");
    if(!info)return false;
    CDefPointer<const CAppearanceModifierDef> definition=PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
    *output=definition;
    return true;
}
template<> bool CDefClassBase::GetSubDef<CStockItemDef>(CDefPointer<const CStockItemDef>* output)const
{
    const CSubDefInfo* info=GetPSubDefInfo("CStockItemDef");
    if(!info)return false;
    CDefPointer<const CStockItemDef> definition=PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
    *output=definition;
    return true;
}
void CTCInventoryClothing::CalculateArmorValues(ArgPasser& values,long itemDefIndex) const
{
    values.is_suit=false;
    values.num_suit_parts=0;
    values.max_suit_parts=0;
    std::vector<long> items;
    CDefPointer<const CThingBaseDef> baseDef;
    GetDefinitionManager().GetDef(itemDefIndex,&baseDef);
    CDefPointer<const CHeroSuitDef> suit;
    if(baseDef->GetSubDef(&suit)) {
        values.is_suit=true;
        std::set<EClothingSuitPart> uncovered;
        uncovered.insert(CLOTHING_SUIT_HEAD);
        uncovered.insert(CLOTHING_SUIT_BODY);
        uncovered.insert(CLOTHING_SUIT_HANDS);
        uncovered.insert(CLOTHING_SUIT_LEGS);
        uncovered.insert(CLOTHING_SUIT_FEET);
        for(std::map<EClothingSuitPart,long>::const_iterator it=suit->SuitParts.begin();it!=suit->SuitParts.end();++it) {
            uncovered.erase(it->first);
            if(itemDefIndex==GetDefinitionManager().GetDefGlobalIndexFromName("HERO_SUIT_NAKED") ||
               (it->second!=GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_HAT") &&
                it->second!=GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_SHIRT") &&
                it->second!=GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_GLOVES") &&
                it->second!=GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_TROUSERS") &&
                it->second!=GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_BOOTS")))
                ++values.max_suit_parts;
            if(GetNumberOfItemsOfTypeInInventory(it->second)>0) {
                if(itemDefIndex==GetDefinitionManager().GetDefGlobalIndexFromName("HERO_SUIT_NAKED") ||
                   (it->second!=GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_HAT") &&
                    it->second!=GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_SHIRT") &&
                    it->second!=GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_GLOVES") &&
                    it->second!=GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_TROUSERS") &&
                    it->second!=GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_BOOTS")))
                    ++values.num_suit_parts;
                items.push_back(it->second);
            } else {
                CDefPointer<const CDefClassBase> definition=GetDefinitionManager().GetPDefFromGlobalIndex(it->second);
                CDefPointer<const CInventoryItemDef> inventory;
                if(definition->GetSubDef(&inventory) && inventory->ItemToSelectUponRemoval>0)
                    items.push_back(inventory->ItemToSelectUponRemoval);
            }
        }
        for(std::set<EClothingSuitPart>::iterator it=uncovered.begin(),end=uncovered.end();it!=end;++it) {
            long index=0;
            switch(*it) {
            case 1:index=GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_HAT");break;
            case 2:index=GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_SHIRT");break;
            case 3:index=GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_GLOVES");break;
            case 4:index=GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_TROUSERS");break;
            case 5:index=GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_BOOTS");break;
            }
            items.push_back(index);
        }
    } else items.push_back(itemDefIndex);
    values.total_item_attractiveness_value_final=0.0f;
    values.total_cur_attractiveness_value_final=0.0f;
    values.total_item_armor_value_final=0;
    values.total_cur_armor_value_final=0;
    values.total_price=0;
    values.total_item_scariness_value_final=0.0f;
    values.total_cur_scariness_value_final=0.0f;
    values.total_item_goodstrength_value_final=0.0f;
    values.total_cur_goodstrength_value_final=0.0f;
    const CHeroStatsDef* stats=GetDefinitionManager().PHeroStatsDef;
    const CTCVolumeContainmentTracker* tracker=0;
    ParentThing->PeekTC(TCI_VOLUME_CONTAINMENT_TRACKER,&tracker);
    std::set<EClothingSuitPart> uncovered;
    uncovered.insert(CLOTHING_SUIT_HEAD);
    uncovered.insert(CLOTHING_SUIT_BODY);
    uncovered.insert(CLOTHING_SUIT_HANDS);
    uncovered.insert(CLOTHING_SUIT_LEGS);
    uncovered.insert(CLOTHING_SUIT_FEET);
    std::list<long> worn;
    std::list<CDefPointer<const CAppearanceModifierDef> > modifiers;
    const CTCHeroAttachableAppearanceModifiers* appearance=0;
    if(ParentThing->PeekTC(TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS,&appearance)) {
        appearance->GetAppearanceModifierList(worn,APPEARANCE_CLOTHING);
        for(std::list<long>::const_iterator it=worn.begin(),end=worn.end();it!=end;++it) {
            CDefPointer<const CDefClassBase> definition=GetDefinitionManager().GetPDefFromGlobalIndex(*it);
            CDefPointer<const CAppearanceModifierDef> modifier;
            if(definition->GetSubDef(&modifier)) {
                uncovered.erase(modifier->SuitPart);
                modifiers.insert(modifiers.end(),modifier);
            }
        }
    }
    for(std::set<EClothingSuitPart>::iterator it=uncovered.begin(),end=uncovered.end();it!=end;++it) {
        long index=0;
        switch(*it) {
        case 1:index=GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_HAT");break;
        case 2:index=GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_SHIRT");break;
        case 3:index=GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_GLOVES");break;
        case 4:index=GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_TROUSERS");break;
        case 5:index=GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_BOOTS");break;
        }
        CDefPointer<const CDefClassBase> definition=GetDefinitionManager().GetPDefFromGlobalIndex(index);
        CDefPointer<const CAppearanceModifierDef> modifier;
        if(definition->GetSubDef(&modifier)) modifiers.insert(modifiers.end(),modifier);
    }
    long count=items.size();
    for(long index=0;index<count;++index) {
        long item=items[index];
        CDefPointer<const CDefClassBase> definition=GetDefinitionManager().GetPDefFromGlobalIndex(item);
        CDefPointer<const CStockItemDef> stock;
        long price=0;
        if(definition->GetSubDef(&stock))price=stock->DefaultPrice;
        if(tracker) {
            const CThing* inside=tracker->GetPThingInside();
            if(IS_THING_ALIVE(inside)) {
                const CTCShop* shop;
                if(inside->PeekTC(TCI_SHOP,&shop) && shop->IsValidStockItem(item))price=shop->GetCurrentBuyPrice(item,1);
            }
        }
        values.total_price+=price;
        float itemFactor=1.0f,itemWeight=0.0f,curFactor=1.0f,curWeight=0.0f;
        float itemAttractiveness=0.0f,curAttractiveness=0.0f,itemScariness=0.0f,curScariness=0.0f,itemGoodstrength=0.0f,curGoodstrength=0.0f;
        CDefPointer<const CAppearanceModifierDef> modifier;
        if(definition->GetSubDef(&modifier)) {
            CDefPointer<const CArmourDef> armour=GetDefinitionManager().GetPDefFromGlobalIndex(modifier->ArmourDefIndex);
            std::map<EDamageAttribute,float>::const_iterator found=armour->DamageTypeResponse.find(DAMAGE_MELEE);
            if(found!=armour->DamageTypeResponse.end())itemFactor=found->second;
            itemWeight=modifier->OveralProtectionWeighting;
            itemAttractiveness=modifier->GetTypeBalancedAttractiveness(stats->AppearanceModifierScaling);
            itemScariness=modifier->GetTypeBalancedScariness(stats->AppearanceModifierScaling);
            itemGoodstrength=modifier->GetTypeBalancedGoodstrength(stats->AppearanceModifierScaling);
            if(appearance) {
                for(std::list<CDefPointer<const CAppearanceModifierDef> >::const_iterator it=modifiers.begin();it!=modifiers.end();++it) {
                    const CAppearanceModifierDef& current=**it;
                    if(current.SuitPart==modifier->SuitPart) {
                        curAttractiveness=current.GetTypeBalancedAttractiveness(stats->AppearanceModifierScaling);
                        curScariness=current.GetTypeBalancedScariness(stats->AppearanceModifierScaling);
                        curGoodstrength=current.GetTypeBalancedGoodstrength(stats->AppearanceModifierScaling);
                        CDefPointer<const CArmourDef> currentArmour=GetDefinitionManager().GetPDefFromGlobalIndex(current.ArmourDefIndex);
                        std::map<EDamageAttribute,float>::const_iterator currentFound=currentArmour->DamageTypeResponse.find(DAMAGE_MELEE);
                        if(currentFound!=currentArmour->DamageTypeResponse.end())curFactor=currentFound->second;
                        curWeight=(*it)->OveralProtectionWeighting;
                    }
                }
            }
        }
        values.total_item_armor_value_final+=(long)(itemWeight/itemFactor*500.0f);
        values.total_cur_armor_value_final+=(long)(curWeight/curFactor*500.0f);
        values.total_item_attractiveness_value_final+=(long)(itemAttractiveness*100.0f);
        values.total_cur_attractiveness_value_final+=(long)(curAttractiveness*100.0f);
        values.total_item_scariness_value_final+=(long)(itemScariness*100.0f);
        values.total_cur_scariness_value_final+=(long)(curScariness*100.0f);
        values.total_item_goodstrength_value_final+=(long)(itemGoodstrength*100.0f);
        values.total_cur_goodstrength_value_final+=(long)(curGoodstrength*100.0f);
    }
}