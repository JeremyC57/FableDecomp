// CGameScriptInterface::IsActiveThreadTerminating() -- read the active script runner's stop request.
// Leaf: ?IsActiveThreadTerminating@CGameScriptInterface@@UBE_NXZ
// Retail: 0x006E71B0, 9 bytes; no callees. Global script runner 0x013D2838, stop byte +5.
// The landed Rest 0x006E71F0 and NewScriptFrame 0x006E7410 independently use this global/flag.
// This query requires an active runner; callers establish it. The interface object is unused.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
#include "engine/CSuspendableProcessMicrothread.h"   // CASuspendableProcess::StopRunning +5
extern CSuspendableProcessMicrothread* g_pScriptRunner; // 0x013D2838 (claim ?g_pScriptRunner@@3PAVCSuspendableProcessMicrothread@@A)
bool CGameScriptInterface::IsActiveThreadTerminating() const
{
    return g_pScriptRunner->StopRunning;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_IsActiveThreadTerminating -> CGameScriptInterface::IsActiveThreadTerminating, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
