// Behaviour test for CWorldMap::LoadLevel @ 0x00502620: the level file (used as topology and navigation file) and the thing
// file derived from its name go to LoadLevelFromFiles, then the map records both files' modification stamps
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#ifdef _MSC_VER
typedef unsigned __int64 UInt64;
#else
typedef unsigned long long UInt64;
#endif
enum ELoadType { WAIT_FOR_LOAD = 0, DONT_WAIT_FOR_LOAD = 1 };
struct MapInfo { std::string LevelName; UInt64 LevStamp, TngStamp; };
struct Call { long map; std::string topology, navigation, thing; ELoadType type; };
static std::map<std::string, UInt64> g_Stamps;
static std::vector<Call> g_Calls;
static std::string GetThingFilenameFromLevelFilename(const std::string& level) { return level.substr(0, level.size() - 4) + ".tng"; }
static UInt64 GetFileModifiedTime(const std::string& name) { return g_Stamps[name]; }
static void LoadLevelFromFiles(long map, const std::string& t, const std::string& n, const std::string& th, ELoadType type)
{ Call c; c.map = map; c.topology = t; c.navigation = n; c.thing = th; c.type = type; g_Calls.push_back(c); }
static void LoadLevel(std::vector<MapInfo>& maps, long map, ELoadType loadType)
{
    std::string thingFilename = GetThingFilenameFromLevelFilename(maps[map].LevelName);
    std::string levelFile = maps[map].LevelName;
    LoadLevelFromFiles(map, levelFile, levelFile, thingFilename, loadType);
    maps[map].LevStamp = GetFileModifiedTime(maps[map].LevelName);
    maps[map].TngStamp = GetFileModifiedTime(thingFilename);
}
int main()
{
    std::vector<MapInfo> maps(2); maps[1].LevelName = "town.lev"; maps[1].LevStamp = maps[1].TngStamp = 0;
    UInt64 big = 287; big = (big << 32) | 0x5AB5A0CBu;  // 0x11F5AB5A0CB
    g_Stamps["town.lev"] = big; g_Stamps["town.tng"] = 42;
    LoadLevel(maps, 1, DONT_WAIT_FOR_LOAD);
    int ok = g_Calls.size() == 1 && g_Calls[0].topology == "town.lev" && g_Calls[0].navigation == "town.lev"
          && g_Calls[0].thing == "town.tng" && g_Calls[0].type == DONT_WAIT_FOR_LOAD
          && maps[1].LevStamp == big && maps[1].TngStamp == 42;
    printf(ok ? "00502620_TEST PASS\n" : "00502620_TEST FAIL\n");
    return ok ? 0 : 1;
}