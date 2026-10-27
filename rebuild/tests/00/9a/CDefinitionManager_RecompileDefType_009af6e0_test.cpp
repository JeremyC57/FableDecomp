// Test for 0x009AF6E0 CDefinitionManager::RecompileDefType: clears the type's defs, rebuilds the symbols from the symbol paths
// only when the symbol map is empty, then compiles the compile paths for that type.
#include <stdio.h>
#include <string.h>
static char g_log[128];
static void Log(const char* s) { strcat(g_log, s); }
class CSymbolMap { public: int Count; bool IsEmpty() const { return Count == 0; } };
class CDefinitionManager {
public:
    void RecompileDefType(const char* type)
    {
        ClearDefsForRecompile(type);
        if (PSymbolMap_Data->IsEmpty()) CreateSymbolsFromPathList(SymbolPathList, false);
        DoCompilePathList(CompilePathList, type);
    }
    CSymbolMap* PSymbolMap_Data; int SymbolPathList; int CompilePathList; const char* LastType;
protected:
    void ClearDefsForRecompile(const char* type) { Log("C"); LastType = type; }
    void CreateSymbolsFromPathList(int paths, bool flag) { Log(paths == 1 && !flag ? "S" : "?"); }
    void DoCompilePathList(int paths, const char* type) { Log(paths == 2 && type == LastType ? "D" : "?"); }
};
int main()
{
    CSymbolMap map; map.Count = 0;
    CDefinitionManager m; m.PSymbolMap_Data = &map; m.SymbolPathList = 1; m.CompilePathList = 2;
    g_log[0] = 0; m.RecompileDefType("CREATURE");
    bool ok = strcmp(g_log, "CSD") == 0;
    map.Count = 3; g_log[0] = 0; m.RecompileDefType("OBJECT");
    if (strcmp(g_log, "CD") != 0) ok = false;
    puts(ok ? "009af6e0_TEST PASS" : "009af6e0_TEST FAIL");
    return ok ? 0 : 1;
}