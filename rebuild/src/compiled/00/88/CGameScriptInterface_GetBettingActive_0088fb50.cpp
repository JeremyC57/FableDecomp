// Leaf: ?GetBettingActive@CGameScriptInterface@@UAE_NXZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CGameScriptInterface.h under its class-model switch FABLE_RETAIL_NSCRIPT (was the flattened
//   free stand-in ?CGameScriptInterface_GetBettingActive@@YI_NPAX@Z); the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
// CGameScriptInterface::GetBettingActive (public virtual, Ego_r / FableWin ?GetBettingActive@CGameScriptInterface@@UAE_NXZ)
// Name fix (NF202, 2026-11-20, q12256): the static at 0x013BAE1D is CTCAIScratchpad::BettingActive (Ego_r.pdb + FableWin.pdb + Ego_d.pdb
//   S_PUB32 ?BettingActive@CTCAIScratchpad@@2_NA; engine/CTCAIScratchpad.h FABLE_RETAIL_CTCAISCRATCHPAD_BETTING, the spelling the landed
//   0x007E5570 binds). Was the made-up extern g_bettingActive.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCAISCRATCHPAD_BETTING
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/CGameScriptInterface.h"   // switch route: the real class model

bool CGameScriptInterface::GetBettingActive()
{
    return CTCAIScratchpad::BettingActive;
}
