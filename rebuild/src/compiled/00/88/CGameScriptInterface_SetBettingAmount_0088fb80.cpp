// Leaf: ?SetBettingAmount@CGameScriptInterface@@UAEXJ@Z
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern int g_bettingAmount;

void CGameScriptInterface::SetBettingAmount(long amount)
{
    g_bettingAmount = amount;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_SetBettingAmount -> CGameScriptInterface::SetBettingAmount, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
