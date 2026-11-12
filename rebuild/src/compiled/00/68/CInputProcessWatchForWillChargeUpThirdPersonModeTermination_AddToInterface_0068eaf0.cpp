// CInputProcessWatchForWillChargeUpThirdPersonModeTermination::AddToInterface(CInputProcessManager& mgr) -- reset the quit-mode timer, then the base registration
// Leaf: ?AddToInterface@CInputProcessWatchForWillChargeUpThirdPersonModeTermination@@UAE_NAAVCInputProcessManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0068EAF0, `mov dword ptr [ecx+28h],-1 ; jmp 0x00687A70`.
// NF28 (2026-11-12, queue land1011eh): the base call was spelled through a made-up CInputProcessWatchForBaseThirdPersonModeTermination
//   (no such class in Ego_r.pdb) with a non-virtual QAE AddToInterface; the jump target 0x00687A70 is
//   ?AddToInterface@CInputProcessGameBase@@UAE_NAAVCInputProcessManager@@@Z (Ego_r.pdb 0x006B2B80 / FableWin.pdb 0x021D3CB0), as the
//   sibling CInputProcessWatchForRangedWeaponThirdPersonModeTermination 0x0068EAE0 binds it. Ego_r.pdb: this class (0x2C bytes) has
//   QuitModeTimer at +0x28 and the virtual override ?AddToInterface@...@@UAE_NAAVCInputProcessManager@@@Z (0x006BAD70).
// Model: engine/CInputProcessWatchForWillChargeUpThirdPersonModeTermination.h (QuitModeTimer +0x28) with the AddToInterface opt-in.
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CINPUTPROCESSWATCHFORWILLCHARGEUPTHIRDPERSONMODETERMINATION_ADDTOINTERFACE
#include "engine/CInputProcessWatchForWillChargeUpThirdPersonModeTermination.h"

bool CInputProcessWatchForWillChargeUpThirdPersonModeTermination::AddToInterface(CInputProcessManager& mgr)
{
    QuitModeTimer = -1;
    return CInputProcessGameBase::AddToInterface(mgr);
}
