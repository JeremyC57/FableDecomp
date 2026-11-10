#include <map>
#include <set>
#include <list>
#include <vector>
#include <string>
#include <assert.h>
#include <stdio.h>
enum EClothingSuitPart {
    CLOTHING_SUIT_NULL=0,
    CLOTHING_SUIT_HEAD=1,
    CLOTHING_SUIT_BODY=2,
    CLOTHING_SUIT_HANDS=3,
    CLOTHING_SUIT_LEGS=4,
    CLOTHING_SUIT_FEET=5
};
enum EDamageAttribute {
    DAMAGE_NULL=-1,
    DAMAGE_MELEE=0,
    DAMAGE_MELEE_UNARMED=1,
    DAMAGE_LIGHTNING=2,
    DAMAGE_FIRE=3,
    DAMAGE_PROJECTILE=4,
    DAMAGE_EXPLOSION=5,
    DAMAGE_DRAIN=6,
    DAMAGE_DRAIN_HEAL=7,
    DAMAGE_GENERIC_WILL=8,
    DAMAGE_DIVINE_WRATH=9,
    DAMAGE_UNHOLY_POWER=10
};
enum EHeroAttachableAppearanceModifierType {
    APPEARANCE_HAIR=0,
    APPEARANCE_HORN=1,
    APPEARANCE_CLOTHING=2,
    NO_OF_APPEARANCE_MODIFIER_TYPES=3
};
enum ETCInterfaceType {TCI_SHOP=83,TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS=94,TCI_VOLUME_CONTAINMENT_TRACKER=117};
class CCharString {public:std::string Text;CCharString(const char*text):Text(text){}};
class CAppearanceModifierDef;class CInventoryItemDef;class CStockItemDef;class CHeroSuitDef;
template<class T> class CDefPointer;
class CDefClassBase {
public:
    mutable int Refs;
    const CAppearanceModifierDef* Appearance;const CInventoryItemDef* Inventory;const CStockItemDef* Stock;const CHeroSuitDef* Suit;
    CDefClassBase():Refs(1),Appearance(0),Inventory(0),Stock(0),Suit(0){}
    virtual ~CDefClassBase(){}
    template<class T> bool GetSubDef(CDefPointer<const T>* output)const;
};
template<class T> class CDefPointer {
    const CDefClassBase* Object;
public:
    CDefPointer():Object(0){}
    CDefPointer(T*value):Object(value){if(Object)++Object->Refs;}
    CDefPointer(const CDefPointer&other):Object(other.Object){if(Object)++Object->Refs;}
    template<class U>CDefPointer(const CDefPointer<U>&other):Object(other.Get()){if(Object)++Object->Refs;}
    ~CDefPointer(){if(Object){--Object->Refs;assert(Object->Refs>=1);}}
    void Reset(T*value){if(Object)--Object->Refs;Object=value;if(Object)++Object->Refs;}
    T* Get()const{return static_cast<T*>(Object);}
    T* operator->()const{return Get();}
    T& operator*()const{return *Get();}
};
class CAppearanceModifierScalingDef {};
class CHeroStatsDef {public:std::vector<CAppearanceModifierScalingDef> AppearanceModifierScaling;};
class CAppearanceModifierDef:public CDefClassBase {
public:
    EClothingSuitPart SuitPart;long ArmourDefIndex;float OveralProtectionWeighting,Attractiveness,Scariness,Goodstrength;
    float GetTypeBalancedAttractiveness(const std::vector<CAppearanceModifierScalingDef>&)const{return Attractiveness;}
    float GetTypeBalancedScariness(const std::vector<CAppearanceModifierScalingDef>&)const{return Scariness;}
    float GetTypeBalancedGoodstrength(const std::vector<CAppearanceModifierScalingDef>&)const{return Goodstrength;}
};
class CArmourDef:public CDefClassBase {public:std::map<EDamageAttribute,float> DamageTypeResponse;};
class CInventoryItemDef:public CDefClassBase {public:long ItemToSelectUponRemoval;};
class CStockItemDef:public CDefClassBase {public:long DefaultPrice;};
class CHeroSuitDef:public CDefClassBase {public:std::map<EClothingSuitPart,long> SuitParts;};
class CThingBaseDef:public CDefClassBase {};
template<class T>struct SubDef;
template<>struct SubDef<CAppearanceModifierDef>{static const CAppearanceModifierDef* Get(const CDefClassBase&d){return d.Appearance;}};
template<>struct SubDef<CInventoryItemDef>{static const CInventoryItemDef* Get(const CDefClassBase&d){return d.Inventory;}};
template<>struct SubDef<CStockItemDef>{static const CStockItemDef* Get(const CDefClassBase&d){return d.Stock;}};
template<>struct SubDef<CHeroSuitDef>{static const CHeroSuitDef* Get(const CDefClassBase&d){return d.Suit;}};
template<class T>bool CDefClassBase::GetSubDef(CDefPointer<const T>*output)const{const T*value=SubDef<T>::Get(*this);if(!value)return false;output->Reset(value);return true;}
static CHeroStatsDef stats;
class CGameDefinitionManager {
public:
    const CHeroStatsDef* PHeroStatsDef;
    std::map<long,const CDefClassBase*> Definitions;std::map<std::string,long> Names;
    CGameDefinitionManager():PHeroStatsDef(&stats){}
    long GetDefGlobalIndexFromName(const CCharString&name)const{std::map<std::string,long>::const_iterator it=Names.find(name.Text);assert(it!=Names.end());return it->second;}
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long index)const{std::map<long,const CDefClassBase*>::const_iterator it=Definitions.find(index);assert(it!=Definitions.end());return CDefPointer<const CDefClassBase>(it->second);}
    bool GetDef(long index,CDefPointer<const CThingBaseDef>*output)const{output->Reset(static_cast<const CThingBaseDef*>(Definitions.find(index)->second));return true;}
};
static CGameDefinitionManager manager;
class CTCHeroAttachableAppearanceModifiers {
public:std::list<long> Worn;
    void GetAppearanceModifierList(std::list<long>&out,EHeroAttachableAppearanceModifierType type)const{assert((int)type==2);out=Worn;}
};
class CThing;
class CTCVolumeContainmentTracker {public:CThing* Inside;CThing*GetPThingInside()const{return Inside;}};
class CTCShop {
public:
    std::map<long,long> Prices;
    bool IsValidStockItem(long item)const{return Prices.find(item)!=Prices.end();}
    long GetCurrentBuyPrice(long item,long count)const{assert(count==1);return Prices.find(item)->second;}
};
class CThing {
public:
    bool Alive;void* Components[118];
    CThing():Alive(true){for(int i=0;i<118;++i)Components[i]=0;}
    template<class T>bool PeekTC(ETCInterfaceType id,T**out)const{if(!Components[(int)id])return false;*out=(T*)Components[(int)id];return true;}
};
template<class T>bool IS_THING_ALIVE(const T&thing){return thing&&thing->Alive;}
class CTCInventoryClothing {
public:
    struct ArgPasser {bool is_suit;long num_suit_parts,max_suit_parts,total_item_armor_value_final,total_cur_armor_value_final;float total_item_attractiveness_value_final,total_cur_attractiveness_value_final,total_item_scariness_value_final,total_cur_scariness_value_final,total_item_goodstrength_value_final,total_cur_goodstrength_value_final;long total_price;};
    CThing* ParentThing;std::map<long,long> Owned;
    const CGameDefinitionManager&GetDefinitionManager()const{return manager;}
    long GetNumberOfItemsOfTypeInInventory(long item)const{std::map<long,long>::const_iterator it=Owned.find(item);return it==Owned.end()?0:it->second;}
    void CalculateArmorValues(ArgPasser&,long)const;
};
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

