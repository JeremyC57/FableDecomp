// CGameScriptInterface::GetBettingActive (stand-in free function; the virtual's own PDB leaf ?GetBettingActive@CGameScriptInterface@@... is not this entry)
// Name fix (NF202, 2026-11-20, q12256): the static at 0x013BAE1D is CTCAIScratchpad::BettingActive (Ego_r.pdb + FableWin.pdb + Ego_d.pdb
//   S_PUB32 ?BettingActive@CTCAIScratchpad@@2_NA; engine/CTCAIScratchpad.h FABLE_RETAIL_CTCAISCRATCHPAD_BETTING, the spelling the landed
//   0x007E5570 binds). Was the made-up extern g_bettingActive.
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

bool __fastcall CGameScriptInterface_GetBettingActive(void* self)
{
    (void)self;
    return CTCAIScratchpad::BettingActive;
}
