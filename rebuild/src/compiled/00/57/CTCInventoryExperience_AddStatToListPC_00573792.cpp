// CTCInventoryExperience::AddStatToListPC(NUISystem::CComponent* screen, EHeroTrainableStatType stat, NUISystem::CList* list, bool dynamically, CCountedPointer<NUISystem::CComponent>* header) -- the PC experience-spending screen's stat entry
// Leaf: ?AddStatToListPC@CTCInventoryExperience@@AAEXPAVCComponent@NUISystem@@W4EHeroTrainableStatType@@PAVCList@3@_NPAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00573792, 3556 bytes, thiscall, `ret 0x14`. Identity: Ego_r.pdb
//   ?AddStatToListPC@CTCInventoryExperience@@AAEXPAVCComponent@NUISystem@@W4EHeroTrainableStatType@@PAVCList@3@_NPAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z
//   (private); the PC twin of AddStatToList 0x005729D8 (PC_ templates, and the screen registers the movie, the stat text and
//   the tick container through its virtual AddPositionChild, slot 124 +0x1F0).
// Callees: CCharString(const char*, long) 0x0099EBF0 / (const CCharString&) 0x0099EC30 / operator+=(const char*) 0x0099F100 /
//   ~ 0x0099EAE0 / ToWideString 0x0099E480, CWideString(const wchar_t*) 0x0099B6B0 / () 0x0099AED0 / operator+=(const CWideString&)
//   0x0099B8D0 / operator= 0x0099B7D0 / ~ 0x0099B510 / CreateFromCharString 0x0099B6A0, operator+(const CWideString&, const
//   CWideString&) 0x0099BE70, GFIntToWideString 0x00405F00, NGameText::CDataBank::GetTextBySymbol 0x009C95E0 (through
//   CMainGameComponent::PMainGameComponent 0x013B86A0 +0x14), NUISystem::CManager::GetInstance 0x0041E5F2 / CreateMenuEntryComponent
//   0x00428494 / CreateComponent 0x0041DB1D / CreateTickSlider 0x00427216, the CTCInventoryExperience members GetExperienceStatName
//   0x0056F26F / GetExperienceStatMovieName 0x0056E6B6 / GetExperienceStatDescription 0x0056FDC3 / GetExperienceStatColour 0x0056ED74
//   / AddSoundAction 0x0056F615 / GetTCHeroExperience 0x0056FF3D, CTCHeroExperience GetTrainableStatLevel 0x006D6520 /
//   GetTrainableStatMaxLevel 0x006D6770 / GetTrainableStatPossibleLevelUpAmount 0x006D6FA0 / GetCostToUpgradeTrainableStat
//   0x006D67B0 / GetCategoryTrainableStatIsIn 0x006D6850 / GetStatExperience 0x006D6510, std::bitset<274>::test 0x00410DE0 and
//   CVectorMap::LowerBound 0x0040F020 (the inline CThing::GetTC), the STLport map instances _M_find 0x00579114 (UndoableStatBuys)
//   and insert_unique 0x00579958 (UpgradableStats), CCountedPointer<NUISystem::CComponent>(CComponent*) 0x00429C15 / ShareData
//   0x0042920A / ~ 0x004291DE, CCountedPointer<NUISystem::CText>::ShareData 0x00429460 / ~ 0x0042925C,
//   CCountedPointer<NUISystem::CList>::ShareData 0x00429486 / ~ 0x00429288, CCountedPointer<NUISystem::CAction>(CAction*)
//   0x0042AA29 / ~ 0x00429160, operator new 0x00BFEA1A, NUISystem::CAction() 0x0042BE50 / AddParam(CActionParam*) 0x0053FDB0 /
//   AddParam(CActionParam*, CCountedPointer<CAction>&) 0x0053FDE0, the inline CActionParamSLONG(long) (vtable 0x012366F4),
//   NUISystem::CDeletion() 0x0042BED4 / (const CDeletion&) 0x0042CD84 and its AssociatedParents list dtor 0x0042ABCA; virtual
//   calls: CComponent FindChild (slot 3, +0xC), AddChild (59, +0xEC), SetDeletion (62, +0xF8), AddAction (66, +0x108),
//   AddActionOnSelected (67, +0x10C), AddActionOnBack (68, +0x110), MakeUnavailable (92, +0x170), AddPositionChild (124, +0x1F0), CMovie::SetMovie (142,
//   +0x238), CText::SetValue(const CWideString&, bool, bool) (144, +0x240), CViewport::AddFakeChild (142, +0x238), CList
//   AddScrollingChildAndSetOffsets (+0x260) / AddScrollingChildDynamicallyAndSetOffsets (+0x264) / AddNonScrollingChild (+0x244) /
//   InitialiseOffsets (+0x270).
// Shape: size TU (`optimize("s", on)` alone: the biased `lea ebp,[esp-64h]` frame). The hero-experience component is looked up
//   twice with the inline CThing::GetTC (two locals sharing one frame word), elsewhere through the out-of-line GetTCHeroExperience;
//   the text bank is a reference local only for the level line. The movie pointer is fetched into a local before its name is
//   built; the sub-list offset count starts at 1 and becomes 2 when both a buy and an undo entry follow; the per-category spend
//   is `categoryExperience <= cost ? categoryExperience : cost` written as an if/else of two AddParam calls (VC7.1 merges the
//   calls and keeps two `new`s); the general-pool remainder reuses `cost`. GetExperienceStatColour's own body (landed
//   0x0056ED74, the same retail TU CTCInventoryExperience.cpp) is compiled above the row: VC7.1 then knows the call leaves ecx
//   untouched and passes `this` to the following GetExperienceStatName without reloading it (retail has no `mov ecx,esi`
//   between the two calls; without the body the row differs by that one instruction).
// Layout: engine/CTCInventoryExperience.h retail branch (UndoableStatBuys +0x140, pViewport +0x1D8, pTextViewport +0x1DC,
//   NumItems +0x218, UpgradableStats +0x224, ExpSliders +0x23C); CTCBase::ParentThing +0x04; CThing UsingComponents +0x20 /
//   Components +0x44.
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
CWideString __fastcall GFIntToWideString(long value);                                // 0x00405F00
using namespace NUISystem;
CRGBFloatColour CTCInventoryExperience::GetExperienceStatColour(EHeroTrainableStatType stat)
{
    if (stat >= 0 && stat <= 2)
        return CRGBFloatColour(1.0f, 0.3f, 0.3f, 1.0f);
    if (stat >= 3 && stat <= 5)
        return CRGBFloatColour(1.0f, 1.0f, 0.3f, 1.0f);
    return CRGBFloatColour(0.3f, 0.3f, 1.0f, 1.0f);
}
void CTCInventoryExperience::AddStatToListPC(CComponent* screen, EHeroTrainableStatType stat, CList* list, bool dynamically, CCountedPointer<CComponent>* header)
{
    CCharString soundName("SND_TEXT_QST_TUTORIAL_GUI_EXPSPEND_BUY_");
    if (stat == 2) soundName += "TOUGHNESS";
    if (stat == 0) soundName += "PHYSIQUE";
    if (stat == 1) soundName += "HEALTH";
    if (stat == 3) soundName += "SPEED";
    if (stat == 4) soundName += "ACCURACY";
    if (stat == 5) soundName += "GUILE";
    if (stat == 9) soundName += "MAGICPOWER";
    const NGameText::CDataBank& textBank = CMainGameComponent::Get()->GetTextBank();
    CManager* manager = CManager::GetInstance();
    CWideString name = GetExperienceStatName(stat);
    CTCHeroExperience* heroExperience;
    ParentThing->GetTC(&heroExperience);
    if (heroExperience->GetTrainableStatLevel(stat) < heroExperience->GetTrainableStatMaxLevel(stat)) {
        name += CWideString::CreateFromCharString(" +");
        name += GFIntToWideString(GetTCHeroExperience()->GetTrainableStatLevel(stat) + 1);
    }
    CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(name, 0.0f, 0.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE",
                                                                        0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    AddSoundAction(entry.Get(), soundName);
    UpgradableStats.insert(std::pair<const EHeroTrainableStatType, CComponent*>(stat, entry.Get()));
    CCountedPointer<CComponent> movie(manager->CreateComponent("PC_MOVIE", false));
    CMovie* moviePtr = static_cast<CMovie*>(movie.Get());
    CWideString movieName = GetExperienceStatMovieName(stat).ToWideString();
    moviePtr->SetMovie(movieName);
    entry->AddChild(movie);
    if (pViewport)
        pViewport->AddFakeChild(movie.Get());
    screen->AddPositionChild(movie.Get());
    CCountedPointer<CComponent> statText(manager->CreateComponent("PC_TEXT_EXPERIENCE_STAT_NAME", false));
    CCountedPointer<CText> text;
    text.SetStaticCast(statText);
    CWideString space(L" ");
    CWideString description;
    if (heroExperience->GetTrainableStatLevel(stat) < heroExperience->GetTrainableStatMaxLevel(stat)) {
        description = GetExperienceStatName(stat) + space + textBank.GetTextBySymbol("TEXT_GUI_MENU_EXP_LEVEL") + space
                    + GFIntToWideString(GetTCHeroExperience()->GetTrainableStatLevel(stat) + 1) + CWideString(L"\n");
        description += CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_EXPERIENCE_COST") + CWideString(L": ")
                     + GFIntToWideString(GetTCHeroExperience()->GetCostToUpgradeTrainableStat(stat));
        description += CWideString(L"\n\n");
    }
    description += GetExperienceStatDescription(stat, heroExperience->GetTrainableStatLevel(stat) + 1);
    text->SetValue(description, true, true);
    entry->AddChild(statText);
    screen->AddPositionChild(statText.Get());
    if (pTextViewport)
        pTextViewport->AddFakeChild(statText.Get());
    CTCHeroExperience* tickExperience;
    ParentThing->GetTC(&tickExperience);
    float levelFraction = 0.0f;
    long level = tickExperience->GetTrainableStatLevel(stat);
    long maxLevel = tickExperience->GetTrainableStatMaxLevel(stat);
    long possibleLevels = tickExperience->GetTrainableStatPossibleLevelUpAmount(stat, levelFraction);
    levelFraction += possibleLevels + level;
    CCountedPointer<CComponent> tickContainer(manager->CreateComponent("PC_EXPERIENCE_SPENDING_TICK_CONTAINER", false));
    CCountedPointer<CComponent> slider(manager->CreateTickSlider(GetExperienceStatName(stat), C2DCoordI(0, 0), level, maxLevel,
                                                                 GetExperienceStatColour(stat), levelFraction, 310.0f, false, false,
                                                                 CRGBFloatColour(0.0f, 0.0f, 0.0f, 0.0f), 40.0f));
    tickContainer->AddChild(slider);
    entry->AddChild(tickContainer);
    screen->AddPositionChild(tickContainer.Get());
    std::map<EHeroTrainableStatType, SExpSpendUndo>::iterator undo = UndoableStatBuys.find(stat);
    if (possibleLevels <= 0 && undo == UndoableStatBuys.end()) {
        CComponent* arrow = entry->FindChild("UI_SELECTION_ARROW_RIGHT");
        if (arrow) {
            CDeletion deletion;
            deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
            arrow->SetDeletion(deletion);
        }
        entry->MakeUnavailable();
    } else {
        CCountedPointer<CComponent> subComponent(manager->CreateComponent("PC_SUB_LIST", false));
        CCountedPointer<CList> subList;
        subList.SetStaticCast(subComponent);
        if (header)
            subList->AddNonScrollingChild(*header);
        unsigned long offsets = 1;
        if (possibleLevels > 0 && undo != UndoableStatBuys.end())
            offsets = 2;
        subList->InitialiseOffsets(offsets);
        entry->AddChild(subComponent);
        CCountedPointer<CAction> changeNumbers(new CAction);
        changeNumbers->ActionType = UI_ACTION_TYPE_CHANGE_NUMBER;
        changeNumbers->AddParam(ExpSliders[0]->FindChild("UI_LEVEL_SLIDER_NUMBER"), changeNumbers);
        changeNumbers->AddParam(new CActionParamSLONG(-1));
        changeNumbers->AddParam(ExpSliders[1]->FindChild("UI_LEVEL_SLIDER_NUMBER"), changeNumbers);
        changeNumbers->AddParam(new CActionParamSLONG(-1));
        changeNumbers->AddParam(ExpSliders[2]->FindChild("UI_LEVEL_SLIDER_NUMBER"), changeNumbers);
        changeNumbers->AddParam(new CActionParamSLONG(-1));
        changeNumbers->AddParam(ExpSliders[3]->FindChild("UI_LEVEL_SLIDER_NUMBER"), changeNumbers);
        changeNumbers->AddParam(new CActionParamSLONG(-1));
        entry->AddActionOnSelected(changeNumbers);
        entry->AddActionOnBack(changeNumbers);
        if (possibleLevels > 0) {
            ++NumItems;
            CWideString buyText = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_BUY");
            CCountedPointer<CComponent> buy(manager->CreateMenuEntryComponent(buyText, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                                              0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
            CCountedPointer<CComponent> buyButton;
            buyButton = buy;
            CCountedPointer<CAction> buyAction(new CAction);
            buyAction->ActionType = UI_ACTION_TYPE_BUY_STAT;
            buyAction->AddParam(new CActionParamSLONG(stat));
            buyAction->AddParam(new CActionParamSLONG(GetTCHeroExperience()->GetCostToUpgradeTrainableStat(stat)));
            buyButton->AddAction(buyAction);
            subList->AddScrollingChildAndSetOffsets(buy);
            long cost = GetTCHeroExperience()->GetCostToUpgradeTrainableStat(stat);
            EHeroExperienceStatCategory category = GetTCHeroExperience()->GetCategoryTrainableStatIsIn(stat);
            long categoryExperience = GetTCHeroExperience()->GetStatExperience(category);
            CCountedPointer<CAction> spend(new CAction);
            spend->ActionType = UI_ACTION_TYPE_CHANGE_NUMBER;
            if (categoryExperience > 0) {
                spend->AddParam(ExpSliders[category + 1]->FindChild("UI_LEVEL_SLIDER_NUMBER"), spend);
                if (categoryExperience <= cost)
                    spend->AddParam(new CActionParamSLONG(categoryExperience));
                else
                    spend->AddParam(new CActionParamSLONG(cost));
            }
            cost -= categoryExperience;
            if (cost > 0) {
                spend->AddParam(ExpSliders[0]->FindChild("UI_LEVEL_SLIDER_NUMBER"), spend);
                spend->AddParam(new CActionParamSLONG(cost));
            }
            entry->AddActionOnSelected(spend);
        }
        if (undo != UndoableStatBuys.end()) {
            CWideString undoText = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_UNDO_LEVEL_UP");
            CCountedPointer<CComponent> undoEntry(manager->CreateMenuEntryComponent(undoText, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                                                    0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
            subList->AddScrollingChildAndSetOffsets(undoEntry);
            CCountedPointer<CAction> undoAction(new CAction);
            undoAction->ActionType = UI_ACTION_TYPE_UNDO_BUY_STAT;
            undoAction->AddParam(new CActionParamSLONG(stat));
            undoEntry->AddAction(undoAction);
        }
    }
    if (dynamically)
        list->AddScrollingChildDynamicallyAndSetOffsets(entry);
    else
        list->AddScrollingChildAndSetOffsets(entry);
}