static void CheckRefs(const CDefClassBase&item){assert(item.Refs==1);}
int main(){
    CArmourDef armour,heavyArmour;armour.DamageTypeResponse[DAMAGE_MELEE]=1.0f;heavyArmour.DamageTypeResponse[DAMAGE_MELEE]=2.0f;
    manager.Definitions[200]=&armour;manager.Definitions[201]=&heavyArmour;
    CThingBaseDef defaults[5];CAppearanceModifierDef defaultMods[5];
    const char*names[]={"OBJECT_HERO_NO_HAT","OBJECT_HERO_NO_SHIRT","OBJECT_HERO_NO_GLOVES","OBJECT_HERO_NO_TROUSERS","OBJECT_HERO_NO_BOOTS"};
    for(int i=0;i<5;++i){manager.Names[names[i]]=i+1;manager.Definitions[i+1]=&defaults[i];defaults[i].Appearance=&defaultMods[i];defaultMods[i].SuitPart=(EClothingSuitPart)(i+1);defaultMods[i].ArmourDefIndex=200;defaultMods[i].OveralProtectionWeighting=0.125f;defaultMods[i].Attractiveness=defaultMods[i].Scariness=defaultMods[i].Goodstrength=0.0f;}
    CThingBaseDef item,worn,missing,suitBase,bareBase;
    CAppearanceModifierDef itemMod,wornMod;CInventoryItemDef fallback;CStockItemDef stock;CHeroSuitDef suit,bareSuit;
    manager.Definitions[10]=&item;manager.Definitions[11]=&worn;manager.Definitions[12]=&missing;manager.Definitions[100]=&suitBase;manager.Definitions[101]=&bareBase;
    manager.Names["HERO_SUIT_NAKED"]=101;
    item.Appearance=&itemMod;item.Stock=&stock;stock.DefaultPrice=42;
    itemMod.SuitPart=CLOTHING_SUIT_HEAD;itemMod.ArmourDefIndex=200;itemMod.OveralProtectionWeighting=0.5f;itemMod.Attractiveness=0.125f;itemMod.Scariness=0.25f;itemMod.Goodstrength=-0.125f;
    worn.Appearance=&wornMod;wornMod.SuitPart=CLOTHING_SUIT_HEAD;wornMod.ArmourDefIndex=201;wornMod.OveralProtectionWeighting=1.0f;wornMod.Attractiveness=0.5f;wornMod.Scariness=-0.25f;wornMod.Goodstrength=0.125f;
    missing.Inventory=&fallback;fallback.ItemToSelectUponRemoval=2;
    suitBase.Suit=&suit;suit.SuitParts[CLOTHING_SUIT_HEAD]=10;suit.SuitParts[CLOTHING_SUIT_BODY]=12;suit.SuitParts[CLOTHING_SUIT_HANDS]=3;
    bareBase.Suit=&bareSuit;bareSuit.SuitParts[CLOTHING_SUIT_HEAD]=1;bareSuit.SuitParts[CLOTHING_SUIT_BODY]=2;
    CThing hero,shopThing;CTCInventoryClothing clothing;clothing.ParentThing=&hero;CTCHeroAttachableAppearanceModifiers appearance;appearance.Worn.push_back(11);
    CTCVolumeContainmentTracker tracker;tracker.Inside=&shopThing;CTCShop shop;shop.Prices[10]=99;shopThing.Components[TCI_SHOP]=&shop;
    CTCInventoryClothing::ArgPasser v;
    clothing.CalculateArmorValues(v,10);
    assert(!v.is_suit&&v.num_suit_parts==0&&v.max_suit_parts==0&&v.total_price==42&&v.total_item_armor_value_final==250&&v.total_cur_armor_value_final==0);
    assert(v.total_item_attractiveness_value_final==12.0f&&v.total_item_scariness_value_final==25.0f&&v.total_item_goodstrength_value_final==-12.0f&&v.total_cur_attractiveness_value_final==0.0f);
    hero.Components[TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS]=&appearance;clothing.CalculateArmorValues(v,10);
    assert(v.total_cur_armor_value_final==250&&v.total_cur_attractiveness_value_final==50.0f&&v.total_cur_scariness_value_final==-25.0f&&v.total_cur_goodstrength_value_final==12.0f);
    hero.Components[TCI_VOLUME_CONTAINMENT_TRACKER]=&tracker;clothing.CalculateArmorValues(v,10);assert(v.total_price==99);
    shopThing.Alive=false;clothing.CalculateArmorValues(v,10);assert(v.total_price==42);shopThing.Alive=true;
    tracker.Inside=0;clothing.CalculateArmorValues(v,10);assert(v.total_price==42);tracker.Inside=&shopThing;
    clothing.Owned[10]=1;clothing.Owned[3]=1;clothing.CalculateArmorValues(v,100);
    assert(v.is_suit&&v.num_suit_parts==1&&v.max_suit_parts==2&&v.total_item_armor_value_final==498&&v.total_cur_armor_value_final==498&&v.total_price==99);
    fallback.ItemToSelectUponRemoval=0;clothing.CalculateArmorValues(v,100);assert(v.total_item_armor_value_final==436);fallback.ItemToSelectUponRemoval=2;
    clothing.Owned[1]=1;clothing.Owned[2]=1;clothing.CalculateArmorValues(v,101);assert(v.is_suit&&v.num_suit_parts==2&&v.max_suit_parts==2&&v.total_item_armor_value_final==310);
    item.Stock=0;hero.Components[TCI_VOLUME_CONTAINMENT_TRACKER]=0;clothing.CalculateArmorValues(v,10);assert(v.total_price==0);
    for(int i=0;i<5;++i){CheckRefs(defaults[i]);CheckRefs(defaultMods[i]);}
    CheckRefs(item);CheckRefs(worn);CheckRefs(missing);CheckRefs(suitBase);CheckRefs(bareBase);CheckRefs(itemMod);CheckRefs(wornMod);CheckRefs(fallback);CheckRefs(stock);CheckRefs(suit);CheckRefs(bareSuit);CheckRefs(armour);CheckRefs(heavyArmour);
    puts("005b4eba_TEST PASS");return 0;
}