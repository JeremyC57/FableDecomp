// Leaf: ?SetPrizeTavernTable@CGameScriptInterface@@UAEX_N@Z
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern unsigned char g_prizeTavernTable;

void CGameScriptInterface::SetPrizeTavernTable(bool enabled)
{
    g_prizeTavernTable = (unsigned char)enabled;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_SetPrizeTavernTable -> CGameScriptInterface::SetPrizeTavernTable, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
