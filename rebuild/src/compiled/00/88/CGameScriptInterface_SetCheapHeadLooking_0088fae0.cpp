// Leaf: ?SetCheapHeadLooking@CGameScriptInterface@@UAEX_N@Z
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern unsigned char g_cheapHeadLooking;
unsigned char g_cheapHeadLooking;
void CGameScriptInterface::SetCheapHeadLooking(bool val)
{
    g_cheapHeadLooking = (unsigned char)val;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_SetCheapHeadLooking -> CGameScriptInterface::SetCheapHeadLooking, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
