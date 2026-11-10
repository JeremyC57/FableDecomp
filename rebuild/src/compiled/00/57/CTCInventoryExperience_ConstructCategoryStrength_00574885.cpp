// CTCInventoryExperience::ConstructCategoryStrength(NUISystem::CList* list) -- build the console experience screen's three category branches (strength, skill and will with its three spell sub-lists)
// Leaf: ?ConstructCategoryStrength@CTCInventoryExperience@@AAEXPAVCList@NUISystem@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00574885, 4357 bytes, thiscall, `ret 4`; size TU (biased `lea ebp,[esp-74h]` frame).
//   Identity: Ego_r.pdb / FableWin.pdb ?ConstructCategoryStrength@CTCInventoryExperience@@AAEXPAVCList@NUISystem@@@Z (private);
//   OpenInventoryInternal 0x00577216 calls it with the menu list; its PC twin ConstructCategoryStrengthPC is 0x005759BF.
// Shape: one CHANGE_ALPHA action (0xFF on all four experience sliders) is shared by every branch as its on-selected action. Each
//   branch is a UI_SUB_LIST with a title, filled by AddStatToList 0x005729D8 (strength 0..2, skill 3..5) or, for will, by three
//   spell sub-lists of AddAbilityToList 0x00570332 entries handed to AddMagicCategoryToList 0x0056F69F plus the magic-power stat;
//   NumItems counts the buyable entries for the "xN" suffix (GetXString 0x0056EA4E). The branch entry gets a CLOSE_MENU back
//   action, the sub list, its ring picture, its subtitle and description texts (the description also into pTextViewport), a
//   tutorial sound (AddSoundAction 0x0056F615) and a CHANGE_ALPHA 0x28 action on the two other category sliders.
// Frame: the subtitle CText* is one local reused by all three branches, each description CText* its own local (retail keeps
//   the subtitle pointers at [ebp+68h] and the description pointers at [ebp+6Ch] across the CCharString temp).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_PRIVATE
#define FABLE_RETAIL_EHEROTRAINABLESTATTYPE
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CACTION
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CTEXT
#define FABLE_RETAIL_CMOVIE
#define FABLE_RETAIL_CDATABANK
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCInventoryExperience.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CClickable.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/CText.h"
#include "engine/CMovie.h"
#include "engine/C2DCoordI.h"
#include "engine/CRGBFloatColour.h"
#include "fable_game_text_bank.h"
#include "fable_game.h"

