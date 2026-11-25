// Behaviour test for 0x00891C60 CGameScriptInterface::DisplayMiniGameInfo (NF433): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// The singleton's PClass word is supplied by /alternatename and DisplayMiniGameInfo is defined here; the row must reach the quest info through
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
#define FABLE_RETAIL_CPLAYERGUI_GETQUESTINFO
#define FABLE_RETAIL_CDRAWQUESTINFO_ORACLESYMBOLS
#include "engine/CGameScriptInterface.h"
#include "engine/CPlayerGui.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"
#include <stdio.h>
#include <string.h>
// the GUI's PDrawQuestInfo word (+0x1C8) and pass (display, type, 0), once.
static long g_gui[0x100]; static long g_qi[8];
extern "C" { void* nf433_pg_pclass[2] = { g_gui, 0 }; }
#pragma comment(linker, "/alternatename:?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A=_nf433_pg_pclass")
static int g_calls = 0; static const void* g_this = 0; static bool g_disp = false; static EMiniGameType g_type; static long g_extra = 7;
void NPlayerGui::CDrawQuestInfo::DisplayMiniGameInfo(bool display, EMiniGameType type, long extra) { ++g_calls; g_this = this; g_disp = display; g_type = type; g_extra = extra; }
static long g_gsi[0x50 / 4];
int main(void)
{
    g_gui[0x1c8 / 4] = (long)g_qi;
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::DisplayMiniGameInfo(true, (EMiniGameType)4);
    if (g_calls != 1 || g_this != (const void*)g_qi || !g_disp || g_type != (EMiniGameType)4 || g_extra != 0) { printf("FAIL_00891c60\n"); return 1; }
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::DisplayMiniGameInfo(false, (EMiniGameType)1);
    if (g_calls != 2 || g_disp || g_type != (EMiniGameType)1) { printf("FAIL_00891c60\n"); return 1; }
    printf("PASS_00891c60 CGameScriptInterface_00891c60_TEST PASS\n"); return 0;
}
