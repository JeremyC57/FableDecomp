// Test for 0x009AF720 CDefinitionManager::RecompileAllDefs: as RecompileDefType with the empty type name ("" = every type)
// for both the clear and the compile step.
#include <stdio.h>
#include <string.h>
static char g_log[128];
static void Log(const char* s) { strcat(g_log, s); }
class CSymbolMap { public: int Count; bool IsEmpty() const { return Count == 0; } };
class CDefinitionManager {
public:
    void RecompileAllDefs()
    {
        ClearDefsForRecompile("");
        if (PSymbolMap_Data->IsEmpty()) CreateSymbolsFromPathList(SymbolPathList, false);
        DoCompilePathList(CompilePathList, "");
    }
    CSymbolMap* PSymbolMap_Data; int SymbolPathList; int CompilePathList;
protected:
    void ClearDefsForRecompile(const char* type) { Log(type[0] == 0 ? "C" : "?"); }
    void CreateSymbolsFromPathList(int paths, bool flag) { Log(paths == 1 && !flag ? "S" : "?"); }
    void DoCompilePathList(int paths, const char* type) { Log(paths == 2 && type[0] == 0 ? "D" : "?"); }
};
int main()
{
    CSymbolMap map; map.Count = 0;
    CDefinitionManager m; m.PSymbolMap_Data = &map; m.SymbolPathList = 1; m.CompilePathList = 2;
    g_log[0] = 0; m.RecompileAllDefs();
    bool ok = strcmp(g_log, "CSD") == 0;
    map.Count = 1; g_log[0] = 0; m.RecompileAllDefs();
    if (strcmp(g_log, "CD") != 0) ok = false;
    puts(ok ? "009af720_TEST PASS" : "009af720_TEST FAIL");
    return ok ? 0 : 1;
}