// Behaviour test for 0x0088E4D0 CGameScriptInterface::IsScreenFadingOut (NF433): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// The display engine is raw bytes here; the row must read Active (+0xBC) and Opening (+0xC9) of DisplayEngine (+0x0c) and return both.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CDISPLAYENGINE_SCREENFADEOUT
#define FABLE_RETAIL_CFADEINFADEOUTBASE_OPEN
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISOPENING_PASS26
#define FABLE_RETAIL_CDISPLAYENGINE_ISSCREENFADEOUTOPENING
#include "engine/CGameScriptInterface.h"
#include "engine/CDisplayEngine.h"
#include <stdio.h>
#include <string.h>
static unsigned char g_de[0x100];
static long g_gsi[0x50 / 4];
static int run(unsigned char act, unsigned char open, bool want)
{
    g_de[0xbc] = act; g_de[0xc9] = open;
    return ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::IsScreenFadingOut() == want;
}
int main(void)
{
    g_gsi[0x0c / 4] = (long)g_de;
    if (!run(1, 1, true) || !run(1, 0, false) || !run(0, 1, false) || !run(0, 0, false)) { printf("FAIL_0088e4d0\n"); return 1; }
    printf("PASS_0088e4d0 CGameScriptInterface_0088e4d0_TEST PASS\n"); return 0;
}
