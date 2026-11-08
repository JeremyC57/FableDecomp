// CTCInventoryStats::ConstructExperienceList(NUISystem::CComponent* parent) -- builds the console (pad) experience page of the
// hero stats screen: the "UI_STATS_EXP_LIST" component, a back action on its dummy menu entry that switches the top-level menu
// screen to state 1, the ring picture, the two level sliders of the experience screen, one tick slider per trainable stat
// (strength: physique / health / toughness in red, skill: accuracy / speed / guile in yellow, will: magic power in blue) and one
// row per hero ability, laid out from y = 270 in three groups 15 apart.
// Leaf: ?ConstructExperienceList@CTCInventoryStats@@UAEPAVCComponent@NUISystem@@PAV23@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D6748, 2361 bytes, `ret 4`. Plain frame-pointer TU: optimize("y",off) + ("s",on)
//   (packed odd address, `push imm ; pop reg`, `leave`), as the landed CTCInventoryStats::AddHeroAbility 0x005D64DD /
//   CreateExperienceSliders 0x005CDD86 of the same object.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?ConstructExperienceList@CTCInventoryStats@@UAEPAVCComponent@NUISystem@@PAV23@@Z
//   (virtual, engine/CTCInventoryStats.h); the Ego_r donor order CreateExperienceSliders / AddHeroAbility / ConstructExperienceList /
//   ConstructExperienceListPC matches retail 0x005CDD86 / 0x005D64DD / 0x005D6748 / 0x005D7081; the strings UI_STATS_EXP_*.
// Callees: NUISystem::CManager::GetInstance 0x0041E5F2, std::bitset<274>::test 0x00410DE0 + CVectorMap::LowerBound 0x0040F020
//   (the inline CThing::GetTC on the parent thing, TCI_HERO_EXPERIENCE 0x68 and TCI_INVENTORY_EXPERIENCE 0x16), CCharString
//   0x0099EBF0 / ~ 0x0099EAE0, CManager::CreateComponent 0x0041DB1D, operator new 0x00BFEA1A, CAction::CAction 0x0042BE50,
//   CCountedPointer<CAction>(T*) 0x0042AA29 / ~ 0x00429160, list<CActionParam*>::push_back 0x0042B646,
//   CCountedPointer<CComponent>(T*) 0x00429C15 / Reset 0x00429C6F / ~ 0x004291DE, CTCInventoryStats::CreateExperienceSliders
//   0x005CDD86 (result vector dropped: inline ~vector, free 0x00BFEA14), CTCHeroExperience::GetTrainableStatLevel 0x006D6520 /
//   GetTrainableStatMaxLevel 0x006D6770 / GetTrainableStatPossibleLevelUpAmount 0x006D6FA0, CTCInventoryExperience::
//   GetExperienceStatName 0x0056F26F (hidden CWideString result), CManager::CreateTickSlider 0x00427216, ~CWideString 0x0099B510,
//   CTCInventoryStats::AddHeroAbility 0x005D64DD (18 times). Virtuals: CComponent FindChild 3, AddChild 59, AddActionOnBack 68.
// Float pool: 0.7f 0x01255318 (loaded once and stored twice for the yellow sliders), 270.0f 0x0125531C, 20.0f 0x0122F160,
//   15.0f 0x0124E640; 0.7f is also written as the immediate 0x3F333333 where a single store needs it.
// Layout: CTCBase::ParentThing +0x04; CThing UsingComponents +0x20, Components +0x44; CAction ActionType +0, UIActionParams +4;
//   CActionParamState type 2 at +4, state at +8; CComponent's CActionParam base at +0x18 (the implicit pointer conversion is
//   null-guarded: `neg ; sbb ; and`).
// Shape: the TC lookups are the inline GetTC(id, ptc) (the key temporaries are materialised in registers, `push 68h ; pop ebx`);
//   the state-change action's parameters are pushed straight onto UIActionParams; the seven stat sliders are written out one by
//   one (retail's source is unrolled: the y offsets 0 / 30 / 60 / 110 / 140 / 170 / 220 and the colours differ per group) into
//   one CCountedPointer local filled by Reset and added with the virtual AddChild (level / maxLevel / possibleLevels declared once
//   in the tick-slider block; the ability rows in a nested block of their own, so the row position shares the level slot); the level-up fraction is one float local the
//   possible-level-up getter fills by reference (it reuses the dead `parent` slot) and the whole levels are added into it
//   (`fild ; fadd [fraction] ; fstp [fraction]`) before the slider call.
// Names: EHeroTrainableStatType's enumerators are in neither PDB name list (the type is an MS forward enum in both); the seven
//   below are placeholders (naming queue) spelled after the NGlobalConsole::ConsoleSetHero<Group><Stat>Level symbols
//   (StrengthPhysique / StrengthHealth / StrengthToughness / SkillAccuracy / SkillSpeed / SkillStealth / WillMagicPower) in the
//   order the stats screen shows them; the values are retail's immediates.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYSTATS
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_STATEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CACTIONPARAMSTATE
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCInventoryExperience.h"
#include "engine/C2DCoordI.h"
#include "engine/CRGBFloatColour.h"
#include "engine/EHeroAbility.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamState.h"
#include "engine/CCountedPointer.h"

