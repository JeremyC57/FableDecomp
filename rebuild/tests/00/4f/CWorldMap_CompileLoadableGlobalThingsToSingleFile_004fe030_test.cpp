// 004fe030 behaviour test: CWorldMap::CompileLoadableGlobalThingsToSingleFile (final.cpp body over mock managers)
#include <stdio.h>
#include <string>
#include <vector>
#include <list>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static std::string IntToString(long i) { char b[16]; sprintf(b, "%ld", i); return b; }
static int g_progress; static float g_lastFraction;
static void DisplayProgress(const char*, bool, float f, bool) { ++g_progress; g_lastFraction = f; }
class CThingManager { public:
    long SavingFlag; std::vector<long> Loaded; int Deletes;
    void SaveToScript(std::string& script, long map, std::vector<std::string>&) { script += "THINGS" + IntToString(map) + "@" + IntToString(SavingFlag); }
    void DeleteAllThings() { ++Deletes; Loaded.clear(); }
};
class CMapInfo { public: bool Used, LoadedOnPlayerProximity; };
static std::string g_savedName, g_savedText;
class CWorldMap { public:
    CThingManager* Manager; std::vector<CMapInfo> Maps;
    void LoadLoadableGlobalThingsForLevel(long map) { Manager->Loaded.push_back(map); }
    void CompileLoadableGlobalThingsToSingleFile(const std::string& filename);
};
void CWorldMap::CompileLoadableGlobalThingsToSingleFile(const std::string& filename)
{
    std::string script;
    long oldFlag = Manager->SavingFlag;
    Manager->SavingFlag = 1;
    for (long i = 1; i < (long)Maps.size(); ++i) {
        DisplayProgress("Compiling global things", false, (float)i / (float)(Maps.size() + 1), false);
        CMapInfo& map = Maps[i];
        if (map.Used && map.LoadedOnPlayerProximity) {
            std::list<void*> things;
            LoadLoadableGlobalThingsForLevel(i);
            script += "NEWMAP " + IntToString(i) + "\r\n";
            std::vector<std::string> names;
            Manager->SaveToScript(script, i, names);
            script += "\r\nENDMAP\r\n";
            Manager->DeleteAllThings();
        }
    }
    Manager->SavingFlag = oldFlag;
    g_savedName = filename;
    g_savedText = script;
}
int main()
{
    CThingManager tm = { 7, std::vector<long>(), 0 };
    CWorldMap wm; wm.Manager = &tm;
    CMapInfo m0 = { true, true }, m1 = { true, true }, m2 = { true, false }, m3 = { false, true }, m4 = { true, true };
    wm.Maps.push_back(m0); wm.Maps.push_back(m1); wm.Maps.push_back(m2); wm.Maps.push_back(m3); wm.Maps.push_back(m4);
    wm.CompileLoadableGlobalThingsToSingleFile("globals.bin");
    CHECK(g_progress == 4 && g_lastFraction == 4.0f / 6.0f);
    CHECK(tm.SavingFlag == 7 && tm.Deletes == 2);
    CHECK(g_savedName == "globals.bin");
    CHECK(g_savedText == "NEWMAP 1\r\nTHINGS1@1\r\nENDMAP\r\nNEWMAP 4\r\nTHINGS4@1\r\nENDMAP\r\n");
    if (!g_fail) printf("004fe030_TEST PASS\n");
    return g_fail;
}