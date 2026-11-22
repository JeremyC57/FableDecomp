// CPlayerGui::SetFlourishEffect(long effect) -- pass the melee flourish effect to the melee action-button effects drawer
// Leaf: ?SetFlourishEffect@CPlayerGui@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00437650, 17 bytes, `ret 4`: `mov eax,[ecx+220h] ; mov ecx,[esp+4] ; mov [eax+40h],ecx`.
// Name (NF239, 2026-11-22, CODEXP27 L21): Ego_r + FableWin publics ?SetFlourishEffect@CPlayerGui@@QAEXJ@Z (Ego_r 0x0044A2F0 / FableWin
//   0x01A7CD70); the landed caller 0x0064B1AC already binds it here. Body: PDrawMeleeActionButtonEffects (retail +0x220, Ego_r.pdb
//   CPlayerGui member) -> SetFlourishEffect(effect) (FableWin ?SetFlourishEffect@CDrawMeleeActionButtonEffects@NPlayerGui@@QAEXJ@Z,
//   inline here: FlourishEffect retail +0x40, Ego_d 80). Was the stand-in ?Set@T@@QAEXH@Z (catalog CPlayerGui::AddMoralityChange,
//   a propagated label).
// Model: engine/CPlayerGui.h (FABLE_RETAIL_CPLAYERGUI_MEMBERS: retail member offsets) with the p27 declaration opt-in, and the new
//   engine/NPlayerGui_CDrawMeleeActionButtonEffects.h (Ego_d field list); the define set is the landed ~CPlayerGui 0x0043A570's
//   (minus its destructor-only opt-ins).
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
#include "engine/NPlayerGui_CDrawMeleeActionButtonEffects.h"
#include "engine/CPlayerGui.h"
void CPlayerGui::SetFlourishEffect(long effect)
{
    PDrawMeleeActionButtonEffects->SetFlourishEffect(effect);
}
