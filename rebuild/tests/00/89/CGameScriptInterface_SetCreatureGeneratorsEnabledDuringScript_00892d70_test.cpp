// Behaviour test for 0x00892D70 CGameScriptInterface::SetCreatureGeneratorsEnabledDuringScript (NF433): links the row object (the row is called by its own
// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row's shared
// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.
// The singleton's PClass word (its CCountedPointer Data at +0) is supplied here by /alternatename; the row must pass the name and the flag to SetCreatureGenerationEnabledDuringScript on that object, once.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_CREATUREGENERATIONDURINGSCRIPT
#include "engine/CGameScriptInterface.h"
#include "engine/CQuestManager.h"
#include <stdio.h>
#include <string.h>
static int g_calls = 0;
static const void* g_this = 0;
static const void* g_name = 0;
static bool g_en = false;
void CQuestManager::SetCreatureGenerationEnabledDuringScript(const CCharString& script_name, bool enabled) { ++g_calls; g_this = this; g_name = &script_name; g_en = enabled; }
static long g_qm[0x40];
extern "C" { void* nf433_qm_pclass[2] = { g_qm, 0 }; }
#pragma comment(linker, "/alternatename:?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A=_nf433_qm_pclass")
static long g_gsi[0x50 / 4];
static long g_str[4];
int main(void)
{
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::SetCreatureGeneratorsEnabledDuringScript(*(const CCharString*)g_str, true);
    if (g_calls != 1 || g_this != (const void*)g_qm || g_name != (const void*)g_str || !g_en) { printf("FAIL_00892d70\n"); return 1; }
    ((const CGameScriptInterface*)g_gsi)->CGameScriptInterface::SetCreatureGeneratorsEnabledDuringScript(*(const CCharString*)g_str, false);
    if (g_calls != 2 || g_en) { printf("FAIL_00892d70\n"); return 1; }
    printf("PASS_00892d70 SETCREATUREGEN_OK\n"); return 0;
}
