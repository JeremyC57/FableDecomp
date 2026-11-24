// Leaf: ?SetPlayerUsingWillDummies@CGameScriptInterface@@UAEX_N@Z
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern unsigned char g_playerUsingWillDummies;

void CGameScriptInterface::SetPlayerUsingWillDummies(bool bUsing)
{
    g_playerUsingWillDummies = (unsigned char)bUsing;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_SetPlayerUsingWillDummies -> CGameScriptInterface::SetPlayerUsingWillDummies, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
