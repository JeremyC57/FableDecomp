// CTCInventoryExperience::OpenInventoryInternalPC(bool reset) -- build and open the PC experience-spending screen
// Grade: DIFFER(2157v2157) -- 5 instructions: the integer sum PointsGeneral+Strength+Skill+Will (+0x260..+0x26C) is emitted in VC7.1's canonical operand order (+0x264, +0x260, +0x268, +0x26C) where retail has +0x268, +0x26C, +0x264, +0x260; invariant under 14 source spellings (all orders, groupings, casts, this->, declaration position, a same-TU SaveExperienceValuesForCountDown body); the dialog x-coordinate PAREN lever (chunk 1005) fixed the other residue
// Leaf: ?OpenInventoryInternalPC@CTCInventoryExperience@@UAEX_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00577AC0, 2157 bytes, thiscall, `ret 4`. Identity: CTCInventoryExperience vtable slot
//   "vfunc_87" = Ego_r.pdb ?OpenInventoryInternalPC@CTCInventoryExperience@@UAEX_N@Z (public virtual; the name every caller and
//   the PC screen flow use). Callees: IsInventoryScreen via the NHeroInformationScreens::CBase subobject (+0x130, slot 3),
//   CPlayerGui::GetPHeroInfoScreensManager 0x00437CD0, NHeroInformationScreens::CManager::GetCurrentScreen 0x0066E7BD /
//   SelectScreenOfType 0x0066E799, NInventory::CTCInventoryBase::OpenInventory 0x005BD4EA, NUISystem::CManager GetInstance
//   0x0041E5F2 / CreateComponent 0x0041DB1D / GetUIScale 0x0041CF47 and virtuals AddObserver (+8), SetTCHeroExperienceInventory
//   (+0x48), SetAbilitiesInventory (+0x4C), SetExperienceInventory (+0x50), PlaySample (+0xA4); map<CCharString,unsigned
//   long>::operator[] 0x0042D131, the two map clear()s 0x00579EA3 / 0x00579ECC, CTCInventoryStats::CreateExperienceSliders
//   0x005CDD86, vector<CComponent*>::operator= 0x005798C8 / push_back 0x00486024, CSliderAnimator(...) 0x0056E545,
//   list<CSliderAnimator> clear 0x00578FCE / push_back 0x0057955A / push_front 0x0057953E, CTCHeroExperience::
//   GetStatExperience 0x006D6510, ConstructCategoryStrengthPC 0x005759BF, CUIScreenUnitConverter::GetBackBufferWidth 0x0041CC14
//   / GetBackBufferHeight 0x0041CC2B, CAction() 0x0042BE50, list<CActionParam*>::push_back 0x0042B646 (inserts before the sentinel).
// Shape: GetOriginalExperience (landed 0x0056EA9A, same TU) is compiled above and VC7.1 inlines it for the strength / skill /
//   will animators (the `mov eax,[esi+1B0h] ; mov [ebp+58h],eax ; fild [ebp+58h]` register round trip; OrigGeneral is read
//   directly); `screen` is a CList* (push_back(screen) then binds a converted CComponent* temporary); the dialog position is
//   a named C2DVector built from Width/2 - scale.X*104 and Height/2 - scale.Y*52 (y first).
// Residue (11 lines): (1) the integer sum PointsWill/Skill/Strength/General: retail adds +0x268, +0x26C, +0x264, +0x260, VC7.1
//   reassociates every source order tried to +0x264, +0x260, +0x268, +0x26C (a named `long` total gives +0x268, +0x264,
//   +0x260, +0x26C); (2) `lea ecx,[ebp+40h]` for the position is scheduled one instruction before retail's `fsubp`.
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
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYSTATS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_CYESNO
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
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
#include "engine/CTCInventoryStats.h"
#include "engine/CPlayerGui.h"
#include "engine/NHeroInformationScreens_CManager.h"
#include "engine/NUISystem_CYesNo.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#include "engine/C2DVector.h"
#include "fable_game_text_bank.h"
#include "fable_game.h"
CWideString __fastcall GFIntToWideString(long value);                                // 0x00405F00
using namespace NUISystem;
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
            float remaining = (float)(PointsGeneral + PointsStrength + PointsSkill + PointsWill);
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
        action->UIActionParams.push_back(new CActionParamSLONG(4));
        yes->AddActionOnLeftUnclicked(action);
    }
    CComponent* no = dialog->FindChild("PC_YESNO_GAME_NOBUTTON");
    if (no) {
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_SEND_EVENT;
        action->UIActionParams.push_back(new CActionParamSLONG(5));
        no->AddActionOnLeftUnclicked(action);
    }
    dialog->Initialise();
    C2DVector position((CUIScreenUnitConverter::GetBackBufferWidth() * 0.5f) - (CManager::GetInstance()->GetUIScale().X * 104.0f),
                       CUIScreenUnitConverter::GetBackBufferHeight() * 0.5f - CManager::GetInstance()->GetUIScale().Y * 52.0f);
    dialog->ChangePosition(position, 0.0f, false);
    Components.push_back(dialog);
    pYesNo = static_cast<CYesNo*>(dialog);
}
