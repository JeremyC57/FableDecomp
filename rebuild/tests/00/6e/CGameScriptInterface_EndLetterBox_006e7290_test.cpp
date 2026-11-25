// Behaviour test for 0x006E7290 CGameScriptInterface::EndLetterBox (NF433): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// EndLetterBoxMode is defined here; the row must call it once on DisplayEngine (+0x0c). EndLetterBox is protected, so the call goes through a derived access helper (no object of it exists).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CDISPLAYENGINE_ENDLETTERBOXMODE
#include "engine/CGameScriptInterface.h"
#include "engine/CDisplayEngine.h"
#include <stdio.h>
#include <string.h>
static int g_calls = 0;
static const void* g_this = 0;
void CDisplayEngine::EndLetterBoxMode() { ++g_calls; g_this = this; }
struct NF433Access : CGameScriptInterface { static void Call(CGameScriptInterface* p) { static_cast<NF433Access*>(p)->CGameScriptInterface::EndLetterBox(); } };
static long g_gsi[0x50 / 4];
static long g_de[0x40];
int main(void)
{
    g_gsi[0x0c / 4] = (long)g_de;
    NF433Access::Call((CGameScriptInterface*)g_gsi);
    if (g_calls != 1 || g_this != (const void*)g_de) { printf("FAIL_006e7290\n"); return 1; }
    NF433Access::Call((CGameScriptInterface*)g_gsi);
    if (g_calls != 2) { printf("FAIL_006e7290\n"); return 1; }
    printf("PASS_006e7290 CGameScriptInterface_006e7290_TEST PASS\n"); return 0;
}
