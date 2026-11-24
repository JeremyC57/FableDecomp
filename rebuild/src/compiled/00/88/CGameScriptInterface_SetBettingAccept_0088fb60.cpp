// Leaf: ?SetBettingAccept@CGameScriptInterface@@UAEX_N@Z
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern unsigned char g_bettingAccept;
unsigned char g_bettingAccept;

void CGameScriptInterface::SetBettingAccept(bool accept)
{
    g_bettingAccept = (unsigned char)accept;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_SetBettingAccept -> CGameScriptInterface::SetBettingAccept, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
