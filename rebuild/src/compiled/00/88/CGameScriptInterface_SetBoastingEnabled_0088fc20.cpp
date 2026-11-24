// Leaf: ?SetBoastingEnabled@CGameScriptInterface@@UAEX_N@Z

#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
extern unsigned char g_boastingEnabled;

void CGameScriptInterface::SetBoastingEnabled(bool enabled)
{
g_boastingEnabled = (unsigned char)enabled;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_SetBoastingEnabled -> CGameScriptInterface::SetBoastingEnabled, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
