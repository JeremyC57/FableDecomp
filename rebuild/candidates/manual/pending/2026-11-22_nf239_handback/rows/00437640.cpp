// CPlayerGui::IsDrawingTargetingCentre() const -- whether the projectile targeting-centre drawer is drawing (its inherited
//   NPlayerGui::CDrawBase::Drawing flag)
// Leaf: ?IsDrawingTargetingCentre@CPlayerGui@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00437640, 10 bytes: `mov eax,[ecx+218h] ; mov al,[eax+18h] ; ret`.
// Name (NF239, 2026-11-22, CODEXP27 L20): Ego_r + FableWin publics ?IsDrawingTargetingCentre@CPlayerGui@@QBE_NXZ (Ego_r 0x0044A2E0 /
//   FableWin 0x01A7CD20); the landed caller NPlayerGui::CDrawAbilityButtons::UpdateWeaponAbilities 0x0064B1AC already binds it here.
//   Body: PDrawProjectileTargetingCentre (CCountedPointer, retail +0x218, Ego_r.pdb CPlayerGui member) -> IsDrawing() (CDrawBase
//   Drawing +0x18). Was a free-function stand-in ?CTCShotDefinitionBase_CanBeHitByMeleeStrike@@YI_NPBUCTCShotDefinitionBase@@@Z
//   (catalog _global::GetMember_dword0x218_derefbyte0x18_00437640, propagated label).
// Model: engine/CPlayerGui.h (FABLE_RETAIL_CPLAYERGUI_MEMBERS: the retail member offsets) with the p27 declaration opt-in, and the
//   new engine/NPlayerGui_CDrawProjectileTargetingCentre.h (Ego_d: CDrawBase-derived, no own members); the member-list define set
//   is the landed ~CPlayerGui 0x0043A570's (minus its destructor-only opt-ins).
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDRAWABILITYBUTTONS_WEAPONABILITIES_PASS27
#include "engine/NPlayerGui_CDrawProjectileTargetingCentre.h"
#include "engine/CPlayerGui.h"
bool CPlayerGui::IsDrawingTargetingCentre() const
{
    return PDrawProjectileTargetingCentre->IsDrawing();
}
