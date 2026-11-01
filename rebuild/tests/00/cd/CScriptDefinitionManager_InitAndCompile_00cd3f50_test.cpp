// Test for CScriptDefinitionManager::InitAndCompile() -- builds the five symbol paths and two compile paths in retail order, hands them to
// the manager, compiles, clears the symbols, saves the string table only when binary definitions are saved, then sets the global script def.
#include <cstdio>
#include <cstring>
static char g_log[256];
static bool g_bSaveBinaryDefinitions = true;
struct PathList { int n; char p[8][48]; PathList() : n(0) { } void push_back(const char* a, const char* b) { std::strcpy(p[n], a); std::strcat(p[n], b); ++n; } };
class CScriptDefinitionManager { public:
    PathList SymbolPathList, CompilePathList;
    void Compile() { std::strcat(g_log, "C"); }
    void ClearSymbols() { std::strcat(g_log, "X"); }
    void InitAndCompile();
};
static void SaveTable() { std::strcat(g_log, "S"); }
static void SetGlobalScriptDef() { std::strcat(g_log, "G"); }
void CScriptDefinitionManager::InitAndCompile()
{
    PathList symbolPaths, compilePaths;
    const char* header = "Data\\Symbols\\";
    char headerPc[64]; std::strcpy(headerPc, header); std::strcat(headerPc, "pc\\");
    symbolPaths.push_back(header, "*.h");
    symbolPaths.push_back(headerPc, "*.h");
    symbolPaths.push_back("Data\\Defs\\", "ScriptDefs\\*.h");
    symbolPaths.push_back("Data\\Defs\\", "thing_def.h");
    symbolPaths.push_back("Data\\Defs\\", "ScriptDefs\\*.def");
    compilePaths.push_back("Data\\Defs\\", "ScriptDefs\\*.tpl");
    compilePaths.push_back("Data\\Defs\\", "ScriptDefs\\*.def");
    SymbolPathList = symbolPaths;
    CompilePathList = compilePaths;
    Compile();
    ClearSymbols();
    if (g_bSaveBinaryDefinitions)
        SaveTable();
    SetGlobalScriptDef();
}
int main()
{
    CScriptDefinitionManager m;
    m.InitAndCompile();
    bool ok = std::strcmp(g_log, "CXSG") == 0 && m.SymbolPathList.n == 5 && m.CompilePathList.n == 2
        && std::strcmp(m.SymbolPathList.p[1], "Data\\Symbols\\pc\\*.h") == 0 && std::strcmp(m.CompilePathList.p[0], "Data\\Defs\\ScriptDefs\\*.tpl") == 0;
    g_log[0] = 0; g_bSaveBinaryDefinitions = false;
    m.InitAndCompile();
    ok = ok && std::strcmp(g_log, "CXG") == 0;
    if (!ok) { std::printf("00cd3f50_TEST FAIL\n"); return 1; }
    std::printf("00cd3f50_TEST PASS\n");
    return 0;
}