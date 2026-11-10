// Behaviour test for CWorldMap::UpdateNavMaps @ 0x00501990: like LoadThingsWithSectionNames but each record carries the level
// file itself as the navigation file, with progress ticks at the loop top, after SetAsLoading, after Add and per wait step
#include <stdio.h>
#include <string>
#include <vector>
#include <set>
enum ELoadType { WAIT_FOR_LOAD = 0, DONT_WAIT_FOR_LOAD = 1 };
struct LevelFiles { long LevelID; std::string NavigationFile; bool wad; };
static std::vector<std::string> g_Progress;
static void DisplayProgress(const std::string& text, float, bool, bool) { g_Progress.push_back(text); }
struct Loader { std::vector<LevelFiles> files; int busy; Loader() : busy(0) {} };
static void UpdateNavMaps(const std::vector<long>& loadedMaps, const std::vector<std::string>& levelNames, bool useWAD, Loader& loader,
                          const std::set<std::string>&, ELoadType loadType)
{
    std::vector<LevelFiles> levelFiles;
    for (std::vector<long>::const_iterator it = loadedMaps.begin(); it != loadedMaps.end(); ++it) {
        DisplayProgress("CWorldMap::UpdateNavMaps", -1.0f, false, false);
        LevelFiles f; f.LevelID = *it; f.NavigationFile = levelNames[*it]; f.wad = useWAD;
        levelFiles.push_back(f);
    }
    loader.files = levelFiles; loader.busy = 2;
    DisplayProgress("CWorldMap::UpdateNavMaps - SetAsLoading", -1.0f, false, false);
    DisplayProgress("CWorldMap::UpdateNavMaps - Add", -1.0f, false, false);
    while (loadType == WAIT_FOR_LOAD && loader.busy > 0) {
        DisplayProgress("CWorldMap::UpdateNavMaps - WAIT_FOR_LOAD", -1.0f, false, false);
        --loader.busy;
    }
}
int main()
{
    std::vector<std::string> names; names.push_back(""); names.push_back("a.lev"); names.push_back("b.lev");
    std::vector<long> loaded; loaded.push_back(1); loaded.push_back(2);
    Loader l; std::set<std::string> s;
    UpdateNavMaps(loaded, names, true, l, s, WAIT_FOR_LOAD);
    int ok = l.files.size() == 2 && l.files[1].NavigationFile == "b.lev" && l.busy == 0 && g_Progress.size() == 6
          && g_Progress[2] == "CWorldMap::UpdateNavMaps - SetAsLoading" && g_Progress[5] == "CWorldMap::UpdateNavMaps - WAIT_FOR_LOAD";
    g_Progress.clear();
    UpdateNavMaps(loaded, names, false, l, s, DONT_WAIT_FOR_LOAD);
    ok = ok && g_Progress.size() == 4 && l.busy == 2 && !l.files[0].wad;
    printf(ok ? "00501990_TEST PASS\n" : "00501990_TEST FAIL\n");
    return ok ? 0 : 1;
}