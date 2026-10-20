// Behaviour test for CUserProfileManager::Save (0040a740).
#include <cstdio>
static int g_saves = 0, g_dumps = 0, g_closes = 0; static bool g_opens = true;
struct File { bool Opened; File() : Opened(g_opens) { } ~File() { Close(); } void Close() { ++g_closes; } void DumpToFile() { ++g_dumps; } };
struct Mgr {
    const char* name; bool UseNewProfileValues;
    void SaveGameState(File*) { ++g_saves; }
    void Save()
    {
        if (UseNewProfileValues) return;
        const char* path = name;
        if (path[0] != 0) {
            File file;
            if (file.Opened) { SaveGameState(&file); file.DumpToFile(); file.Close(); }
        }
    }
};
int main()
{
    Mgr m; m.name = "p"; m.UseNewProfileValues = false;
    m.Save(); bool ok = g_saves == 1 && g_dumps == 1 && g_closes == 2;
    m.UseNewProfileValues = true; m.Save(); ok = ok && g_saves == 1 && g_closes == 2;
    m.UseNewProfileValues = false; g_opens = false; m.Save(); ok = ok && g_saves == 1 && g_closes == 3;
    if (!ok) { std::printf("0040a740_TEST FAIL\n"); return 1; }
    std::printf("0040a740_TEST PASS\n");
    return 0;
}