using namespace NUISystem;
void CTCInventoryExperience::ConstructCategoryStrength(CList* list)
{
    const NGameText::CDataBank& textBank = CMainGameComponent::Get()->GetTextBank();
    CManager* manager = CManager::GetInstance();
    CCountedPointer<CAction> highlight(new CAction);
    highlight->ActionType = UI_ACTION_TYPE_CHANGE_ALPHA;
    highlight->AddParam(new CActionParamSLONG(0xFF));
    highlight->AddParam(ExpSliders[0], highlight);
    highlight->AddParam(ExpSliders[1], highlight);
    highlight->AddParam(ExpSliders[2], highlight);
    highlight->AddParam(ExpSliders[3], highlight);

    // Strength.
    CList* subList = static_cast<CList*>(manager->CreateComponent("UI_SUB_LIST", false));
    CCountedPointer<CComponent> strengthTitle(manager->CreateComponent("UI_TITLE_EXPERIENCE_STRENGTH", false));
    subList->AddNonScrollingChild(strengthTitle);
    CCountedPointer<CComponent> subListPointer(subList);
    subList->InitialiseOffsets(3);
    NumItems = 0;
    AddStatToList(HERO_STAT_STRENGTH_PHYSIQUE, subList, false, 0);
    AddStatToList(HERO_STAT_STRENGTH_HEALTH, subList, false, 0);
    AddStatToList(HERO_STAT_STRENGTH_TOUGHNESS, subList, false, 0);
    CWideString name = textBank.GetTextBySymbol("TEXT_GUI_MENU_STRENGTH") + GetXString(NumItems);
    CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(name, 0.0f, 0.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",
                                                                        0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    AddSoundAction(entry.Get(), "SND_TEXT_QST_TUTORIAL_GUI_EXPSPEND_SUB_STRENGTH");
    CCountedPointer<CAction> back(new CAction);
    back->ActionType = UI_ACTION_TYPE_CLOSE_MENU;
    entry->AddActionOnBack(back);
    entry->AddChild(subListPointer);
    CCountedPointer<CComponent> strengthRing(manager->CreateComponent("UI_RING_PIC_EXP_STRENGTH", false));
    entry->AddChild(strengthRing);
    CCountedPointer<CComponent> strengthSubtitle(manager->CreateComponent("UI_ONE_OFF_SUBTITLE", false));
    CText* subtitleText = static_cast<CText*>(strengthSubtitle.Get());
    subtitleText->SetValue("TEXT_GUI_MENU_STRENGTH", true, true);
    entry->AddChild(strengthSubtitle);
    CCountedPointer<CComponent> strengthDescription(manager->CreateComponent("UI_EXPSPEND_CAT_DESC", false));
    CText* strengthDescriptionText = static_cast<CText*>(strengthDescription.Get());
    strengthDescriptionText->SetValue("TEXT_GUI_MENUGUIDE_EXPSPEND_PHYSICAL", true, true);
    entry->AddChild(strengthDescription);
    if (pTextViewport)
        pTextViewport->AddFakeChild(strengthDescription.Get());
    CCountedPointer<CAction> strengthFade(new CAction);
    strengthFade->ActionType = UI_ACTION_TYPE_CHANGE_ALPHA;
    strengthFade->AddParam(new CActionParamSLONG(0x28));
    strengthFade->AddParam(ExpSliders[2], strengthFade);
    strengthFade->AddParam(ExpSliders[3], strengthFade);
    entry->AddAction(strengthFade);
    entry->AddActionOnSelected(highlight);
    list->AddScrollingChildAndSetOffsets(entry);

    // Skill.
    subList = static_cast<CList*>(manager->CreateComponent("UI_SUB_LIST", false));
    subListPointer.Reset(subList);
    CCountedPointer<CComponent> skillTitle(manager->CreateComponent("UI_TITLE_EXPERIENCE_SKILL", false));
    subList->AddNonScrollingChild(skillTitle);
    subList->InitialiseOffsets(3);
    NumItems = 0;
    AddStatToList(HERO_STAT_SKILL_SPEED, subList, false, 0);
    AddStatToList(HERO_STAT_SKILL_ACCURACY, subList, false, 0);
    AddStatToList(HERO_STAT_SKILL_STEALTH, subList, false, 0);
    name = textBank.GetTextBySymbol("TEXT_GUI_MENU_SKILL") + GetXString(NumItems);
    entry.Reset(manager->CreateMenuEntryComponent(name, 0.0f, 0.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",
                                                  0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    back.Reset(new CAction);
    back->ActionType = UI_ACTION_TYPE_CLOSE_MENU;
    entry->AddActionOnBack(back);
    entry->AddChild(subListPointer);
    CCountedPointer<CComponent> skillRing(manager->CreateComponent("UI_RING_PIC_EXP_SKILL", false));
    entry->AddChild(skillRing);
    CCountedPointer<CComponent> skillDescription(manager->CreateComponent("UI_EXPSPEND_CAT_DESC", false));
    CText* skillDescriptionText = static_cast<CText*>(skillDescription.Get());
    skillDescriptionText->SetValue("TEXT_GUI_MENUGUIDE_EXPSPEND_SKILL", true, true);
    entry->AddChild(skillDescription);
    if (pTextViewport)
        pTextViewport->AddFakeChild(skillDescription.Get());
    CCountedPointer<CComponent> skillSubtitle(manager->CreateComponent("UI_ONE_OFF_SUBTITLE", false));
    subtitleText = static_cast<CText*>(skillSubtitle.Get());
    subtitleText->SetValue("TEXT_GUI_MENU_SKILL", true, true);
    entry->AddChild(skillSubtitle);
    AddSoundAction(entry.Get(), "SND_TEXT_QST_TUTORIAL_GUI_EXPSPEND_SUB_SKILL");
    CCountedPointer<CAction> skillFade(new CAction);
    skillFade->ActionType = UI_ACTION_TYPE_CHANGE_ALPHA;
    skillFade->AddParam(new CActionParamSLONG(0x28));
    skillFade->AddParam(ExpSliders[1], skillFade);
    skillFade->AddParam(ExpSliders[3], skillFade);
    entry->AddAction(skillFade);
    entry->AddActionOnSelected(highlight);
    list->AddScrollingChildAndSetOffsets(entry);

    // Will: three spell sub-lists, then the magic-power stat.
    subList = static_cast<CList*>(manager->CreateComponent("UI_SUB_LIST", false));
    subListPointer.Reset(subList);
    CCountedPointer<CComponent> willListTitle(manager->CreateComponent("UI_TITLE_EXPERIENCE_WILL", false));
    subList->AddNonScrollingChild(willListTitle);
    subList->InitialiseOffsets(3);
    NumItems = 0;
    CList* attackList = static_cast<CList*>(manager->CreateComponent("UI_SUB_LIST", false));
    CCountedPointer<CComponent> attackTitle(manager->CreateComponent("UI_TITLE_EXPERIENCE_ATTACK_SPELLS", false));
    attackList->AddNonScrollingChild(attackTitle);
    attackList->InitialiseOffsets(7);
    AddAbilityToList(HERO_ABILITY_ENFLAME_SPELL, attackList, false);
    AddAbilityToList(HERO_ABILITY_FIREBALL_SPELL, attackList, false);
    AddAbilityToList(HERO_ABILITY_BATTLE_CHARGE, attackList, false);
    AddAbilityToList(HERO_ABILITY_DOUBLE_STRIKE, attackList, false);
    AddAbilityToList(HERO_ABILITY_LIGHTNING_SPELL, attackList, false);
    AddAbilityToList(HERO_ABILITY_DIVINE_WRATH_SPELL, attackList, false);
    AddAbilityToList(HERO_ABILITY_UNHOLY_POWER_SPELL, attackList, false);
    AddMagicCategoryToList(subList, textBank.GetTextBySymbol("TEXT_GUI_MENU_ATTACK_SPELLS") + GetXString(NumItems),
                           "UI_RING_PIC_EXP_SPEND_WILL_ATTACK", attackList, L"TEXT_GUI_MENU_ATTACK_SPELLS",
                           "TEXT_GUI_MENUGUIDE_EXPSPEND_MAGIC_COMBAT");
    long willItems = NumItems;
    NumItems = 0;
    CList* surroundList = static_cast<CList*>(manager->CreateComponent("UI_SUB_LIST", false));
    CCountedPointer<CComponent> surroundTitle(manager->CreateComponent("UI_TITLE_EXPERIENCE_SURROUND_SPELLS", false));
    surroundList->AddNonScrollingChild(surroundTitle);
    surroundList->InitialiseOffsets(5);
    AddAbilityToList(HERO_ABILITY_FORCE_PUSH, surroundList, false);
    AddAbilityToList(HERO_ABILITY_TURNCOAT_SPELL, surroundList, false);
    AddAbilityToList(HERO_ABILITY_TIME_SPELL, surroundList, false);
    AddAbilityToList(HERO_ABILITY_DRAIN_LIFE_SPELL, surroundList, false);
    AddAbilityToList(HERO_ABILITY_SUMMON_SPELL, surroundList, false);
    AddMagicCategoryToList(subList, textBank.GetTextBySymbol("TEXT_GUI_MENU_SURROUND_SPELLS") + GetXString(NumItems),
                           "UI_RING_PIC_EXP_SPEND_WILL_SURROUND", surroundList, L"TEXT_GUI_MENU_SURROUND_SPELLS",
                           "TEXT_GUI_MENUGUIDE_EXPSPEND_MAGIC_SURROUND");
    willItems += NumItems;
    NumItems = 0;
    CList* physicalList = static_cast<CList*>(manager->CreateComponent("UI_SUB_LIST", false));
    CCountedPointer<CComponent> physicalTitle(manager->CreateComponent("UI_TITLE_EXPERIENCE_PHYSICAL_SPELLS", false));
    physicalList->AddNonScrollingChild(physicalTitle);
    physicalList->InitialiseOffsets(6);
    AddAbilityToList(HERO_ABILITY_PHYSICAL_SHIELD_SPELL, physicalList, false);
    AddAbilityToList(HERO_ABILITY_BERSERK, physicalList, false);
    AddAbilityToList(HERO_ABILITY_ASSASSIN_RUSH, physicalList, false);
    AddAbilityToList(HERO_ABILITY_HEAL_LIFE_SPELL, physicalList, false);
    AddAbilityToList(HERO_ABILITY_GHOST_SWORD_SPELL, physicalList, false);
    AddAbilityToList(HERO_ABILITY_MULTI_ARROW, physicalList, false);
    AddMagicCategoryToList(subList, textBank.GetTextBySymbol("TEXT_GUI_MENU_PHYSICAL_SPELLS") + GetXString(NumItems),
                           "UI_RING_PIC_EXP_SPEND_WILL_PHYSICAL", physicalList, L"TEXT_GUI_MENU_PHYSICAL_SPELLS",
                           "TEXT_GUI_MENUGUIDE_EXPSPEND_MAGIC_PHYSICAL");
    willItems += NumItems;
    NumItems = 0;
    CCountedPointer<CComponent> magicPowerTitle(manager->CreateComponent("UI_TITLE_EXPERIENCE_WILL", false));
    AddStatToList(HERO_STAT_WILL_MAGIC_POWER, subList, false, &magicPowerTitle);
    willItems += NumItems;
    NumItems = 0;
    name = textBank.GetTextBySymbol("TEXT_GUI_MENU_WILL") + GetXString(willItems);
    entry.Reset(manager->CreateMenuEntryComponent(name, 0.0f, 0.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",
                                                  0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    AddSoundAction(entry.Get(), "SND_TEXT_QST_TUTORIAL_GUI_EXPSPEND_SUB_WILL");
    back.Reset(new CAction);
    back->ActionType = UI_ACTION_TYPE_CLOSE_MENU;
    entry->AddActionOnBack(back);
    entry->AddChild(subListPointer);
    CCountedPointer<CComponent> willRing(manager->CreateComponent("UI_RING_PIC_EXP_WILL", false));
    entry->AddChild(willRing);
    CCountedPointer<CComponent> willDescription(manager->CreateComponent("UI_EXPSPEND_CAT_DESC", false));
    CText* willDescriptionText = static_cast<CText*>(willDescription.Get());
    willDescriptionText->SetValue("TEXT_GUI_MENUGUIDE_EXPSPEND_MAGIC", true, true);
    entry->AddChild(willDescription);
    if (pTextViewport)
        pTextViewport->AddFakeChild(willDescription.Get());
    CCountedPointer<CComponent> willSubtitle(manager->CreateComponent("UI_ONE_OFF_SUBTITLE", false));
    subtitleText = static_cast<CText*>(willSubtitle.Get());
    subtitleText->SetValue("TEXT_GUI_MENU_WILL", true, true);
    entry->AddChild(willSubtitle);
    CCountedPointer<CAction> willFade(new CAction);
    willFade->ActionType = UI_ACTION_TYPE_CHANGE_ALPHA;
    willFade->AddParam(new CActionParamSLONG(0x28));
    willFade->AddParam(ExpSliders[1], willFade);
    willFade->AddParam(ExpSliders[2], willFade);
    entry->AddAction(willFade);
    entry->AddActionOnSelected(highlight);
    list->AddScrollingChildAndSetOffsets(entry);
}