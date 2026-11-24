// Leaf: ?SetGlobalTargetingDistanceOffset@CGameScriptInterface@@UAEXM@Z
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern float g_globalTargetingDistanceOffset;
float g_globalTargetingDistanceOffset;

void CGameScriptInterface::SetGlobalTargetingDistanceOffset(float offset)
{
    g_globalTargetingDistanceOffset = offset;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_SetGlobalTargetingDistanceOffset -> CGameScriptInterface::SetGlobalTargetingDistanceOffset, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
