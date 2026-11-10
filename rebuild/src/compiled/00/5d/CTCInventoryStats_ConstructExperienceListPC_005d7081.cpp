// CTCInventoryStats::ConstructExperienceListPC(NUISystem::CComponent* parent) -- builds the PC (mouse) experience page of the hero stats screen: the "PC_STATS_EXP_LIST" component, mouse-movement listen / ignore actions for the parent's scroll-list buttons and, inside the experience frame, the scroll-stats viewport buttons (send the viewport right-stick up / down events) with their own listen / ignore actions, the frame's position children (viewport, tick and level slider containers, title, ring picture), then the level sliders, one tick slider per trainable stat and one row per hero ability as on the console page
// Leaf: ?ConstructExperienceListPC@CTCInventoryStats@@UAEPAVCComponent@NUISystem@@PAV23@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D7081, 3882 bytes, `ret 4`. Plain frame-pointer TU (`push ebp ; mov ebp,esp ; sub esp,74h`,
//   `leave`): optimize("y",off) + ("s",on), as the landed console twin ConstructExperienceList 0x005D6748 of the same object.
// Identity (proven): slot of the CTCInventoryStats vtable 0x01254C28 (abs .rdata reference 0x01254C3C, next to the console
//   ConstructExperienceList slot), Ego_r.pdb / FableWin.pdb ?ConstructExperienceListPC@CTCInventoryStats@@UAEPAVCComponent@NUISystem@@PAV23@@Z
//   (engine/CTCInventoryStats.h); the Ego_r donor order CreateExperienceSliders / AddHeroAbility / ConstructExperienceList /
//   ConstructExperienceListPC matches retail 0x005CDD86 / 0x005D64DD / 0x005D6748 / 0x005D7081; the strings PC_STATS_EXP_*.
// Callees: as the console twin, plus CAction::AddParam(CActionParam*, CCountedPointer<CAction>&) 0x0053FDE0 (the component's
//   CActionParam base at +0x18), CActionParamULONG (vtable 0x0122F844, type 5) / CActionParamSLONG (0x012366F4, type 3) built
//   inline; the CComponent virtuals FindChild 3, AddChild 59, AddActionOnSelected 67, AddActionOnInactive 70,
//   AddActionOnLeftClicked 71, AddActionOnLeftHeld 72, AddPositionChild 124 and CViewport::AddFakeChild 142.
// Shape: the listen (UI_ACTION_TYPE_OBSERVE_EVENT 245) / ignore (UI_ACTION_TYPE_IGNORE_EVENT 249) pairs are the body of
//   NUISystem::CManager::AddActionsListenIgnoreOnList 0x0041DC9D written out (another TU); slider widths 303 / 53 where the
//   console page has 270 / 20.
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
#define FABLE_RETAIL_CACTIONPARAMULONG
#define FABLE_RETAIL_CACTIONPARAMSLONG
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
#include "engine/NUISystem_CActionParamULONG.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/CCountedPointer.h"

using NUISystem::CComponent;
using NUISystem::CAction;
using NUISystem::CActionParamULONG;
using NUISystem::CActionParamSLONG;

enum EHeroTrainableStatType {                                     // enumerator spellings unproven (console twin 0x005D6748)
    HERO_TRAINABLE_STAT_STRENGTH_PHYSIQUE = 0,
    HERO_TRAINABLE_STAT_STRENGTH_HEALTH = 1,
    HERO_TRAINABLE_STAT_STRENGTH_TOUGHNESS = 2,
    HERO_TRAINABLE_STAT_SKILL_ACCURACY = 3,
    HERO_TRAINABLE_STAT_SKILL_SPEED = 4,
    HERO_TRAINABLE_STAT_SKILL_STEALTH = 5,
    HERO_TRAINABLE_STAT_WILL_MAGIC_POWER = 9
};

