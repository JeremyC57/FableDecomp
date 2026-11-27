// Flags: (none)
// Leaf: ??0CTCPhysicalShieldSpell@@QAE@AAVCThing@@@Z
#define FABLE_RETAIL_SHIELD_CTOR_PASS29
#define FABLE_RETAIL_BRIAR_ROSE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR_ADOPT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_PHYSICAL_SHIELD_EFFECT_PASS26
#include "engine/CThingCreatureBase.h"
#include "engine/EHeroAbility.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CSpecialAbilitiesPhysicalShieldDef.h"
#include "engine/CTCPhysicalShieldSpell.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFDebugPDynamicCast.h"

CTCPhysicalShieldSpell::CTCPhysicalShieldSpell(CThing& thing)
    : CTCBase(thing), PDef((const CSpecialAbilitiesPhysicalShieldDef*)0), SpellLevel(0), ShieldFadeOutTimer(0), CurShieldFadeOutTimer(0), DurationTimer(-1),
      ShieldHealth(0.0f), CurWobbleTimer(0.0f), WobbleDelta(0.0f), ShieldEffect(0), SwirlyEffect(0), Pausable(true), Paused(false), CutSceneCancelled(false)
{
    long def_index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_PHYSICAL_SHIELD_DEF", 1);
    CGameDefinitionManager::Get().GetDef(def_index,&PDef);
    WobbleDelta = PDef->WobbleRate;
    CThingCreatureBase* pcreature = GFDebugPDynamicCast(&GetParentThing(),(CThingCreatureBase*)0);
    const CTCInventoryAbilities* ptc_inventory_abilities;
    if (pcreature->PeekTC(&ptc_inventory_abilities))
        SpellLevel = ptc_inventory_abilities->GetAbilityLevel(HERO_ABILITY_PHYSICAL_SHIELD_SPELL);
    switch (SpellLevel) {
    case 0:
        ShieldColor = CRGBColour((unsigned char)PDef->ColorStartLevel1[0], (unsigned char)PDef->ColorStartLevel1[1], (unsigned char)PDef->ColorStartLevel1[2], (unsigned char)PDef->ColorStartLevel1[3]);
        break;
    case 1:
        ShieldColor = CRGBColour((unsigned char)PDef->ColorStartLevel2[0], (unsigned char)PDef->ColorStartLevel2[1], (unsigned char)PDef->ColorStartLevel2[2], (unsigned char)PDef->ColorStartLevel2[3]);
        break;
    case 2:
        ShieldColor = CRGBColour((unsigned char)PDef->ColorStartLevel3[0], (unsigned char)PDef->ColorStartLevel3[1], (unsigned char)PDef->ColorStartLevel3[2], (unsigned char)PDef->ColorStartLevel3[3]);
        break;
    case 3:
        ShieldColor = CRGBColour((unsigned char)PDef->ColorStartLevel4[0], (unsigned char)PDef->ColorStartLevel4[1], (unsigned char)PDef->ColorStartLevel4[2], (unsigned char)PDef->ColorStartLevel4[3]);
        break;
    default:
        ShieldColor = CRGBColour((unsigned char)PDef->ColorStartLevel1[0], (unsigned char)PDef->ColorStartLevel1[1], (unsigned char)PDef->ColorStartLevel1[2], (unsigned char)PDef->ColorStartLevel1[3]);
        break;
    }
    LoopingSound = PlaySoundW(PDef->LoopingSound[SpellLevel],pcreature->GetType()==2);
    PlayEffect(0,"CAST","",pcreature,8);
    PlayShieldEffects(pcreature);
    pcreature->MakeNoise("SIMPLE_NOISE");
    ShieldFadeOutTimer = CurShieldFadeOutTimer = (long)(CMainGameComponent::GetConstantFPS()*PDef->FadeOutPhysicalShieldTimeSecs);
}