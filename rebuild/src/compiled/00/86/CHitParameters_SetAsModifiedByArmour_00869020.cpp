// CHitParameters::SetAsModifiedByArmour(const CThing& thing, CDefPointer<const CArmourDef>& armour) -- collect the thing's armour defs (or the given armour at weight 1 when it has none), raise the recoil / negate / knock-down flags the armour asks for (an AI creature immune to attack negates the hit), then run every damage attribute through the armour weights (a deflectable projectile whose armour threshold beats the shot build-up time negates the hit), buff it by the hit's augmentations, scale it by the toughness damage reduction factor and, for physical things, clamp the total to the thing's health; no damage left and no guaranteed hit negates the hit
// Leaf: ?SetAsModifiedByArmour@CHitParameters@@QAEXABVCThing@@AAV?$CDefPointer@$$CBVCArmourDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00869020, 955 bytes, thiscall `ret 8`.
// NF67 port (round 7): onto the shared headers -- engine/CHitParameters.h (DamageMap std::map<EDamageAttribute,float> +0x30,
//   Augmentations std::vector<EObjectAugmentationType> +0x3C, PWeapon CIntelligentPointer<CThing> +0x58), engine/CArmourDef.h
//   (AllHitsKnockdown / AllHitsNegated / AllHitsCauseRecoil +0x60..+0x62), engine/CDefPointer.h, engine/CCompareDefPointer.h, the
//   STLport multimap<CDefPointer<const CArmourDef>, float, CCompareDefPointer<const CArmourDef, float> > of stlport/fable_map.h
//   (the tree's _M_insert 0x00869CA0 and _M_erase 0x00869BB0 out of line), engine/CThing.h (CClass::GetType +0x10, GetTypeFlags +0x6C),
//   engine/CThingAICreature.h (IsImmuneToAttack, +0x1C8 bit 6), engine/CThingPhysical.h (GetHealth +0xB4) and
//   engine/NThingShot_CTCShotDefinitionBase.h (IsAbleToBeDeflected 0x007BED60 through PeekTC, TCI_SHOT_DEFINITION 119).
//   `GetTypeFlags() & 2` is the thing-type bit retail tests (`test byte ptr [esi+6Ch],2`) before reading CThingPhysical::Health.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_ISIMMUNETOATTACK
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP
#define FABLE_RETAIL_CHITPARAMETERS_AUGMENTATIONS
#define FABLE_RETAIL_CHITPARAMETERS_SETASMODIFIEDBYARMOUR
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ISABLETOBEDEFLECTED
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_EQUAL_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_STLP_TREE_MINSERT_NOINLINE
#define FABLE_RETAIL_STLP_SET_EMPTY
#define FABLE_RETAIL_STLP_MULTIMAP_EMPTY
#define FABLE_RETAIL_STLP_MULTIMAP_PASS22_ATMOS_ITERATION
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_CARMOURDEF_AUGMENTATIONRESPONSE
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_LIST
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_map.h"
#include "fable_compiler.h"
#include "engine/CDefPointer.h"
#include "engine/CArmourDef.h"
#include "engine/CCompareDefPointer.h"
#include "engine/EDamageAttribute.h"
#include "engine/EObjectAugmentationType.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPhysical.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"
#include "engine/ThingAlive.h"
#include "engine/CHitParameters.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingObjectDef.h"
#include "engine/CAppearanceModifierDef.h"
#include "engine/CTCHeroAttachableAppearanceModifiers.h"

