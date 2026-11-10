// CTCBerserk::CTCBerserk(CThing& thing) -- set up the berserk component: members cleared, the berserk def fetched, the timers and multipliers copied from its level-0 entries, the strength share, the crazy-move flourish and the ability level, then SetLevel
// Leaf: ??0CTCBerserk@@QAE@AAVCThing@@@Z
// Retail: Fable.exe 0x0078C210, 602 bytes (`ret 4` at 0x0078C467; next function 0x0078C470).
// Retail-asm: 0x0078C2B8..0x0078C2F0 `fadd [ebp-8] ; fistp dword ptr [ebp+8] ; fild [ebp+8] ; fsub [ebp-4] ; fstp [ebp-10h] ; mov eax,[ebp-0Ch] ; cmp eax,[ebp-10h] ; jne ; dec dword ptr [ebp+8]` -- the shared engine/GFFloatToLongCeil.h inline-asm ceiling expanded in place (the row carries no asm of its own).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBERSERK_HIT_EFFECT
#define FABLE_RETAIL_CTCBERSERK_CTOR
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_TOTALSTAT
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_FLOURISH
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CDEFINITIONMANAGER
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/EHeroAbility.h"
#include "engine/EHeroExperienceStatCategory.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CSpecialAbilitiesBerserkDef.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCCombat.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CTCBerserk.h"
#include "engine/GFFloatToLongCeil.h"

CTCBerserk::CTCBerserk(CThing& thing)
    : CTCBase(thing), ReleasedAnimationTimer(0.0f), AnimTimer(0), BerserkingLoopingSound(0), SpellLevel(0), JustReleased(true),
      AbilityInitialised(false), RadialBlurFadeKey(0), Paused(false), Pausable(true), IsInCutscene(false)
{
    long def_index;
    {
        CCharString def_name("SPECIAL_ABILITIES_BERSERK_DEF");
        def_index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(def_name, 1);
    }
    CGameDefinitionManager::Get().GetDef(def_index, &PDef);
    Timer = GFFloatToLongCeil(CMainGameComponent::GetConstantFPS() * PDef->BerserkDuration[0]);
    BerserkDamageToMe = PDef->BerserkDamageToMeMultiplier[0];
    BerserkDamageToEnemy = PDef->BerserkDamageToEnemyMultiplier[0];
    MaxIncreaseInStrength = PDef->MaxIncreaseInStrength[0];
    BerserkScale = PDef->BerserkScale[0];
    RumbleLevel = PDef->RumbleLevel[0];
    RumbleSmoothness = PDef->RumbleSmoothness[0];
    RumbleAttack = PDef->RumbleAttack[0];
    RumbleDecay = PDef->RumbleDecay[0];
    if (Timer <= PDef->FramesToFadeOut) Timer = PDef->FramesToFadeOut + 1;

    CTCHeroExperience* experience;
    if (GetParentThing().GetTC(TCI_HERO_EXPERIENCE, &experience)) {
        long max_level = experience->GetTotalStatMaxLevel(HERO_STAT_STRENGTH);
        StrengthPercentage = (float)experience->GetTotalStatLevel(HERO_STAT_STRENGTH) / max_level;
    }
    else
        StrengthPercentage = 0.0f;

    CTCCombat* combat;
    if (GetParentThing().GetTC(TCI_COMBAT, &combat)) {
        long flourish;
        {
            CCharString flourish_name("HERO_MELEE_COMBAT_ABILITY_BERSERK_CRAZY_MOVE");
            flourish = CMainGameComponent::Get()->PeekDefinitionManager().GetDefGlobalIndexFromName(flourish_name);
        }
        combat->AddMeleeFlourishAbility(flourish);
    }

    const CThing& parent = GetParentThing();
    const CTCInventoryAbilities* abilities;
    if (parent.PeekTC(TCI_HERO_ABILITIES, &abilities))
        SpellLevel = abilities->GetAbilityLevel(HERO_ABILITY_BERSERK);
    SetLevel();
}