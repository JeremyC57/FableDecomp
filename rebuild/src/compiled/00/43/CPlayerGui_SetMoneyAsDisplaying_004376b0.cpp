// CPlayerGui::SetMoneyAsDisplaying() -- reopen the money stat bar: clear its fully-opened frame and put a bar that is not closed back
// into the opening mode
// Leaf: ?SetMoneyAsDisplaying@CPlayerGui@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004376B0, 36 bytes, thiscall, no arguments.
// Identity: Ego_r.pdb ?SetMoneyAsDisplaying@CPlayerGui@@QAEXXZ 0x0044A350, the first of the seven Set*AsDisplaying members
//   0x0044A350..0x0044A470 (Money, Renown, Morality, Experience, Crime, StatUpdates, AutoPickup), which retail keeps in the same order
//   at 0x004376B0..0x00437770; this body reads PMoney (+0x2CC), the landed 0x00437770 reads PCrime (+0x2EC). Caller: CTCShop::FrameUpdate
//   0x00775B40. (Landed earlier as a placeholder free function spelled CPlayerGui_SetCrimeAsDisplaying; respelled 2026-11-16.)
// Inferred: EPlayerGuiMode values -- 0 closed, 2 opening -- from the landed CPlayerGui::UpdateTimer (2 = opening, 3 = closing) and the
//   twin 0x00437770; no PDB lists the enumerators, so they are written as casts.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_CPLAYERGUI_SETMONEYASDISPLAYING
#include "engine/CPlayerGui.h"
#include "engine/NPlayerGui_CStatBar.h"

void CPlayerGui::SetMoneyAsDisplaying()
{
    PMoney->FrameFullyOpenedOn = 0;
    if (PMoney->GuiMode != (EPlayerGuiMode)0)
        PMoney->GuiMode = (EPlayerGuiMode)2;
}
