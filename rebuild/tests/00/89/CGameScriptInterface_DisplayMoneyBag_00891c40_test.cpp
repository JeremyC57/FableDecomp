// Behaviour test for 0x00891C40 CGameScriptInterface::DisplayMoneyBag (NF431): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// The singleton's PClass word (its CCountedPointer Data at +0) is supplied here by /alternatename; the flag must land in the GUI's ShowMoneyBag byte (retail +0x29e) and nowhere else.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_CPLAYERGUI_SETSHOWMONEYBAG
#include "engine/CGameScriptInterface.h"
#include "engine/CPlayerGui.h"
#include <stdio.h>
#include <string.h>
static unsigned char g_gui[0x400];
extern "C" { void* nf431_pclass[2] = { g_gui, 0 }; }
#pragma comment(linker, "/alternatename:?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A=_nf431_pclass")
static long g_gsi[0x50 / 4];
static int run(bool v)
{
    memset(g_gui, 0x5a, sizeof g_gui);
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::DisplayMoneyBag(v);
    for (int i = 0; i < (int)sizeof g_gui; ++i) if (g_gui[i] != (i == 0x29e ? (unsigned char)v : 0x5a)) return 0;
    return 1;
}
int main(void)
{
    if (!run(true) || !run(false)) { printf("FAIL_00891c40\n"); return 1; }
    printf("PASS_00891c40 CGameScriptInterface_00891c40_TEST PASS\n"); return 0;
}