// The armour helpers are this TU's anonymous-namespace functions (Ego_r.pdb ?...@?A0x23f4dc30@@YI...; retail 0x008686E0..0x00868E50, all
// called only from here). Two bodies are visible, as they are in the retail TU, and kept out of line as retail calls them: the
// texts of the landed rows 0x00868E50 (GetArmourWeightsFromThing, with its GetDef / GetSubDef<CAppearanceModifierDef> helpers)
// and 0x00868A80 (GetArmourResponseAugmentationDamageMultiplier). Seeing them, VC7.1 knows the weight total and the augmentation
// buff are written only through their references and CHitParameters is not touched: the map header and `it->second = 0` stores
// sink below the argument pushes (0x00869065 / 0x0086922A) and Augmentations' end stays in edi across the buff loop (0x00869256).
template<class T> bool CDefinitionManager::GetDef(long index,CDefPointer<const T>* out) const
{
    if(index>0) {
        CDefPointer<const CDefClassBase> def=GetPDefFromGlobalIndex(index);
        if(!def.IsNull()) {
            out->SetDynamicCast(def);
            return true;
        }
    }
    return false;
}
template<> bool CDefClassBase::GetSubDef<CAppearanceModifierDef>(CDefPointer<const CAppearanceModifierDef>* out) const
{
    const CSubDefInfo* info;
    {
        CCharString name("CAppearanceModifierDef");
        info=GetPSubDefInfo(name);
    }
    if(info) {
        CDefPointer<const CDefClassBase> def=PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
namespace {
__declspec(noinline) void FABLE_FASTCALL GetArmourWeightsFromThing(const CThing& thing,
    std::multimap<CDefPointer<const CArmourDef>,float,CCompareDefPointer<const CArmourDef,float> >& armour_weights,
    float& total_armour_weight)                                                                               // 0x00868E50
{
    const CGameDefinitionManager& def_man=thing.PeekDefinitionManager();
    const CTCHeroAttachableAppearanceModifiers* ptc_armour_clothes;
    if(thing.PeekTC(TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS,&ptc_armour_clothes)) {
        std::list<long> clothes_list;
        ptc_armour_clothes->GetAppearanceModifierList(clothes_list,APPEARANCE_CLOTHING);
        for(std::list<long>::iterator it=clothes_list.begin();it!=clothes_list.end();++it) {
            CDefPointer<const CThingObjectDef> pdef;
            long clothing_item_def_index=*it;
            def_man.GetDef(clothing_item_def_index,&pdef);
            CDefPointer<const CAppearanceModifierDef> pclothing_def;
            pdef->GetSubDef(&pclothing_def);
            if(pclothing_def->ArmourDefIndex>=0) {
                CDefPointer<const CArmourDef> parmour_def;
                def_man.GetDef(pclothing_def->ArmourDefIndex,&parmour_def);
                armour_weights.insert(std::make_pair(parmour_def,pclothing_def->OveralProtectionWeighting));
                total_armour_weight+=pclothing_def->OveralProtectionWeighting;
            }
        }
    }
}
float FABLE_FASTCALL GetToughnessDamageReductionFactorForThing(const CThing& thing);                        // 0x00868B40
float FABLE_FASTCALL GetArmourThresholdValueFromArmourWeights(EDamageAttribute attribute,
    const std::multimap<CDefPointer<const CArmourDef>,float,CCompareDefPointer<const CArmourDef,float> >& armour_weights,
    float total_armour_weight);                                                                               // 0x008686E0
bool FABLE_FASTCALL GetArmourResponseDamageAttributeDamageMultiplier(EDamageAttribute attribute,
    const std::multimap<CDefPointer<const CArmourDef>,float,CCompareDefPointer<const CArmourDef,float> >& armour_weights,
    float total_armour_weight, float& multiplier);                                                            // 0x008687A0
__declspec(noinline) bool FABLE_FASTCALL GetArmourResponseAugmentationDamageMultiplier(EObjectAugmentationType augmentation,
    const std::multimap<CDefPointer<const CArmourDef>,float,CCompareDefPointer<const CArmourDef,float> >& armour_weights,
    float total_armour_weight, float& multiplier)                                                             // 0x00868A80
{
    float result = 0.0f;
    for (std::multimap<CDefPointer<const CArmourDef>,float,CCompareDefPointer<const CArmourDef,float> >::const_iterator it = armour_weights.begin();
         it != armour_weights.end(); ++it)
    {
        float scale = it->second / total_armour_weight * multiplier;
        float response = 1.0f;
        std::map<EObjectAugmentationType,float>::const_iterator found = it->first->AugmentationResponse.find(augmentation);
        if (found != it->first->AugmentationResponse.end())
            response = found->second;
        result += response * scale;
    }
    multiplier = result;
    return true;
}
}


void CHitParameters::SetAsModifiedByArmour(const CThing& thing, CDefPointer<const CArmourDef>& armour)
{
    std::multimap<CDefPointer<const CArmourDef>,float,CCompareDefPointer<const CArmourDef,float> > armour_weights;
    float total_armour_weight = 0.0f;
    GetArmourWeightsFromThing(thing, armour_weights, total_armour_weight);
    if (armour_weights.empty()) {
        armour_weights.insert(std::make_pair(armour, 1.0f));
        total_armour_weight = 1.0f;
    }
    if (armour->AllHitsCauseRecoil)
        CauseRecoil = true;
    if (armour->AllHitsNegated)
        HitNegated = true;
    if (armour->AllHitsKnockdown)
        KnockDown = true;
    if (thing.GetType() == 1 && static_cast<const CThingAICreature&>(thing).IsImmuneToAttack())
        HitNegated = true;

    float damage_reduction = GetToughnessDamageReductionFactorForThing(thing);
    long num_augmentations = Augmentations.size();
    bool any_damage = false;
    float total_damage = 0.0f;
    for (std::map<EDamageAttribute,float>::iterator it = DamageMap.begin(); it != DamageMap.end(); ++it) {
        float damage = it->second;
        EDamageAttribute attribute = it->first;
        if (DamageMap.find(DAMAGE_PROJECTILE) != DamageMap.end() && IS_THING_ALIVE(PWeapon.Get())) {
            const NThingShot::CTCShotDefinitionBase* shot;
            if (PWeapon->PeekTC(&shot) && shot->IsAbleToBeDeflected()) {
                if (GetArmourThresholdValueFromArmourWeights(attribute, armour_weights, total_armour_weight) > ShotBuildupTime)
                    HitNegated = true;
            }
        }
        it->second = 0.0f;
        if (GetArmourResponseDamageAttributeDamageMultiplier(attribute, armour_weights, total_armour_weight, damage)) {
            float value = damage;
            if (num_augmentations > 0) {
                float current = damage;
                for (std::vector<EObjectAugmentationType>::iterator aug = Augmentations.begin(); aug != Augmentations.end(); ++aug) {
                    float buff = current;
                    GetArmourResponseAugmentationDamageMultiplier(*aug, armour_weights, total_armour_weight, buff);
                    current = buff;
                }
                value = current;
            }
            if (value > 0.0001f)
                any_damage = true;
            it->second = value * damage_reduction;
            total_damage += it->second;
        }
    }
    if ((thing.GetTypeFlags() & 2) && total_damage > 0.0001f) {
        float scale = static_cast<const CThingPhysical&>(thing).GetHealth() / total_damage;
        if (scale < 1.0f) {
            for (std::map<EDamageAttribute,float>::iterator it = DamageMap.begin(); it != DamageMap.end(); ++it) {
                float& entry = it->second;
                entry = entry * scale;
            }
        }
    }
    if (!any_damage && !GuaranteeHit)
        HitNegated = true;
}