using NUISystem::CComponent;
using NUISystem::CAction;
using NUISystem::CActionParamState;

enum EHeroTrainableStatType {                                     // enumerator spellings unproven (see above)
    HERO_TRAINABLE_STAT_STRENGTH_PHYSIQUE = 0,
    HERO_TRAINABLE_STAT_STRENGTH_HEALTH = 1,
    HERO_TRAINABLE_STAT_STRENGTH_TOUGHNESS = 2,
    HERO_TRAINABLE_STAT_SKILL_ACCURACY = 3,
    HERO_TRAINABLE_STAT_SKILL_SPEED = 4,
    HERO_TRAINABLE_STAT_SKILL_STEALTH = 5,
    HERO_TRAINABLE_STAT_WILL_MAGIC_POWER = 9
};

CComponent* CTCInventoryStats::ConstructExperienceList(CComponent* parent)
{
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CTCHeroExperience* heroExperience;
    ParentThing->GetTC(TCI_HERO_EXPERIENCE, &heroExperience);
    CTCInventoryExperience* inventoryExperience;
    ParentThing->GetTC(TCI_INVENTORY_EXPERIENCE, &inventoryExperience);

    CComponent* list = manager->CreateComponent("UI_STATS_EXP_LIST", false);
    list->FindChild("UI_STATS_EXP_VIEWPORT");

    // backing out of the dummy entry puts the top-level menu screen back into state 1
    CCountedPointer<CAction> backAction(new CAction);
    backAction->ActionType = NUISystem::UI_ACTION_TYPE_CHANGESTATE;
    backAction->UIActionParams.push_back(parent->FindChild("UI_TOP_LEVEL_MENU_SCREEN"));
    backAction->UIActionParams.push_back(new CActionParamState(1));
    CComponent* dummyEntry = list->FindChild("UI_STATS_EXP_DUMMY_MENU_ENTRY");
    if (dummyEntry) {
        dummyEntry->AddActionOnBack(backAction);
        CCountedPointer<CComponent> ringPic(NUISystem::CManager::GetInstance()->CreateComponent("UI_RING_PIC_EXP_STATS", false));
        dummyEntry->AddChild(ringPic);
    }

    CComponent* screen = list->FindChild("UI_STATS_EXP_SCREEN");
    if (screen) {
        CComponent* levelSliderContainer = screen->FindChild("UI_STATS_EXP_LEVEL_SLIDER_CONTAINER");
        if (levelSliderContainer)
            CreateExperienceSliders(levelSliderContainer, 2, true);
        CComponent* tickSliderContainer = screen->FindChild("UI_STATS_EXP_TICK_SLIDER_CONTAINER");
        if (tickSliderContainer) {
            float fraction = 0.0f;
            CCountedPointer<CComponent> slider;
            long level;
            long maxLevel;
            long possibleLevels;

            // strength stats, red
            level = heroExperience->GetTrainableStatLevel(HERO_TRAINABLE_STAT_STRENGTH_PHYSIQUE);
            maxLevel = heroExperience->GetTrainableStatMaxLevel(HERO_TRAINABLE_STAT_STRENGTH_PHYSIQUE);
            possibleLevels = heroExperience->GetTrainableStatPossibleLevelUpAmount(HERO_TRAINABLE_STAT_STRENGTH_PHYSIQUE, fraction);
            fraction += possibleLevels + level;
            slider.Reset(manager->CreateTickSlider(inventoryExperience->GetExperienceStatName(HERO_TRAINABLE_STAT_STRENGTH_PHYSIQUE), C2DCoordI(0, 0), level, maxLevel,
                                                   CRGBFloatColour(0.7f, 0.0f, 0.0f, 1.0f), fraction, 270.0f, false, false,
                                                   CRGBFloatColour(0.0f, 0.0f, 0.0f, 1.0f), 20.0f));
            tickSliderContainer->AddChild(slider);

            level = heroExperience->GetTrainableStatLevel(HERO_TRAINABLE_STAT_STRENGTH_HEALTH);
            maxLevel = heroExperience->GetTrainableStatMaxLevel(HERO_TRAINABLE_STAT_STRENGTH_HEALTH);
            possibleLevels = heroExperience->GetTrainableStatPossibleLevelUpAmount(HERO_TRAINABLE_STAT_STRENGTH_HEALTH, fraction);
            fraction += possibleLevels + level;
            slider.Reset(manager->CreateTickSlider(inventoryExperience->GetExperienceStatName(HERO_TRAINABLE_STAT_STRENGTH_HEALTH), C2DCoordI(0, 30), level, maxLevel,
                                                   CRGBFloatColour(0.7f, 0.0f, 0.0f, 1.0f), fraction, 270.0f, false, false,
                                                   CRGBFloatColour(0.0f, 0.0f, 0.0f, 1.0f), 20.0f));
            tickSliderContainer->AddChild(slider);

            level = heroExperience->GetTrainableStatLevel(HERO_TRAINABLE_STAT_STRENGTH_TOUGHNESS);
            maxLevel = heroExperience->GetTrainableStatMaxLevel(HERO_TRAINABLE_STAT_STRENGTH_TOUGHNESS);
            possibleLevels = heroExperience->GetTrainableStatPossibleLevelUpAmount(HERO_TRAINABLE_STAT_STRENGTH_TOUGHNESS, fraction);
            fraction += possibleLevels + level;
            slider.Reset(manager->CreateTickSlider(inventoryExperience->GetExperienceStatName(HERO_TRAINABLE_STAT_STRENGTH_TOUGHNESS), C2DCoordI(0, 60), level, maxLevel,
                                                   CRGBFloatColour(0.7f, 0.0f, 0.0f, 1.0f), fraction, 270.0f, false, false,
                                                   CRGBFloatColour(0.0f, 0.0f, 0.0f, 1.0f), 20.0f));
            tickSliderContainer->AddChild(slider);

            // skill stats, yellow
            level = heroExperience->GetTrainableStatLevel(HERO_TRAINABLE_STAT_SKILL_ACCURACY);
            maxLevel = heroExperience->GetTrainableStatMaxLevel(HERO_TRAINABLE_STAT_SKILL_ACCURACY);
            possibleLevels = heroExperience->GetTrainableStatPossibleLevelUpAmount(HERO_TRAINABLE_STAT_SKILL_ACCURACY, fraction);
            fraction += possibleLevels + level;
            slider.Reset(manager->CreateTickSlider(inventoryExperience->GetExperienceStatName(HERO_TRAINABLE_STAT_SKILL_ACCURACY), C2DCoordI(0, 110), level, maxLevel,
                                                   CRGBFloatColour(0.7f, 0.7f, 0.0f, 1.0f), fraction, 270.0f, false, false,
                                                   CRGBFloatColour(0.0f, 0.0f, 0.0f, 1.0f), 20.0f));
            tickSliderContainer->AddChild(slider);

            level = heroExperience->GetTrainableStatLevel(HERO_TRAINABLE_STAT_SKILL_SPEED);
            maxLevel = heroExperience->GetTrainableStatMaxLevel(HERO_TRAINABLE_STAT_SKILL_SPEED);
            possibleLevels = heroExperience->GetTrainableStatPossibleLevelUpAmount(HERO_TRAINABLE_STAT_SKILL_SPEED, fraction);
            fraction += possibleLevels + level;
            slider.Reset(manager->CreateTickSlider(inventoryExperience->GetExperienceStatName(HERO_TRAINABLE_STAT_SKILL_SPEED), C2DCoordI(0, 140), level, maxLevel,
                                                   CRGBFloatColour(0.7f, 0.7f, 0.0f, 1.0f), fraction, 270.0f, false, false,
                                                   CRGBFloatColour(0.0f, 0.0f, 0.0f, 1.0f), 20.0f));
            tickSliderContainer->AddChild(slider);

            level = heroExperience->GetTrainableStatLevel(HERO_TRAINABLE_STAT_SKILL_STEALTH);
            maxLevel = heroExperience->GetTrainableStatMaxLevel(HERO_TRAINABLE_STAT_SKILL_STEALTH);
            possibleLevels = heroExperience->GetTrainableStatPossibleLevelUpAmount(HERO_TRAINABLE_STAT_SKILL_STEALTH, fraction);
            fraction += possibleLevels + level;
            slider.Reset(manager->CreateTickSlider(inventoryExperience->GetExperienceStatName(HERO_TRAINABLE_STAT_SKILL_STEALTH), C2DCoordI(0, 170), level, maxLevel,
                                                   CRGBFloatColour(0.7f, 0.7f, 0.0f, 1.0f), fraction, 270.0f, false, false,
                                                   CRGBFloatColour(0.0f, 0.0f, 0.0f, 1.0f), 20.0f));
            tickSliderContainer->AddChild(slider);

            // will stat, blue
            level = heroExperience->GetTrainableStatLevel(HERO_TRAINABLE_STAT_WILL_MAGIC_POWER);
            maxLevel = heroExperience->GetTrainableStatMaxLevel(HERO_TRAINABLE_STAT_WILL_MAGIC_POWER);
            possibleLevels = heroExperience->GetTrainableStatPossibleLevelUpAmount(HERO_TRAINABLE_STAT_WILL_MAGIC_POWER, fraction);
            fraction += possibleLevels + level;
            slider.Reset(manager->CreateTickSlider(inventoryExperience->GetExperienceStatName(HERO_TRAINABLE_STAT_WILL_MAGIC_POWER), C2DCoordI(0, 220), level, maxLevel,
                                                   CRGBFloatColour(0.0f, 0.0f, 0.7f, 1.0f), fraction, 270.0f, false, false,
                                                   CRGBFloatColour(0.0f, 0.0f, 0.0f, 1.0f), 20.0f));
            tickSliderContainer->AddChild(slider);

            // the hero ability rows below the stats, in three groups 15 apart
            {
            float yPos = 270.0f;
            AddHeroAbility(tickSliderContainer, HERO_ABILITY_ENFLAME_SPELL, yPos);
            AddHeroAbility(tickSliderContainer, HERO_ABILITY_FIREBALL_SPELL, yPos);
            AddHeroAbility(tickSliderContainer, HERO_ABILITY_BATTLE_CHARGE, yPos);
            AddHeroAbility(tickSliderContainer, HERO_ABILITY_DOUBLE_STRIKE, yPos);
            AddHeroAbility(tickSliderContainer, HERO_ABILITY_LIGHTNING_SPELL, yPos);
            AddHeroAbility(tickSliderContainer, HERO_ABILITY_DIVINE_WRATH_SPELL, yPos);
            AddHeroAbility(tickSliderContainer, HERO_ABILITY_UNHOLY_POWER_SPELL, yPos);
            yPos += 15.0f;
            AddHeroAbility(tickSliderContainer, HERO_ABILITY_FORCE_PUSH, yPos);
            AddHeroAbility(tickSliderContainer, HERO_ABILITY_TURNCOAT_SPELL, yPos);
            AddHeroAbility(tickSliderContainer, HERO_ABILITY_TIME_SPELL, yPos);
            AddHeroAbility(tickSliderContainer, HERO_ABILITY_DRAIN_LIFE_SPELL, yPos);
            AddHeroAbility(tickSliderContainer, HERO_ABILITY_SUMMON_SPELL, yPos);
            yPos += 15.0f;
            AddHeroAbility(tickSliderContainer, HERO_ABILITY_PHYSICAL_SHIELD_SPELL, yPos);
            AddHeroAbility(tickSliderContainer, HERO_ABILITY_BERSERK, yPos);
            AddHeroAbility(tickSliderContainer, HERO_ABILITY_ASSASSIN_RUSH, yPos);
            AddHeroAbility(tickSliderContainer, HERO_ABILITY_HEAL_LIFE_SPELL, yPos);
            AddHeroAbility(tickSliderContainer, HERO_ABILITY_GHOST_SWORD_SPELL, yPos);
            AddHeroAbility(tickSliderContainer, HERO_ABILITY_MULTI_ARROW, yPos);
            }
        }
    }
    return list;
}