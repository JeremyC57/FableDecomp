// Behaviour test for CWorld::AutoLoadRespawn (0x004A5E90) with the same-TU LoadGameState (0x004A3200 shape) and
// AutoLoadInternal: between a quest's start and finish screens a present checkpoint save is queued, else the most recent manual
// save, else (quest-start save type, between the screens) the quest-start autosave, else the plain autosave; a queued load sets
// the load status to 3 and returns true.
#include <stdio.h>
#include <string>
#include <set>

static std::set<std::string> g_Files;          // full paths that exist on disk
static bool g_AnyDirectory = false;            // the console flag 0x013B8646
static bool g_BetweenScreens = false;

class CAFile { public:
    static bool PathExists(const std::string& p) { return g_Files.count(p) != 0; }
    static bool IsPathFile(const std::string& p) { return g_Files.count(p) != 0; }
};
static std::string ProfilePath(const std::string& name) { return name.empty() ? std::string("") : "P:/" + name; }

class CWorld { public:
    long SaveGameLoadStatus;
    std::string SaveGamePathName;
    long MostRecentSaveType;
    std::string MostRecentManualSaveName;
    CWorld() : SaveGameLoadStatus(0), MostRecentSaveType(0) {}
    std::string GetPathNameForFileName(const std::string& name) const { return g_Files.count("D:/" + name) ? "D:/" + name : ""; }
    bool LoadGameState(const std::string& name, bool loadNow)
    {
        if (SaveGameLoadStatus == 0 || loadNow) {
            bool found = false;
            std::string path = ProfilePath(name);
            if (path != "")
                found = true;
            if (g_AnyDirectory) {
                if (found) {
                    SaveGameLoadStatus = 1;
                    SaveGamePathName = path;
                    return true;
                }
            } else if (found && CAFile::PathExists(path) && CAFile::IsPathFile(path)) {
                SaveGameLoadStatus = 1;
                SaveGamePathName = path;
                return true;
            } else {
                std::string fullPath = GetPathNameForFileName(name);
                if (fullPath != "") {
                    SaveGameLoadStatus = 1;
                    SaveGamePathName = fullPath;
                    return true;
                }
            }
        }
        return false;
    }
    bool AutoLoadInternal(const std::string& name, bool loadNow) { return LoadGameState(name, loadNow); }
    bool AutoLoadRespawn()
    {
        if (g_BetweenScreens) {
            std::string checkPointPath = ProfilePath("cp");
            if (CAFile::PathExists(checkPointPath)) {
                if (AutoLoadInternal("cp", false)) { SaveGameLoadStatus = 3; return true; }
                return false;
            }
        }
        if (MostRecentSaveType == 1) {
            if (AutoLoadInternal(MostRecentManualSaveName, false)) { SaveGameLoadStatus = 3; return true; }
            return false;
        }
        if (MostRecentSaveType == 3 && g_BetweenScreens) {
            if (AutoLoadInternal("qs", false)) { SaveGameLoadStatus = 3; return true; }
            return false;
        }
        if (AutoLoadInternal("auto", false)) { SaveGameLoadStatus = 3; return true; }
        return false;
    }
};

int main()
{
    int fails = 0;
    { CWorld w; g_BetweenScreens = true; g_Files.insert("P:/cp");
      if (!w.AutoLoadRespawn() || w.SaveGameLoadStatus != 3 || w.SaveGamePathName != "P:/cp") ++fails; }
    { CWorld w; g_BetweenScreens = false; g_Files.clear(); g_Files.insert("P:/manual"); w.MostRecentSaveType = 1; w.MostRecentManualSaveName = "manual";
      if (!w.AutoLoadRespawn() || w.SaveGamePathName != "P:/manual") ++fails; }
    { CWorld w; g_Files.clear(); w.MostRecentSaveType = 1; w.MostRecentManualSaveName = "gone";      // not found anywhere
      if (w.AutoLoadRespawn() || w.SaveGameLoadStatus != 0) ++fails; }
    { CWorld w; g_Files.clear(); g_Files.insert("D:/auto");                                        // found by the directory scan
      if (!w.AutoLoadRespawn() || w.SaveGamePathName != "D:/auto" || w.SaveGameLoadStatus != 3) ++fails; }
    { CWorld w; g_Files.clear(); g_AnyDirectory = true; g_BetweenScreens = true; w.MostRecentSaveType = 3; // any-directory load
      if (!w.AutoLoadRespawn() || w.SaveGamePathName != "P:/qs") ++fails; g_AnyDirectory = false; }
    { CWorld w; w.SaveGameLoadStatus = 1;                                                        // a load already pending
      if (w.AutoLoadRespawn()) ++fails; }
    if (fails) { printf("004a5e90_TEST FAIL %d\n", fails); return 1; }
    printf("004a5e90_TEST PASS\n");
    return 0;
}