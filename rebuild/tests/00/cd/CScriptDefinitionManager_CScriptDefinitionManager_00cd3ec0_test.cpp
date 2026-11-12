// Behaviour test for 0x00CD3EC0 CScriptDefinitionManager::~CScriptDefinitionManager (agentFAM08; the catalog test only printed its
// pass line). Model: the destructor resets the global script definition (ResetGlobalScriptDef) and then runs the
// CDefinitionManager base destructor. Test-only model types.
#include <cstdio>
static int g_seq[4];
static int g_count = 0;
static void Record(int v) { if (g_count < 4) g_seq[g_count++] = v; }
static void ResetGlobalScriptDefModel() { Record(1); }
struct ModelDefinitionManager { virtual ~ModelDefinitionManager() { Record(2); } };
struct ModelScriptDefinitionManager : ModelDefinitionManager { ~ModelScriptDefinitionManager() { ResetGlobalScriptDefModel(); } };
int main()
{
    { ModelScriptDefinitionManager m; }
    if (g_count != 2 || g_seq[0] != 1 || g_seq[1] != 2) { std::printf("00cd3ec0_TEST FAIL\n"); return 1; }
    std::printf("00cd3ec0_TEST PASS\n");
    return 0;
}
