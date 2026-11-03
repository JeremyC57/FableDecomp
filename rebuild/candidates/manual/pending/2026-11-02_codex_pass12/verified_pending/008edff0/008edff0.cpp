// CWillButtonHelperTimer::CWillButtonHelperTimer(EHeroAbility,CThing&) -- initialize ability charging levels and cumulative frame thresholds.
// Leaf: ??0CWillButtonHelperTimer@@QAE@W4EHeroAbility@@AAVCThing@@@Z
#define FABLE_RETAIL_CRESOURCE_PRIVATE_MEMBERS
#define FABLE_RETAIL_IVREFCOUNT_PRIVATE
#define FABLE_RETAIL_VENDOR_VECTOR_ORDINARY
#define FABLE_RETAIL_DEFPTR_ORDINARY_INLINE
#define FABLE_RETAIL_WILL_BUTTON_TIMER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEFINITION_MANAGER
#define FABLE_RETAIL_TYPED_GETTC
#define FABLE_RETAIL_VISIBLE_LOWERBOUND
#define FABLE_STLPORT_VISIBLE_PUSH_BACK
#define FABLE_STLPORT_VISIBLE_TRUE_OVERFLOW
#include "engine/CThing.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CWillButtonHelperTimer.h"
#include "engine/CGameDefinitionManager.h"
#include "boost/scoped_ptr.h"
#include "engine/CMainGameComponent.h"
#include "engine/CSpecialAbilitiesForcePushDef.h"
#include "engine/CSpecialAbilitiesBulletTimeDef.h"
#include "engine/CSpecialAbilitiesBerserkDef.h"
#include "engine/CSpecialAbilitiesEnflameDef.h"

#pragma auto_inline(off)
template<class T> bool CDefinitionManager::GetDef(long index,CDefPointer<const T>* out) const
{
    if(index<=0) return false;
    CDefPointer<const CDefClassBase> def=GetPDefFromGlobalIndex(index);
    if(!def.IsNull()) {
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#pragma auto_inline(on)

CWillButtonHelperTimer::CWillButtonHelperTimer(EHeroAbility ability,CThing& creature)
    : ButtonHeldDownTimer(0), WillAccumulatedLevel(0), PCreature(&creature),
      MyAbility(ability), CurrentState(BEING_HELD_DOWN), MaxNumberLevels(0),
      CurMaxLevels(0), WillIncrementedFlag(false), UpdateAccLevel(true),
      IsAICasting(false), IsAIHoldingDown(false), LevelToCast(0)
{
    const CTCInventoryAbilities* ptc_inventory_abilities=0;
    if(PCreature->PeekTC(&ptc_inventory_abilities)) {
        MaxNumberLevels=(float)ptc_inventory_abilities->GetAbilityMaximumLevel(MyAbility);
        CurMaxLevels=(float)ptc_inventory_abilities->GetAbilityLevel(MyAbility);
    } else {
        MaxNumberLevels=3;
        CurMaxLevels=3;
    }
    switch(ability) {
    case HERO_ABILITY_FORCE_PUSH:
        {
            long def_index=CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_FORCE_PUSH_DEF",1);
            CDefPointer<const CSpecialAbilitiesForcePushDef> def;
            CGameDefinitionManager::Get().GetDef(def_index,&def);
            long index=(long)MaxNumberLevels;
            float accum=0;
            for(long c1=0;c1<index;++c1) {
                accum+=def->LevelUpTimings[c1];
                WillPowerUpAccumulatedTimings.push_back(accum*CMainGameComponent::GetConstantFPS());
            }
        }
        break;
    case HERO_ABILITY_TIME_SPELL:
        {
            long def_index=CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_BULLET_TIME_DEF",1);
            CDefPointer<const CSpecialAbilitiesBulletTimeDef> def;
            CGameDefinitionManager::Get().GetDef(def_index,&def);
            long index=(long)MaxNumberLevels;
            float accum=0;
            for(long c1=0;c1<index;++c1) {
                accum+=def->LevelUpTimings[c1];
                WillPowerUpAccumulatedTimings.push_back(accum*CMainGameComponent::GetConstantFPS());
            }
        }
        break;
    case HERO_ABILITY_BERSERK:
        {
            long def_index=CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_BERSERK_DEF",1);
            CDefPointer<const CSpecialAbilitiesBerserkDef> def;
            CGameDefinitionManager::Get().GetDef(def_index,&def);
            long index=(long)MaxNumberLevels;
            float accum=0;
            for(long c1=0;c1<index;++c1) {
                accum+=def->LevelUpTimings[c1];
                WillPowerUpAccumulatedTimings.push_back(accum*CMainGameComponent::GetConstantFPS());
            }
        }
        break;
    case HERO_ABILITY_BATTLE_CHARGE:
    case HERO_ABILITY_ASSASSIN_RUSH:
        MaxNumberLevels=0;
        break;
    case HERO_ABILITY_ENFLAME_SPELL:
        {
            long def_index=CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_ENFLAME_DEF",1);
            CDefPointer<const CSpecialAbilitiesEnflameDef> def;
            CGameDefinitionManager::Get().GetDef(def_index,&def);
            long index=(long)MaxNumberLevels;
            float accum=0;
            for(long c1=0;c1<index;++c1) {
                accum+=def->LevelUpTimings[c1];
                WillPowerUpAccumulatedTimings.push_back(accum*CMainGameComponent::GetConstantFPS());
            }
        }
        break;
    default:
        long index=(long)MaxNumberLevels;
            for(long c1=0;c1<index;++c1)
            WillPowerUpAccumulatedTimings.push_back((float)((c1+1)*CMainGameComponent::GetConstantFPS()));
    }
}
