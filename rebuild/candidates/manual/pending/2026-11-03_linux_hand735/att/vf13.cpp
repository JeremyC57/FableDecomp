// CTCInventoryExperience::OpenInventoryInternalPC(bool reset) -- build and open the PC experience-spending screen
// Leaf: ?OpenInventoryInternalPC@CTCInventoryExperience@@UAEX_N@Z
// Flags: none
#include "vf_head.inc"
// Same retail TU (CTCInventoryExperience.cpp): the landed GetOriginalExperience 0x0056EA9A body.
long CTCInventoryExperience::GetOriginalExperience(long pool)
{
    if (pool == 0)
        return OrigGeneral;
    else if (pool == 1)
        return OrigStrength;
    else if (pool == 2)
        return OrigSkill;
    else if (pool == 3)
        return OrigWill;
    return 0;
}
void CTCInventoryExperience::OpenInventoryInternalPC(bool reset)
{
    if (!IsInventoryScreen()) {
        PreviousScreenType = CPlayerGui::Get().GetPHeroInfoScreensManager()->GetCurrentScreen()->GetScreenType();
        CPlayerGui::Get().GetPHeroInfoScreensManager()->SelectScreenOfType(NHeroInformationScreens::INVENTORY_EXPERIENCE_SCREEN);
    }
    NInventory::CTCInventoryBase::OpenInventory();
    if (!reset)
        FirstRenderFrame = false;
    CManager* manager = CManager::GetInstance();
    if (reset) {
        UndoableStatBuys.clear();
        UndoableAbilityBuys.clear();
        if (!OpenedExperienceSpending) {
            CCharString sound("SND_TEXT_QST_TUTORIAL_GUI_EXPSPEND_TOPLEVEL");
            TutorialSoundHandles[sound] = manager->PlaySample(sound, 4);
        }
    }
    manager->SetTCHeroExperienceInventory(GetTCHeroExperience());
    manager->SetAbilitiesInventory(GetTCInventoryAbilities());
    manager->SetExperienceInventory(this);
    CWideString title = CWideString::CreateFromCharString("");
    CList* screen = static_cast<CList*>(manager->CreateComponent("PC_EXPERIENCE_SPENDING_MENU", false));
    CComponent* frame = screen->FindChild("PC_UI_FRAME");
    CComponent* clickable = screen->FindChild("SCROLL_DESCRIPTION_UP");
    if (clickable) {
        manager->AddObserver(clickable);
        clickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    clickable = screen->FindChild("SCROLL_DESCRIPTION_DOWN");
    if (clickable) {
        manager->AddObserver(clickable);
        clickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    clickable = screen->FindChild("PC_BACK_CLICKABLE");
    if (clickable) {
        manager->AddObserver(clickable);
        clickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    clickable = screen->FindChild("PC_CLOSE_CLICKABLE");
    if (clickable) {
        manager->AddObserver(clickable);
        clickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    clickable = screen->FindChild("SCROLL_LIST_UP");
    if (clickable) {
        manager->AddObserver(clickable);
        clickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    clickable = screen->FindChild("SCROLL_LIST_DOWN");
    if (clickable) {
        manager->AddObserver(clickable);
        clickable->ObserveEvent(UI_EVENT_MOUSE_MOVEMENT);
    }
    CComponent* titleText = screen->FindChild("PC_TITLE_EXPERIENCE_SPENDING");
    if (titleText)
        frame->AddPositionChild(titleText);
    pViewport = static_cast<CViewport*>(screen->FindChild("PC_EXPERIENCE_SPENDING_MOVIE_VIEWPORT"));
    pTextViewport = static_cast<CViewport*>(screen->FindChild("PC_DESCRIPTION_VIEWPORT"));
    if (pViewport)
        frame->AddPositionChild(pViewport);
    if (pTextViewport)
        frame->AddPositionChild(pTextViewport);
    CTCInventoryStats* stats;
    if (ParentThing->GetTC(&stats)) {
        CCountedPointer<CComponent> sliders(manager->CreateComponent("PC_EXP_SPENDING_SLIDER_CONTAINER", false));
        frame->AddPositionChild(sliders.Get());
        ExpSliders = stats->CreateExperienceSliders(sliders.Get(), 2, false);
        screen->AddNonScrollingChild(sliders);
        AnimIt = Animators.end();
        CTCHeroExperience* heroExperience;
        if (!reset && ParentThing->GetTC(&heroExperience)) {
            float remaining = (float)(PointsGeneral + PointsStrength + (PointsSkill + PointsWill));
            remaining -= (float)(heroExperience->GetExperiencePointsAvailableToSpend() + heroExperience->GetStatExperience(HERO_STAT_STRENGTH)
                                 + heroExperience->GetStatExperience(HERO_STAT_SKILL) + heroExperience->GetStatExperience(HERO_STAT_WILL));
            CSliderAnimator general(ExpSliders[0], (float)PointsGeneral, (float)heroExperience->GetExperiencePointsAvailableToSpend(),
                                    (float)OrigGeneral, remaining);
            CSliderAnimator strength(ExpSliders[1], (float)PointsStrength, (float)heroExperience->GetStatExperience(HERO_STAT_STRENGTH),
                                     (float)GetOriginalExperience(1), remaining);
            CSliderAnimator skill(ExpSliders[2], (float)PointsSkill, (float)heroExperience->GetStatExperience(HERO_STAT_SKILL),
                                  (float)GetOriginalExperience(2), remaining);
            CSliderAnimator will(ExpSliders[3], (float)PointsWill, (float)heroExperience->GetStatExperience(HERO_STAT_WILL),
                                 (float)GetOriginalExperience(3), remaining);
            Animators.clear();
            if (CreateBecauseOfUndo) {
                Animators.push_back(general);
                Animators.push_back(strength);
                Animators.push_back(skill);
                Animators.push_back(will);
            } else {
                Animators.push_front(general);
                Animators.push_front(strength);
                Animators.push_front(skill);
                Animators.push_front(will);
            }
            CreateBecauseOfUndo = false;
            AnimIt = Animators.begin();
            (*AnimIt).Play = true;
        }
    }
    screen->InitialiseOffsets(3);
    ConstructCategoryStrengthPC(screen);
    screen->Initialise();
    if (reset) {
        screen->ChangeState(5);
        {
            CCharString pointerName("UI_MOUSE_POINTER");
            CComponent* pointer = CManager::GetInstance()->CreateComponent(pointerName, false);
            Components.push_back(pointer);
        }
        Components[0]->Initialise();
        PMousePointer = Components[0];
    }
    Components.push_back(screen);
    CComponent* dialog = manager->CreateComponent("PC_DIALOG_YESNO_GAME", false);
    CComponent* yes = dialog->FindChild("PC_YESNO_GAME_YESBUTTON");
    if (yes) {
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_SEND_EVENT;
        action->UIActionParams.push_front(new CActionParamSLONG(4));
        yes->AddActionOnLeftUnclicked(action);
    }
    CComponent* no = dialog->FindChild("PC_YESNO_GAME_NOBUTTON");
    if (no) {
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_SEND_EVENT;
        action->UIActionParams.push_front(new CActionParamSLONG(5));
        no->AddActionOnLeftUnclicked(action);
    }
    dialog->Initialise();
    C2DVector position(CUIScreenUnitConverter::GetBackBufferWidth() * 0.5f - CManager::GetInstance()->GetUIScale().X * 104.0f,
                       CUIScreenUnitConverter::GetBackBufferHeight() * 0.5f - CManager::GetInstance()->GetUIScale().Y * 52.0f);
    dialog->ChangePosition(position, 0.0f, false);
    Components.push_back(dialog);
    pYesNo = static_cast<CYesNo*>(dialog);
}
