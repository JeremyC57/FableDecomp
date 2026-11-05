// CGameScriptInterface::IsActiveThreadTerminating() -- read the active script runner's stop request.
// Leaf: ?CGameScriptInterface_IsActiveThreadTerminating@@YI_NPAVCGameScriptInterface@@PAX@Z
// Retail: 0x006E71B0, 9 bytes; no callees. Global script runner 0x013D2838, stop byte +5.
// The landed Rest 0x006E71F0 and NewScriptFrame 0x006E7410 independently use this global/flag.
// This query requires an active runner; callers establish it. The interface object is unused.
#include "engine/CGameScriptInterface.h"
struct CScriptRunner {
    void* __vftable;
    unsigned char m_runnerState;
    bool m_bStopRequested;
};
extern CScriptRunner* g_pScriptRunner; // 0x013D2838
bool __fastcall CGameScriptInterface_IsActiveThreadTerminating(CGameScriptInterface*, void*)
{
    return g_pScriptRunner->m_bStopRequested;
}