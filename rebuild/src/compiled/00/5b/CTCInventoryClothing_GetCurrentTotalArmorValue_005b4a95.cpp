// CTCInventoryClothing::GetCurrentTotalArmorValue() -- sum the armour of every worn clothing modifier plus the "no clothing" defaults of the uncovered suit parts
// Leaf: ?GetCurrentTotalArmorValue@CTCInventoryClothing@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B4A95, frame-pointer size TU (optimize("y", off) + ("s", on)), thiscall `ret`.
// Name: Ego_r.pdb ?GetCurrentTotalArmorValue@CTCInventoryClothing@@QBEJXZ (public const; header opt-in FABLE_RETAIL_CTCINVENTORYCLOTHING_TOTALARMOUR).
// Name fix (agentNF 2026-11-10, Codex fixes [5]-[10]): moved off the local CSuitPartSet / CDamageFactorMap / CLongList / CThingDefPointer
//   models onto the shared headers of the landed neighbour 0x005B4EBA CTCInventoryClothing::CalculateArmorValues (same TU). The
//   STLport instantiations now bind their real spellings: set<EClothingSuitPart>() 0x005BB3B3, set::insert 0x005BAF94,
//   _Rb_tree<EClothingSuitPart>::erase(key) 0x005BBB34 and ~_Rb_tree 0x005BB04F, map<EDamageAttribute,float> _M_find<EDamageAttribute>
//   0x005BA9BE, list<long> _List_base ctor/dtor 0x0042AB58 / 0x0042AB73, _Rb_global<bool>::_M_increment 0x004292C0,
//   GetAppearanceModifierList(list<long>&, EHeroAttachableAppearanceModifierType) const 0x00706800, GetSubDef<CAppearanceModifierDef>
//   0x0057FF92 (same-TU body, as 0x005B4EBA).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_TOTALARMOUR
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
template<> bool CDefClassBase::GetSubDef<CAppearanceModifierDef>(CDefPointer<const CAppearanceModifierDef>* output)const
{
    const CSubDefInfo* info=GetPSubDefInfo("CAppearanceModifierDef");
    if(!info)return false;
    CDefPointer<const CAppearanceModifierDef> definition=PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
    *output=definition;
    return true;
}
long CTCInventoryClothing::GetCurrentTotalArmorValue() const
{
    std::set<EClothingSuitPart> uncovered;
    uncovered.insert(CLOTHING_SUIT_HEAD);
    uncovered.insert(CLOTHING_SUIT_BODY);
    uncovered.insert(CLOTHING_SUIT_HANDS);
    uncovered.insert(CLOTHING_SUIT_LEGS);
    uncovered.insert(CLOTHING_SUIT_FEET);
    long total = 0;
    const CTCHeroAttachableAppearanceModifiers* appearance;
    if (ParentThing->PeekTC(TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS, &appearance)) {
        std::list<long> worn;
        appearance->GetAppearanceModifierList(worn, APPEARANCE_CLOTHING);
        for (std::list<long>::const_iterator it = worn.begin(), end = worn.end(); it != end; ++it) {
            CDefPointer<const CDefClassBase> definition = GetDefinitionManager().GetPDefFromGlobalIndex(*it);
            CDefPointer<const CAppearanceModifierDef> modifier;
            if (definition->GetSubDef(&modifier)) {
                uncovered.erase(modifier->SuitPart);
                float factor = 1.0f;
                CDefPointer<const CArmourDef> armour = GetDefinitionManager().GetPDefFromGlobalIndex(modifier->ArmourDefIndex);
                std::map<EDamageAttribute, float>::const_iterator found = armour->DamageTypeResponse.find(DAMAGE_MELEE);
                if (found != armour->DamageTypeResponse.end())
                    factor = found->second;
                total += (long)(modifier->OveralProtectionWeighting / factor * 500.0f);
            }
        }
    }
    for (std::set<EClothingSuitPart>::iterator it = uncovered.begin(), end = uncovered.end(); it != end; ++it) {
        long index = 0;
        switch (*it) {
        case 1: index = GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_HAT"); break;
        case 2: index = GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_SHIRT"); break;
        case 3: index = GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_GLOVES"); break;
        case 4: index = GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_TROUSERS"); break;
        case 5: index = GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_BOOTS"); break;
        }
        CDefPointer<const CDefClassBase> definition = GetDefinitionManager().GetPDefFromGlobalIndex(index);
        CDefPointer<const CAppearanceModifierDef> modifier;
        if (definition->GetSubDef(&modifier)) {
            float factor = 1.0f;
            CDefPointer<const CArmourDef> armour = GetDefinitionManager().GetPDefFromGlobalIndex(modifier->ArmourDefIndex);
            std::map<EDamageAttribute, float>::const_iterator found = armour->DamageTypeResponse.find(DAMAGE_MELEE);
            if (found != armour->DamageTypeResponse.end())
                factor = found->second;
            total += (long)(modifier->OveralProtectionWeighting / factor * 500.0f);
        }
    }
    return total;
}