CComponent* CTCInventoryStats::ConstructExperienceListPC(CComponent* parent)
{
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CTCHeroExperience* heroExperience = 0;
    CTCInventoryExperience* inventoryExperience = 0;
    ParentThing->GetTC(TCI_HERO_EXPERIENCE, &heroExperience);
    ParentThing->GetTC(TCI_INVENTORY_EXPERIENCE, &inventoryExperience);

    CComponent* list = manager->CreateComponent("PC_STATS_EXP_LIST", false);
    CComponent* frame = list->FindChild("PC_UI_EXPERIENCE_FRAME");
    CComponent* scrollListUp = parent->FindChild("SCROLL_LIST_UP");
    CComponent* scrollListDown = parent->FindChild("SCROLL_LIST_DOWN");

    // the list observes / ignores mouse movement over the parent's scroll-list buttons
    {
        CCountedPointer<CAction> observeUp(new CAction);
        observeUp->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeUp->AddParam(scrollListUp, observeUp);
        observeUp->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        CCountedPointer<CAction> ignoreUp(new CAction);
        ignoreUp->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreUp->AddParam(scrollListUp, ignoreUp);
        ignoreUp->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnSelected(ignoreUp);
        list->AddActionOnInactive(observeUp);
        CCountedPointer<CAction> observeDown(new CAction);
        observeDown->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeDown->AddParam(scrollListDown, observeDown);
        observeDown->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        CCountedPointer<CAction> ignoreDown(new CAction);
        ignoreDown->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreDown->AddParam(scrollListDown, ignoreDown);
        ignoreDown->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnSelected(ignoreDown);
        list->AddActionOnInactive(observeDown);
    }

    if (frame) {
        CComponent* viewportUp = frame->FindChild("SCROLL_STATS_VIEWPORT_UP");
        CComponent* viewportDown = frame->FindChild("SCROLL_STATS_VIEWPORT_DOWN");
        NUISystem::CViewport* viewport = (NUISystem::CViewport*)list->FindChild("PC_STATS_EXP_VIEWPORT");
        if (viewport)
            frame->AddPositionChild(viewport);

        // clicking or holding the viewport buttons sends the viewport the right-stick up / down event
        CCountedPointer<CAction> sendUp(new CAction);
        sendUp->ActionType = NUISystem::UI_ACTION_TYPE_SEND_OWNED_EVENT;
        sendUp->UIActionParams.push_back(new CActionParamSLONG(NUISystem::UI_EVENT_RIGHT_STICK_UP));
        sendUp->AddParam(viewport, sendUp);
        viewportUp->AddActionOnLeftClicked(sendUp);
        viewportUp->AddActionOnLeftHeld(sendUp);
        CCountedPointer<CAction> sendDown(new CAction);
        sendDown->ActionType = NUISystem::UI_ACTION_TYPE_SEND_OWNED_EVENT;
        sendDown->UIActionParams.push_back(new CActionParamSLONG(NUISystem::UI_EVENT_RIGHT_STICK_DOWN));
        sendDown->AddParam(viewport, sendDown);
        viewportDown->AddActionOnLeftClicked(sendDown);
        viewportDown->AddActionOnLeftHeld(sendDown);

        CCountedPointer<CAction> observeUp(new CAction);
        observeUp->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeUp->AddParam(viewportUp, observeUp);
        observeUp->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        CCountedPointer<CAction> ignoreUp(new CAction);
        ignoreUp->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreUp->AddParam(viewportUp, ignoreUp);
        ignoreUp->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnSelected(observeUp);
        list->AddActionOnInactive(ignoreUp);
        CCountedPointer<CAction> observeDown(new CAction);
        observeDown->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeDown->AddParam(viewportDown, observeDown);
        observeDown->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        CCountedPointer<CAction> ignoreDown(new CAction);
        ignoreDown->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreDown->AddParam(viewportDown, ignoreDown);
        ignoreDown->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnSelected(observeDown);
        list->AddActionOnInactive(ignoreDown);

        CComponent* tickSliderContainer = list->FindChild("PC_STATS_EXP_TICK_SLIDER_CONTAINER");
        if (tickSliderContainer)
            viewport->AddFakeChild(tickSliderContainer);
    }

    CComponent* screen = list->FindChild("PC_STATS_EXP_SCREEN");
    if (screen) {
        CComponent* levelSliderContainer = screen->FindChild("PC_STATS_EXP_LEVEL_SLIDER_CONTAINER");
        if (levelSliderContainer)
            CreateExperienceSliders(levelSliderContainer, 2, true);
        CComponent* tickSliderContainer = screen->FindChild("PC_STATS_EXP_TICK_SLIDER_CONTAINER");
        if (tickSliderContainer) {
            if (frame) {
                frame->AddPositionChild(tickSliderContainer);
                CComponent* child = screen->FindChild("PC_STATS_EXP_LEVEL_SLIDER_CONTAINER");
                if (child)
                    frame->AddPositionChild(child);
                child = screen->FindChild("PC_TITLE_EXPERIENCE");
                if (child)
                    frame->AddPositionChild(child);
                child = screen->FindChild("PC_RING_PIC_EXPERIENCE_INDEPENDANT");
                if (child)
                    frame->AddPositionChild(child);
            }
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
                                                   CRGBFloatColour(0.7f, 0.0f, 0.0f, 1.0f), fraction, 303.0f, false, false,
                                                   CRGBFloatColour(0.0f, 0.0f, 0.0f, 1.0f), 53.0f));
            tickSliderContainer->AddChild(slider);

            level = heroExperience->GetTrainableStatLevel(HERO_TRAINABLE_STAT_STRENGTH_HEALTH);
            maxLevel = heroExperience->GetTrainableStatMaxLevel(HERO_TRAINABLE_STAT_STRENGTH_HEALTH);
            possibleLevels = heroExperience->GetTrainableStatPossibleLevelUpAmount(HERO_TRAINABLE_STAT_STRENGTH_HEALTH, fraction);
            fraction += possibleLevels + level;
            slider.Reset(manager->CreateTickSlider(inventoryExperience->GetExperienceStatName(HERO_TRAINABLE_STAT_STRENGTH_HEALTH), C2DCoordI(0, 30), level, maxLevel,
                                                   CRGBFloatColour(0.7f, 0.0f, 0.0f, 1.0f), fraction, 303.0f, false, false,
                                                   CRGBFloatColour(0.0f, 0.0f, 0.0f, 1.0f), 53.0f));
            tickSliderContainer->AddChild(slider);

            level = heroExperience->GetTrainableStatLevel(HERO_TRAINABLE_STAT_STRENGTH_TOUGHNESS);
            maxLevel = heroExperience->GetTrainableStatMaxLevel(HERO_TRAINABLE_STAT_STRENGTH_TOUGHNESS);
            possibleLevels = heroExperience->GetTrainableStatPossibleLevelUpAmount(HERO_TRAINABLE_STAT_STRENGTH_TOUGHNESS, fraction);
            fraction += possibleLevels + level;
            slider.Reset(manager->CreateTickSlider(inventoryExperience->GetExperienceStatName(HERO_TRAINABLE_STAT_STRENGTH_TOUGHNESS), C2DCoordI(0, 60), level, maxLevel,
                                                   CRGBFloatColour(0.7f, 0.0f, 0.0f, 1.0f), fraction, 303.0f, false, false,
                                                   CRGBFloatColour(0.0f, 0.0f, 0.0f, 1.0f), 53.0f));
            tickSliderContainer->AddChild(slider);

            // skill stats, yellow
            level = heroExperience->GetTrainableStatLevel(HERO_TRAINABLE_STAT_SKILL_ACCURACY);
            maxLevel = heroExperience->GetTrainableStatMaxLevel(HERO_TRAINABLE_STAT_SKILL_ACCURACY);
            possibleLevels = heroExperience->GetTrainableStatPossibleLevelUpAmount(HERO_TRAINABLE_STAT_SKILL_ACCURACY, fraction);
            fraction += possibleLevels + level;
            slider.Reset(manager->CreateTickSlider(inventoryExperience->GetExperienceStatName(HERO_TRAINABLE_STAT_SKILL_ACCURACY), C2DCoordI(0, 110), level, maxLevel,
                                                   CRGBFloatColour(0.7f, 0.7f, 0.0f, 1.0f), fraction, 303.0f, false, false,
                                                   CRGBFloatColour(0.0f, 0.0f, 0.0f, 1.0f), 53.0f));
            tickSliderContainer->AddChild(slider);

            level = heroExperience->GetTrainableStatLevel(HERO_TRAINABLE_STAT_SKILL_SPEED);
            maxLevel = heroExperience->GetTrainableStatMaxLevel(HERO_TRAINABLE_STAT_SKILL_SPEED);
            possibleLevels = heroExperience->GetTrainableStatPossibleLevelUpAmount(HERO_TRAINABLE_STAT_SKILL_SPEED, fraction);
            fraction += possibleLevels + level;
            slider.Reset(manager->CreateTickSlider(inventoryExperience->GetExperienceStatName(HERO_TRAINABLE_STAT_SKILL_SPEED), C2DCoordI(0, 140), level, maxLevel,
                                                   CRGBFloatColour(0.7f, 0.7f, 0.0f, 1.0f), fraction, 303.0f, false, false,
                                                   CRGBFloatColour(0.0f, 0.0f, 0.0f, 1.0f), 53.0f));
            tickSliderContainer->AddChild(slider);

            level = heroExperience->GetTrainableStatLevel(HERO_TRAINABLE_STAT_SKILL_STEALTH);
            maxLevel = heroExperience->GetTrainableStatMaxLevel(HERO_TRAINABLE_STAT_SKILL_STEALTH);
            possibleLevels = heroExperience->GetTrainableStatPossibleLevelUpAmount(HERO_TRAINABLE_STAT_SKILL_STEALTH, fraction);
            fraction += possibleLevels + level;
            slider.Reset(manager->CreateTickSlider(inventoryExperience->GetExperienceStatName(HERO_TRAINABLE_STAT_SKILL_STEALTH), C2DCoordI(0, 170), level, maxLevel,
                                                   CRGBFloatColour(0.7f, 0.7f, 0.0f, 1.0f), fraction, 303.0f, false, false,
                                                   CRGBFloatColour(0.0f, 0.0f, 0.0f, 1.0f), 53.0f));
            tickSliderContainer->AddChild(slider);

            // will stat, blue
            level = heroExperience->GetTrainableStatLevel(HERO_TRAINABLE_STAT_WILL_MAGIC_POWER);
            maxLevel = heroExperience->GetTrainableStatMaxLevel(HERO_TRAINABLE_STAT_WILL_MAGIC_POWER);
            possibleLevels = heroExperience->GetTrainableStatPossibleLevelUpAmount(HERO_TRAINABLE_STAT_WILL_MAGIC_POWER, fraction);
            fraction += possibleLevels + level;
            slider.Reset(manager->CreateTickSlider(inventoryExperience->GetExperienceStatName(HERO_TRAINABLE_STAT_WILL_MAGIC_POWER), C2DCoordI(0, 220), level, maxLevel,
                                                   CRGBFloatColour(0.0f, 0.0f, 0.7f, 1.0f), fraction, 303.0f, false, false,
                                                   CRGBFloatColour(0.0f, 0.0f, 0.0f, 1.0f), 53.0f));
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