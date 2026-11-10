// Behaviour test for CWorldMap::LoadThingsWithSectionNames @ 0x00500EB0: one level-files record per loaded map with its thing
// file (from the level WAD bank when one is in use, else from disk), one loader object for the sections, and with
// WAIT_FOR_LOAD the loader is run until it is idle
#include <stdio.h>
#include <string>
#include <vector>
#include <set>
enum ELoadType { WAIT_FOR_LOAD = 0, DONT_WAIT_FOR_LOAD = 1 };
struct File { std::string name; bool wad; };
struct LevelFiles { long LevelID; File ThingFile; };
struct Loader {
    std::vector<std::vector<LevelFiles> > objects; std::vector<std::set<std::string> > sections; int busy; int runs;
    Loader() : busy(0), runs(0) {}
    bool IsBusy() const { return busy > 0; }
    void RunSuspendableProcess() { --busy; ++runs; }
};
static std::string GetThingFilenameFromLevelFilename(const std::string& level) { return level.substr(0, level.size() - 4) + ".tng"; }
static void LoadThingsWithSectionNames(const std::vector<long>& loadedMaps, const std::vector<std::string>& levelNames, bool useWAD,
                                       Loader& loader, const std::set<std::string>& sections, ELoadType loadType)
{
    std::vector<LevelFiles> levelFiles;
    for (std::vector<long>::const_iterator it = loadedMaps.begin(); it != loadedMaps.end(); ++it) {
        long map = *it;
        File file; file.name = GetThingFilenameFromLevelFilename(levelNames[map]); file.wad = useWAD;
        levelFiles.push_back(LevelFiles());
        levelFiles.back().LevelID = map;
        levelFiles.back().ThingFile = file;
    }
    loader.objects.push_back(levelFiles); loader.sections.push_back(sections); loader.busy = 3;
    while (loadType == WAIT_FOR_LOAD && loader.IsBusy())
        loader.RunSuspendableProcess();
}
int main()
{
    std::vector<std::string> names; names.push_back(""); names.push_back("a.lev"); names.push_back("b.lev");
    std::vector<long> loaded; loaded.push_back(2); loaded.push_back(1);
    std::set<std::string> sections; sections.insert("S1");
    Loader l;
    LoadThingsWithSectionNames(loaded, names, true, l, sections, WAIT_FOR_LOAD);
    int ok = l.objects.size() == 1 && l.objects[0].size() == 2 && l.objects[0][0].LevelID == 2
          && l.objects[0][0].ThingFile.name == "b.tng" && l.objects[0][1].ThingFile.wad && l.runs == 3 && !l.IsBusy();
    LoadThingsWithSectionNames(loaded, names, false, l, sections, DONT_WAIT_FOR_LOAD);
    ok = ok && l.objects.size() == 2 && !l.objects[1][0].ThingFile.wad && l.IsBusy() && l.sections[1].count("S1") == 1;
    printf(ok ? "00500eb0_TEST PASS\n" : "00500eb0_TEST FAIL\n");
    return ok ? 0 : 1;
}