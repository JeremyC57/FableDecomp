#include <cstdio>
#include <cstring>
// Model of GFLoadScriptDef_00cbf647 @ 0x00CBF647
static int g_strCtor = 0, g_strDtor = 0;
struct CCharString { const char* m_text; CCharString(const char* t, long) : m_text(t) { ++g_strCtor; } ~CCharString() { ++g_strDtor; } };
struct CDefPointer { int id; };
static CDefPointer g_scriptDef = { 0 }; static const char* g_name = 0; static CDefPointer* g_def = 0; static int g_mgrCalls = 0; static int g_liveStrings = -1;
struct CDefinitionManager { bool GetDef(const CCharString& name, CDefPointer& def) { g_name = name.m_text; g_def = &def; g_liveStrings = g_strCtor - g_strDtor; def.id = 99; return true; } };
static CDefinitionManager g_mgr;
static CDefinitionManager* GFGetPDefinitionManager() { ++g_mgrCalls; return &g_mgr; }
static void GFLoadScriptDef_00cbf647() { CCharString name("SCRIPT_DEF", -1); GFGetPDefinitionManager()->GetDef(name, g_scriptDef); }
int main()
{
    GFLoadScriptDef_00cbf647();
    if (g_mgrCalls != 1 || g_def != &g_scriptDef || g_scriptDef.id != 99 || g_name == 0 || std::strcmp(g_name, "SCRIPT_DEF") != 0 || g_liveStrings != 1 || g_strDtor != 1) { std::printf("00cbf647_TEST FAIL\n"); return 1; }
    std::printf("00cbf647_TEST PASS\n"); return 0;
}