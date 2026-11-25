// Behaviour test for 0x006E72A0 CGameScriptInterface::Validate() const (NF446): links the row object and calls it by its own symbol (a
// qualified call, not through the vftable). No class body is defined here: the classes come from the row's shared headers (its prelude,
// verbatim). World (+0x04) is a raw object whose vftable slot 3 (GetThingManager) returns a planted thing manager; the tail-jump target
// CClassFactory::Validate is defined here and records its `this`. Prints the landed pass pattern `VALIDATE PASS`.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CCLASSFACTORY_VALIDATE
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include "engine/CThingManager.h"
#include <stdio.h>
#include <string.h>
static unsigned char g_tm[0x40];
static const void* g_this = 0; static int g_calls = 0; static int g_gets = 0;
void CClassFactory::Validate() { ++g_calls; g_this = this; }
extern "C" void* __fastcall nf446_get_thing_manager(void*) { ++g_gets; return g_tm; }
static void* g_vt[8]; static void* g_world[4]; static unsigned char g_gsi[0x40];
int main(void)
{
    memset(g_gsi, 0, sizeof g_gsi);
    g_vt[3] = (void*)&nf446_get_thing_manager; g_world[0] = g_vt;
    *(void**)(g_gsi + 4) = g_world;
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::Validate();
    if (g_calls != 1 || g_gets != 1 || g_this != (const void*)static_cast<CClassFactory*>((CThingManager*)g_tm)) { printf("006e72a0_TEST FAIL\n"); return 1; }
    printf("VALIDATE PASS\n"); return 0;
}
