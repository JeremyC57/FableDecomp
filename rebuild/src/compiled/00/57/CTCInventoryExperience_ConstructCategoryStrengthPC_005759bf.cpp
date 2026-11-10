// CTCInventoryExperience::ConstructCategoryStrengthPC(NUISystem::CList* list) -- PC twin of ConstructCategoryStrength: builds the strength, skill and will branches of the PC experience screen, every title, ring, subtitle and description also placed in the list's PC_UI_FRAME
// Leaf: ?ConstructCategoryStrengthPC@CTCInventoryExperience@@AAEXPAVCList@NUISystem@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005759BF, 4684 bytes, thiscall, `ret 4`; size TU (biased `lea ebp,[esp-74h]` frame).
//   Identity: Ego_r.pdb / FableWin.pdb ?ConstructCategoryStrengthPC@CTCInventoryExperience@@AAEXPAVCList@NUISystem@@@Z (private);
//   OpenInventoryInternalPC 0x00577AC0 calls it with the menu list; console twin ConstructCategoryStrength 0x00574885.
// Shape: the console body with PC_* component names, PC_MENU_ENTRY_BRANCH_TEMPLATE entries and the PC callees AddStatToListPC
//   0x00573792 / AddAbilityToListPC 0x00571280 / AddMagicCategoryToListPC 0x0056F8A4, each handed the PC_UI_FRAME found on the
//   list (FindChild, slot 3) once, after the strength title is made; every title, ring picture, subtitle and description is
//   also given to the frame (AddPositionChild, slot 124): the strength title before it joins its list, the strength
//   description after it joins pTextViewport, the other descriptions before.
// Frame: as in the console twin, one subtitle CText* reused by the three branches and a CText* per description.
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
void CTCInventoryExperience::ConstructCategoryStrengthPC(CList* list)
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
    CList* subList = static_cast<CList*>(manager->CreateComponent("PC_SUB_LIST", false));
    CCountedPointer<CComponent> strengthTitle(manager->CreateComponent("PC_TITLE_EXPERIENCE_STRENGTH", false));
    CComponent* frame = list->FindChild("PC_UI_FRAME");
    frame->AddPositionChild(strengthTitle.Get());
    subList->AddNonScrollingChild(strengthTitle);
    CCountedPointer<CComponent> subListPointer(subList);
    subList->InitialiseOffsets(3);
    NumItems = 0;
    AddStatToListPC(frame, HERO_STAT_STRENGTH_PHYSIQUE, subList, false, 0);
    AddStatToListPC(frame, HERO_STAT_STRENGTH_HEALTH, subList, false, 0);
    AddStatToListPC(frame, HERO_STAT_STRENGTH_TOUGHNESS, subList, false, 0);
    CWideString name = textBank.GetTextBySymbol("TEXT_GUI_MENU_STRENGTH") + GetXString(NumItems);
    CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(name, 0.0f, 0.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE",
                                                                        0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    AddSoundAction(entry.Get(), "SND_TEXT_QST_TUTORIAL_GUI_EXPSPEND_SUB_STRENGTH");
    CCountedPointer<CAction> back(new CAction);
    back->ActionType = UI_ACTION_TYPE_CLOSE_MENU;
    entry->AddActionOnBack(back);
    entry->AddChild(subListPointer);
    CCountedPointer<CComponent> strengthRing(manager->CreateComponent("PC_RING_PIC_EXP_STRENGTH", false));
    entry->AddChild(strengthRing);
    frame->AddPositionChild(strengthRing.Get());
    CCountedPointer<CComponent> strengthSubtitle(manager->CreateComponent("PC_SUBTITLE", false));
    CText* subtitleText = static_cast<CText*>(strengthSubtitle.Get());
    subtitleText->SetValue("TEXT_GUI_MENU_STRENGTH", true, true);
    entry->AddChild(strengthSubtitle);
    frame->AddPositionChild(strengthSubtitle.Get());
    CCountedPointer<CComponent> strengthDescription(manager->CreateComponent("PC_EXPSPEND_CAT_DESC", false));
    CText* strengthDescriptionText = static_cast<CText*>(strengthDescription.Get());
    strengthDescriptionText->SetValue("TEXT_GUI_MENUGUIDE_EXPSPEND_PHYSICAL", true, true);
    entry->AddChild(strengthDescription);
    if (pTextViewport)
        pTextViewport->AddFakeChild(strengthDescription.Get());
    frame->AddPositionChild(strengthDescription.Get());
    CCountedPointer<CAction> strengthFade(new CAction);
    strengthFade->ActionType = UI_ACTION_TYPE_CHANGE_ALPHA;
    strengthFade->AddParam(new CActionParamSLONG(0x28));
    strengthFade->AddParam(ExpSliders[2], strengthFade);
    strengthFade->AddParam(ExpSliders[3], strengthFade);
    entry->AddAction(strengthFade);
    entry->AddActionOnSelected(highlight);
    list->AddScrollingChildAndSetOffsets(entry);

    // Skill.
    subList = static_cast<CList*>(manager->CreateComponent("PC_SUB_LIST", false));
    subListPointer.Reset(subList);
    CCountedPointer<CComponent> skillTitle(manager->CreateComponent("PC_TITLE_EXPERIENCE_SKILL", false));
    subList->AddNonScrollingChild(skillTitle);
    frame->AddPositionChild(skillTitle.Get());
    subList->InitialiseOffsets(3);
    NumItems = 0;
    AddStatToListPC(frame, HERO_STAT_SKILL_SPEED, subList, false, 0);
    AddStatToListPC(frame, HERO_STAT_SKILL_ACCURACY, subList, false, 0);
    AddStatToListPC(frame, HERO_STAT_SKILL_STEALTH, subList, false, 0);
    name = textBank.GetTextBySymbol("TEXT_GUI_MENU_SKILL") + GetXString(NumItems);
    entry.Reset(manager->CreateMenuEntryComponent(name, 0.0f, 0.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE",
                                                  0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    back.Reset(new CAction);
    back->ActionType = UI_ACTION_TYPE_CLOSE_MENU;
    entry->AddActionOnBack(back);
    entry->AddChild(subListPointer);
    CCountedPointer<CComponent> skillRing(manager->CreateComponent("PC_RING_PIC_EXP_SKILL", false));
    entry->AddChild(skillRing);
    frame->AddPositionChild(skillRing.Get());
    CCountedPointer<CComponent> skillDescription(manager->CreateComponent("PC_EXPSPEND_CAT_DESC", false));
    CText* skillDescriptionText = static_cast<CText*>(skillDescription.Get());
    skillDescriptionText->SetValue("TEXT_GUI_MENUGUIDE_EXPSPEND_SKILL", true, true);
    entry->AddChild(skillDescription);
    frame->AddPositionChild(skillDescription.Get());
    if (pTextViewport)
        pTextViewport->AddFakeChild(skillDescription.Get());
    CCountedPointer<CComponent> skillSubtitle(manager->CreateComponent("PC_SUBTITLE", false));
    subtitleText = static_cast<CText*>(skillSubtitle.Get());
    subtitleText->SetValue("TEXT_GUI_MENU_SKILL", true, true);
    entry->AddChild(skillSubtitle);
    frame->AddPositionChild(skillSubtitle.Get());
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
    subList = static_cast<CList*>(manager->CreateComponent("PC_SUB_LIST", false));
    subListPointer.Reset(subList);
    CCountedPointer<CComponent> willListTitle(manager->CreateComponent("PC_TITLE_EXPERIENCE_WILL", false));
    subList->AddNonScrollingChild(willListTitle);
    frame->AddPositionChild(willListTitle.Get());
    subList->InitialiseOffsets(3);
    NumItems = 0;
    CList* attackList = static_cast<CList*>(manager->CreateComponent("PC_SUB_LIST", false));
    CCountedPointer<CComponent> attackTitle(manager->CreateComponent("PC_TITLE_EXPERIENCE_ATTACK_SPELLS", false));
    attackList->AddNonScrollingChild(attackTitle);
    frame->AddPositionChild(attackTitle.Get());
    attackList->InitialiseOffsets(7);
    AddAbilityToListPC(frame, HERO_ABILITY_ENFLAME_SPELL, attackList, false);
    AddAbilityToListPC(frame, HERO_ABILITY_FIREBALL_SPELL, attackList, false);
    AddAbilityToListPC(frame, HERO_ABILITY_BATTLE_CHARGE, attackList, false);
    AddAbilityToListPC(frame, HERO_ABILITY_DOUBLE_STRIKE, attackList, false);
    AddAbilityToListPC(frame, HERO_ABILITY_LIGHTNING_SPELL, attackList, false);
    AddAbilityToListPC(frame, HERO_ABILITY_DIVINE_WRATH_SPELL, attackList, false);
    AddAbilityToListPC(frame, HERO_ABILITY_UNHOLY_POWER_SPELL, attackList, false);
    AddMagicCategoryToListPC(frame, subList, textBank.GetTextBySymbol("TEXT_GUI_MENU_ATTACK_SPELLS") + GetXString(NumItems),
                           "PC_RING_PIC_EXP_SPEND_WILL_ATTACK", attackList, L"TEXT_GUI_MENU_ATTACK_SPELLS",
                           "TEXT_GUI_MENUGUIDE_EXPSPEND_MAGIC_COMBAT");
    long willItems = NumItems;
    NumItems = 0;
    CList* surroundList = static_cast<CList*>(manager->CreateComponent("PC_SUB_LIST", false));
    CCountedPointer<CComponent> surroundTitle(manager->CreateComponent("PC_TITLE_EXPERIENCE_SURROUND_SPELLS", false));
    surroundList->AddNonScrollingChild(surroundTitle);
    frame->AddPositionChild(surroundTitle.Get());
    surroundList->InitialiseOffsets(5);
    AddAbilityToListPC(frame, HERO_ABILITY_FORCE_PUSH, surroundList, false);
    AddAbilityToListPC(frame, HERO_ABILITY_TURNCOAT_SPELL, surroundList, false);
    AddAbilityToListPC(frame, HERO_ABILITY_TIME_SPELL, surroundList, false);
    AddAbilityToListPC(frame, HERO_ABILITY_DRAIN_LIFE_SPELL, surroundList, false);
    AddAbilityToListPC(frame, HERO_ABILITY_SUMMON_SPELL, surroundList, false);
    AddMagicCategoryToListPC(frame, subList, textBank.GetTextBySymbol("TEXT_GUI_MENU_SURROUND_SPELLS") + GetXString(NumItems),
                           "PC_RING_PIC_EXP_SPEND_WILL_SURROUND", surroundList, L"TEXT_GUI_MENU_SURROUND_SPELLS",
                           "TEXT_GUI_MENUGUIDE_EXPSPEND_MAGIC_SURROUND");
    willItems += NumItems;
    NumItems = 0;
    CList* physicalList = static_cast<CList*>(manager->CreateComponent("PC_SUB_LIST", false));
    CCountedPointer<CComponent> physicalTitle(manager->CreateComponent("PC_TITLE_EXPERIENCE_PHYSICAL_SPELLS", false));
    physicalList->AddNonScrollingChild(physicalTitle);
    frame->AddPositionChild(physicalTitle.Get());
    physicalList->InitialiseOffsets(6);
    AddAbilityToListPC(frame, HERO_ABILITY_PHYSICAL_SHIELD_SPELL, physicalList, false);
    AddAbilityToListPC(frame, HERO_ABILITY_BERSERK, physicalList, false);
    AddAbilityToListPC(frame, HERO_ABILITY_ASSASSIN_RUSH, physicalList, false);
    AddAbilityToListPC(frame, HERO_ABILITY_HEAL_LIFE_SPELL, physicalList, false);
    AddAbilityToListPC(frame, HERO_ABILITY_GHOST_SWORD_SPELL, physicalList, false);
    AddAbilityToListPC(frame, HERO_ABILITY_MULTI_ARROW, physicalList, false);
    AddMagicCategoryToListPC(frame, subList, textBank.GetTextBySymbol("TEXT_GUI_MENU_PHYSICAL_SPELLS") + GetXString(NumItems),
                           "PC_RING_PIC_EXP_SPEND_WILL_PHYSICAL", physicalList, L"TEXT_GUI_MENU_PHYSICAL_SPELLS",
                           "TEXT_GUI_MENUGUIDE_EXPSPEND_MAGIC_PHYSICAL");
    willItems += NumItems;
    NumItems = 0;
    CCountedPointer<CComponent> magicPowerTitle(manager->CreateComponent("PC_TITLE_EXPERIENCE_WILL", false));
    frame->AddPositionChild(magicPowerTitle.Get());
    AddStatToListPC(frame, HERO_STAT_WILL_MAGIC_POWER, subList, false, &magicPowerTitle);
    willItems += NumItems;
    NumItems = 0;
    name = textBank.GetTextBySymbol("TEXT_GUI_MENU_WILL") + GetXString(willItems);
    entry.Reset(manager->CreateMenuEntryComponent(name, 0.0f, 0.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE",
                                                  0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    AddSoundAction(entry.Get(), "SND_TEXT_QST_TUTORIAL_GUI_EXPSPEND_SUB_WILL");
    back.Reset(new CAction);
    back->ActionType = UI_ACTION_TYPE_CLOSE_MENU;
    entry->AddActionOnBack(back);
    entry->AddChild(subListPointer);
    CCountedPointer<CComponent> willRing(manager->CreateComponent("PC_RING_PIC_EXP_WILL", false));
    entry->AddChild(willRing);
    frame->AddPositionChild(willRing.Get());
    CCountedPointer<CComponent> willDescription(manager->CreateComponent("PC_EXPSPEND_CAT_DESC", false));
    CText* willDescriptionText = static_cast<CText*>(willDescription.Get());
    willDescriptionText->SetValue("TEXT_GUI_MENUGUIDE_EXPSPEND_MAGIC", true, true);
    entry->AddChild(willDescription);
    frame->AddPositionChild(willDescription.Get());
    if (pTextViewport)
        pTextViewport->AddFakeChild(willDescription.Get());
    CCountedPointer<CComponent> willSubtitle(manager->CreateComponent("PC_SUBTITLE", false));
    subtitleText = static_cast<CText*>(willSubtitle.Get());
    subtitleText->SetValue("TEXT_GUI_MENU_WILL", true, true);
    entry->AddChild(willSubtitle);
    frame->AddPositionChild(willSubtitle.Get());
    CCountedPointer<CAction> willFade(new CAction);
    willFade->ActionType = UI_ACTION_TYPE_CHANGE_ALPHA;
    willFade->AddParam(new CActionParamSLONG(0x28));
    willFade->AddParam(ExpSliders[1], willFade);
    willFade->AddParam(ExpSliders[2], willFade);
    entry->AddAction(willFade);
    entry->AddActionOnSelected(highlight);
    list->AddScrollingChildAndSetOffsets(entry);
}