// Leaf: ?SetCountBetMoneyDown@CGameScriptInterface@@UAEX_N@Z
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern unsigned char g_countBetMoneyDown;

void CGameScriptInterface::SetCountBetMoneyDown(bool count)
{
    g_countBetMoneyDown = (unsigned char)count;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_SetCountBetMoneyDown -> CGameScriptInterface::SetCountBetMoneyDown, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
