// CTCInventoryExperience::AddStatToListPC(NUISystem::CComponent* screen, EHeroTrainableStatType stat, NUISystem::CList* list, bool dynamically, CCountedPointer<NUISystem::CComponent>* header) -- the PC experience-spending screen's stat entry
// Leaf: ?AddStatToListPC@CTCInventoryExperience@@AAEXPAVCComponent@NUISystem@@W4EHeroTrainableStatType@@PAVCList@3@_NPAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z
// Flags: none
#include "st_head.inc"
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
    ParentThing->GetTC(&heroExperience);
    float levelFraction = 0.0f;
    const long level = heroExperience->GetTrainableStatLevel(stat);
    long maxLevel = heroExperience->GetTrainableStatMaxLevel(stat);
    long possibleLevels = heroExperience->GetTrainableStatPossibleLevelUpAmount(stat, levelFraction);
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
