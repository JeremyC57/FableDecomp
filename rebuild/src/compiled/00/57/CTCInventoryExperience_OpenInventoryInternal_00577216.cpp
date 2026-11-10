// CTCInventoryExperience::OpenInventoryInternal(bool reset) -- build and open the console experience-spending screen
// Leaf: ?OpenInventoryInternal@CTCInventoryExperience@@UAEX_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00577216, 1268 bytes, thiscall, `ret 4`; size TU (biased `lea ebp,[esp-74h]` frame).
//   Identity: CTCInventoryExperience vtable 0x0124F11C slot 86 (catalog "vfunc_86"): the class's five Ego_r.pdb virtuals that the
//   base lacks (UAE: OpenInventoryInternal, OpenInventoryInternalPC, OpenInventoryOld, CloseInventoryInternal, CloseInventoryInternalPC)
//   fill slots 86..90 in that order -- 87 is OpenInventoryInternalPC 0x00577AC0 and 88 OpenInventoryOld 0x00574576 (landed), 90 keeps
//   the PC mouse pointer (CloseInventoryInternalPC) -- and this body is the console twin of 0x00577AC0: no PC frame / scroll
//   clickables / mouse pointer, the UI_ (not PC_) templates, and ConstructCategoryStrength 0x00574885 instead of its PC twin.
// Shape: as OpenInventoryInternalPC (prior chunk 735 model): the hero-info screen switch, the base OpenInventory 0x005BD4EA, the
//   tutorial sound on a fresh open, the three inventory hand-offs to the UI manager, the menu component, its viewports, the
//   experience-slider container (CTCInventoryStats::CreateExperienceSliders 0x005CDD86, the vector assigned with the out-of-line
//   operator= 0x005798C8), the four count-down animators when re-opened after a purchase, the strength category list, and the
//   yes/no dialog kept in pYesNo.
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
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_VIRTUALS
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_PRIVATE
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
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
void CTCInventoryExperience::OpenInventoryInternal(bool reset)
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
    CList* screen = static_cast<CList*>(manager->CreateComponent("UI_EXPERIENCE_SPENDING_MENU", false));
    pViewport = static_cast<CViewport*>(screen->FindChild("UI_MINI_MAP_VIEWPORT_EXP_SPENDING"));
    pTextViewport = static_cast<CViewport*>(screen->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA"));
    CTCInventoryStats* stats;
    if (ParentThing->GetTC(&stats)) {
        CCountedPointer<CComponent> sliders(manager->CreateComponent("UI_EXP_SPENDING_SLIDER_CONTAINER", false));
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
    ConstructCategoryStrength(screen);
    screen->Initialise();
    if (reset)
        screen->ChangeState(5);
    Components.push_back(screen);
    CComponent* dialog = manager->CreateComponent("UI_DIALOG_YESNO_GAME", false);
    dialog->Initialise();
    Components.push_back(dialog);
    pYesNo = static_cast<CYesNo*>(dialog);
}