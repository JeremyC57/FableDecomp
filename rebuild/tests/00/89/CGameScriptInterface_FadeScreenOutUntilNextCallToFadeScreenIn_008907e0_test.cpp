// Behaviour test for 0x008907E0 CGameScriptInterface::FadeScreenOutUntilNextCallToFadeScreenIn (NF433): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// The interface's slot 373 (FadeScreenOut, +0x5D4) is planted here; the row must call it with both times and an opaque black colour
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CDISPLAYENGINE_SETSCREENFADEOUTLOCKFLAG
#include "engine/CGameScriptInterface.h"
#include "engine/CRGBColour.h"
#include "engine/CDisplayEngine.h"
#include <stdio.h>
#include <string.h>
// (r = g = b = 0, alpha 255), and set the display engine's lock byte (+0xD8) only when it returns true.
static int g_calls = 0; static const void* g_self = 0; static float g_a = 0, g_b = 0; static unsigned long g_col = 0; static bool g_ret = false;
extern "C" bool __fastcall nf433_fadeout(const void* self, void*, float a, float b, unsigned long colour)
{ ++g_calls; g_self = self; g_a = a; g_b = b; g_col = colour; return g_ret; }
static void* g_vtbl[920];
static unsigned char g_de[0x100];
static void* g_gsi[0x50 / 4];
static int run(bool ret, float a, float b, unsigned char want)
{
    g_ret = ret; g_calls = 0; g_de[0xd8] = 0x5a;
    ((CGameScriptInterface*)g_gsi)->CGameScriptInterface::FadeScreenOutUntilNextCallToFadeScreenIn(a, b);
    return g_calls == 1 && g_self == (const void*)g_gsi && g_a == a && g_b == b && g_col == 0xff000000ul && g_de[0xd8] == want;
}
int main(void)
{
    for (int i = 0; i < 920; ++i) g_vtbl[i] = 0; g_vtbl[0x5d4 / 4] = (void*)&nf433_fadeout; g_gsi[0] = g_vtbl; g_gsi[0x0c / 4] = g_de;
    if (!run(true, 1.5f, 2.5f, 1) || !run(false, 0.5f, 3.0f, 0x5a)) { printf("FAIL_008907e0\n"); return 1; }
    printf("PASS_008907e0 FADE_OK\n"); return 0;
}
