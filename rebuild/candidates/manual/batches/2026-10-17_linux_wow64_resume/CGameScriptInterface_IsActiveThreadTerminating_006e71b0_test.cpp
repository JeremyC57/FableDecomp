// CGameScriptInterface::IsActiveThreadTerminating() -- read the active script runner's stop request.
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
bool __fastcall IsActiveThreadTerminating(CGameScriptInterface*, void*)
{
    return g_pScriptRunner->m_bStopRequested;
}


#include <cstdio>
CScriptRunner* g_pScriptRunner;
int main() {
 CScriptRunner first={0,0xa5,false}, second={0,0x5a,true};
 g_pScriptRunner=&first;
 if(IsActiveThreadTerminating(0,0)) return 1;
 first.m_bStopRequested=true;
 if(!IsActiveThreadTerminating(0,0)) return 2;
 g_pScriptRunner=&second;
 if(!IsActiveThreadTerminating(0,0)) return 3;
 second.m_bStopRequested=false;
 if(IsActiveThreadTerminating(0,0)) return 4;
 if(first.m_runnerState!=0xa5 || second.m_runnerState!=0x5a || !first.m_bStopRequested) return 5;
 std::printf("006e71b0_TEST PASS\n");return 0;
}
