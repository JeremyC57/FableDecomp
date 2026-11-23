// Leaf: ?GetGlobalTargetingDistanceOffset@CGameScriptInterface@@UAEMXZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CGameScriptInterface.h under its class-model switch FABLE_RETAIL_NSCRIPT (was the flattened
//   free stand-in ?CGameScriptInterface_GetGlobalTargetingDistanceOffset@@YIMPAX@Z); the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"   // switch route: the real class model
extern float g_GlobalTargetingDistanceOffset;

float CGameScriptInterface::GetGlobalTargetingDistanceOffset()
{
    return g_GlobalTargetingDistanceOffset;
}