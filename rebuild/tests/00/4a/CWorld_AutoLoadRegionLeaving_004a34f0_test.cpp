// Behaviour test for CWorld::AutoLoadRegionLeaving (0x004A34F0): save the region-leaving state, delete the quest-start autosave and
// (when present) its .hs hero-state companion, then queue the region-leaving autosave through LoadGameState(name, false).
#include <stdio.h>
#include <string>
#include <set>

static std::set<std::string> g_Files;          // files on disk
static int g_Saves = 0;
static std::string g_Loaded;

class CAFile { public: static bool PathExists(const std::string& p) { return g_Files.count(p) != 0; } };
class CDriveManager { public: void DeleteFileW(const std::string& p) { g_Files.erase(p); } };
static CDriveManager g_Drives;

class CWorld { public:
    bool AutoSaveRegionLeaving() { ++g_Saves; return true; }
    bool LoadGameState(const std::string& name, bool loadNow) { if (loadNow) return false; g_Loaded = name; return true; }
    bool AutoLoadRegionLeaving()
    {
        AutoSaveRegionLeaving();
        std::string questStart = "U:/profile/AutoSave.qs";
        if (CAFile::PathExists(questStart)) {
            g_Drives.DeleteFileW(questStart);
            std::string heroState = questStart + ".hs";
            if (CAFile::PathExists(heroState))
                g_Drives.DeleteFileW(heroState);
        }
        std::string name = "AutoSave.rl";
        return LoadGameState(name, false);
    }
};

int main()
{
    int fails = 0;
    CWorld w;
    g_Files.insert("U:/profile/AutoSave.qs"); g_Files.insert("U:/profile/AutoSave.qs.hs"); g_Files.insert("other");
    if (!w.AutoLoadRegionLeaving() || g_Saves != 1 || g_Files.size() != 1 || g_Loaded != "AutoSave.rl") ++fails;
    g_Files.clear(); g_Files.insert("U:/profile/AutoSave.qs"); g_Loaded = "";
    if (!w.AutoLoadRegionLeaving() || g_Saves != 2 || !g_Files.empty() || g_Loaded != "AutoSave.rl") ++fails;
    g_Files.clear(); g_Files.insert("U:/profile/AutoSave.qs.hs");      // no quest-start save: the .hs file is left alone
    if (!w.AutoLoadRegionLeaving() || g_Files.size() != 1) ++fails;
    if (fails) { printf("004a34f0_TEST FAIL %d\n", fails); return 1; }
    printf("004a34f0_TEST PASS\n");
    return 0;
}