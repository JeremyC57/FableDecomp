// CTCInventoryExperience::AddAbilityToList(EHeroAbility ability, NUISystem::CList* list, bool dynamically) -- add one hero ability's branch entry to the experience-spending list
// Leaf: ?AddAbilityToList@CTCInventoryExperience@@AAEXW4EHeroAbility@@PAVCList@NUISystem@@_N@Z
// Flags: none
#include "ab_head.inc"
void CTCInventoryExperience::AddAbilityToList(EHeroAbility ability, CList* list, bool dynamically)
{
    const NGameText::CDataBank& textBank = CMainGameComponent::Get()->GetTextBank();
    CTCHeroStats* heroStats;
    ParentThing->GetTC(&heroStats);
    CManager* manager = CManager::GetInstance();
    CWideString name = GetExperienceAbilityName(ability);
    CTCHeroExperience* heroExperience;
    ParentThing->GetTC(&heroExperience);
    CTCInventoryAbilities* abilities;
    ParentThing->GetTC(&abilities);
    long nextLevel = abilities->GetAbilityLevel(ability) + 2;
    long level = abilities->GetAbilityLevel(ability);
    if (level < abilities->GetAbilityMaximumLevel(ability)) {
        name += CWideString::CreateFromCharString(" +");
        name += GFIntToWideString(nextLevel);
    }
    CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(name, 0.0f, 0.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",
                                                                        0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    CCountedPointer<CComponent> movie(manager->CreateComponent("UI_MOVIE", false));
    CMovie* moviePtr = static_cast<CMovie*>(movie.Get());
    CWideString movieName = GetExperienceAbilityMovieName(ability);
    moviePtr->SetMovie(movieName);
    entry->AddChild(movie);
    if (pViewport)
        pViewport->AddFakeChild(movie.Get());
    CCountedPointer<CComponent> statDescription(manager->CreateComponent("UI_STAT_DESCRIPTION", false));
    entry->AddChild(statDescription);
    if (pTextViewport)
        pTextViewport->AddFakeChild(statDescription.Get());
    CCountedPointer<CComponentContainer> container;
    container.SetStaticCast(statDescription);
    long descriptionLevel = abilities->GetAbilityLevel(ability);
    if (descriptionLevel < abilities->GetAbilityMaximumLevel(ability)) {
        CWideString space(L" ");
        long cost = abilities->GetExperienceCostToUpgradeAbility(ability, 1, true);
        CWideString text = GetExperienceAbilityName(ability) + space + textBank.GetTextBySymbol("TEXT_GUI_MENU_EXP_LEVEL") + space
                         + GFIntToWideString(nextLevel) + CWideString(L"\n");
        text += CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_EXPERIENCE_COST") + CWideString(L": ")
              + GFIntToWideString(cost);
        container->AddTextLine(text, GetExperienceAbilityColour(ability).GetRGBColour(), abilities->GetAbilityGraphicIndex(ability), 210);
        long modifier = cost - abilities->GetExperienceCostToUpgradeAbility(ability, 1, false);
        if (modifier != 0) {
            CWideString modifierText = GFIntToWideString(modifier);
            if (modifier > 0)
                modifierText = CWideString(L"+") + modifierText;
            modifierText = textBank.GetTextBySymbol("TEXT_GUI_MENU_ALIGNMENT_MODIFIER") + space + modifierText;
            heroStats->GetMoralityPercentage();
            container->AddTextLine(modifierText, CRGBColour(255, 255, 255, 255), "", 240);
        }
        container->AddTextLine(CWideString(L""), CRGBColour(0, 0, 0, 255), "", 240);
    }
    container->AddTextLine(GetExperienceAbilityDescription(ability, nextLevel - 1), CRGBColour(255, 255, 255, 255), "", 240);
    float levelFraction = 0.0f;
    long possibleLevels = heroExperience->GetAbilityPossibleLevelUpAmount(ability, levelFraction);
    levelFraction += abilities->GetAbilityLevel(ability) + possibleLevels + 1;
    if (levelFraction < 0.0f)
        levelFraction = 0.0f;
    CRGBFloatColour moralityColour;
    float moralityFactor = abilities->GetAbilityMoralityCostFactor(ability);
    bool showMorality = true;
    if (moralityFactor < -DELTA_NOTIONAL_ZERO)
        moralityColour = CRGBFloatColour(1.0f, 0.0f, 0.0f, 0.4f);
    else if (moralityFactor > DELTA_NOTIONAL_ZERO)
        moralityColour = CRGBFloatColour(0.0f, 0.0f, 1.0f, 0.4f);
    else
        showMorality = false;
    CCountedPointer<CComponent> tickContainer(manager->CreateComponent("UI_EXPERIENCE_SPENDING_TICK_CONTAINER", false));
    CCountedPointer<CComponent> slider(manager->CreateTickSlider(GetExperienceAbilityName(ability), C2DCoordI(0, 0),
                                                                 abilities->GetAbilityLevel(ability) + 1,
                                                                 abilities->GetAbilityMaximumLevel(ability) + 1,
                                                                 GetExperienceAbilityColour(ability), levelFraction, 310.0f, false,
                                                                 showMorality, moralityColour, 50.0f));
    tickContainer->AddChild(slider);
    entry->AddChild(tickContainer);
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
    std::map<EHeroAbility, SExpSpendUndo>::iterator undo = UndoableAbilityBuys.find(ability);
    if (possibleLevels <= 0 && undo == UndoableAbilityBuys.end()) {
        CComponent* arrow = entry->FindChild("UI_SELECTION_ARROW_RIGHT");
        if (arrow) {
            CDeletion deletion;
            deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
            arrow->SetDeletion(deletion);
        }
        entry->MakeUnavailable();
    } else {
        CCountedPointer<CComponent> subComponent(manager->CreateComponent("UI_SUB_LIST", false));
        CCountedPointer<CList> subList;
        unsigned long offsets = 1;
        if (possibleLevels > 0 && undo != UndoableAbilityBuys.end())
            offsets = 2;
        subList.SetStaticCast(subComponent);
        subList->InitialiseOffsets(offsets);
        entry->AddChild(subComponent);
        if (possibleLevels > 0) {
            ++NumItems;
            CWideString buyText = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_BUY");
            CCountedPointer<CComponent> buy(manager->CreateMenuEntryComponent(buyText, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW",
                                                                              0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
            CCountedPointer<CComponent> buyButton;
            buyButton = buy;
            AddSoundAction(buy.Get(), "SND_TEXT_QST_TUTORIAL_GUI_EXPSPEND_BUY_WILL");
            CCountedPointer<CAction> buyAction(new CAction);
            buyAction->ActionType = UI_ACTION_TYPE_BUY_ABILITY;
            buyAction->AddParam(new CActionParamSLONG(ability));
            buyAction->AddParam(new CActionParamSLONG(GetTCInventoryAbilities()->GetExperienceCostToUpgradeAbility(ability, 1, true)));
            buyButton->AddAction(buyAction);
            subList->AddScrollingChildAndSetOffsets(buy);
            long cost = GetTCInventoryAbilities()->GetExperienceCostToUpgradeAbility(ability, 1, true);
            long willExperience = GetTCHeroExperience()->GetStatExperience(HERO_STAT_WILL);
            CCountedPointer<CAction> spend(new CAction);
            spend->ActionType = UI_ACTION_TYPE_CHANGE_NUMBER;
            if (willExperience > 0) {
                spend->AddParam(ExpSliders[3]->FindChild("UI_LEVEL_SLIDER_NUMBER"), spend);
                if (willExperience <= cost)
                    spend->AddParam(new CActionParamSLONG(willExperience));
                else
                    spend->AddParam(new CActionParamSLONG(cost));
            }
            cost -= willExperience;
            if (cost > 0) {
                spend->AddParam(ExpSliders[0]->FindChild("UI_LEVEL_SLIDER_NUMBER"), spend);
                spend->AddParam(new CActionParamSLONG(cost));
            }
            entry->AddActionOnSelected(spend);
        }
        if (undo != UndoableAbilityBuys.end()) {
            CWideString undoText = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_UNDO_LEVEL_UP");
            CCountedPointer<CComponent> undoEntry(manager->CreateMenuEntryComponent(undoText, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW",
                                                                                    0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
            subList->AddScrollingChildAndSetOffsets(undoEntry);
            CCountedPointer<CAction> undoAction(new CAction);
            undoAction->ActionType = UI_ACTION_TYPE_UNDO_BUY_ABILITY;
            undoAction->AddParam(new CActionParamSLONG(ability));
            undoEntry->AddAction(undoAction);
        }
    }
    if (dynamically)
        list->AddScrollingChildDynamicallyAndSetOffsets(entry);
    else
        list->AddScrollingChildAndSetOffsets(entry);
}
