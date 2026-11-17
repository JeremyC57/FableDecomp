// CTCInventoryExperience::OpenInventoryOld() -- opens the experience-spending screen the pre-PC way: unless this is the inventory screen already, remembers the current hero-info screen's type and selects the experience screen; then the base open, the spend values set up and reset, and the UI_EXP_SPENDING_SCREEN component created, initialised, shown and appended to the inventory's components
// Leaf: ?OpenInventoryOld@CTCInventoryExperience@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00574576, length 182 bytes (vtable 0x0124F11C slot 88). Frame-pointer size TU:
//   optimize("y",off) + ("s",on).
// Identity: Ego_r.pdb + FableWin.pdb ?OpenInventoryOld@CTCInventoryExperience@@UAEXXZ (virtual; engine/CTCInventoryExperience.h VIRTUALS
//   declares it for slot 88 = 0x00574576). The landed leaf was the non-virtual QAE spelling.
// Callees: NHeroInformationScreens::CBase::IsInventoryScreen (slot 3 on the +0x130 base), CPlayerGui::GetPHeroInfoScreensManager 0x00437CD0
//   (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass 0x013B8790), NHeroInformationScreens::CManager::GetCurrentScreen 0x0066E7BD, the
//   screen's GetScreenType (slot 2), CManager::SelectScreenOfType(INVENTORY_EXPERIENCE_SCREEN) 0x0066E799, NInventory::CTCInventoryBase::
//   OpenInventory 0x005BD4EA, InitialiseExperienceSpendValues 0x005724CD / ResetExperienceSpendValues 0x0056F03D (Ego_r.pdb + FableWin.pdb),
//   NUISystem::CManager::GetInstance 0x0041E5F2, CreateComponent(const CCharString&, bool) 0x0041DB1D on "UI_EXP_SPENDING_SCREEN",
//   CComponent::Initialise (slot 43) / ChangeState (slot 48), vector<NUISystem::CComponent*>::push_back 0x00486024 on Components.
// Shape: the front-end manager is fetched before the string temporary (a named local) and the created component is a named local that
//   push_back takes by reference; push_back is out of line (FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE: the visible STLport body kept as a
//   COMDAT, so c2 knows the component local does not escape).
// NF79 (round 7, q10959): ported onto the shared headers (the OpenInventoryInternalPC 0x00577AC0 / CloseInventoryInternalPC 0x005778CA
//   macro blocks) plus my overlay opt-in CTCINVENTORYEXPERIENCE_SPENDVALUES; every local model is gone (CCharString, NUISystem::CComponent /
//   CManager, CComponentVector + its same-TU push_back, NHeroInformationScreens::CBase / CManager, CTBaseSingletonNoDefaultAlloc, CPlayerGui,
//   CTCInventoryBase_Retail, CTCInventoryExperience). 0x0066E799 now binds SelectScreenOfType (was the stand-in SetCurrentScreenType(long)).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_VIRTUALS
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_SPENDVALUES
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryExperience.h"
#include "engine/CPlayerGui.h"
#include "engine/NHeroInformationScreens_CManager.h"
#include "engine/NUISystem_CManager.h"

void CTCInventoryExperience::OpenInventoryOld()
{
    if (!IsInventoryScreen()) {
        PreviousScreenType = CPlayerGui::Get().GetPHeroInfoScreensManager()->GetCurrentScreen()->GetScreenType();
        CPlayerGui::Get().GetPHeroInfoScreensManager()->SelectScreenOfType(NHeroInformationScreens::INVENTORY_EXPERIENCE_SCREEN);
    }
    NInventory::CTCInventoryBase::OpenInventory();
    InitialiseExperienceSpendValues();
    ResetExperienceSpendValues();
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    NUISystem::CComponent* screen = manager->CreateComponent("UI_EXP_SPENDING_SCREEN", false);
    screen->Initialise();
    screen->ChangeState(1);
    Components.push_back(screen);
}
