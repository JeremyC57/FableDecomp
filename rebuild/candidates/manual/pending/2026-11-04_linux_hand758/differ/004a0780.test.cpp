// Behaviour test for CWorld::SaveGameStateManual (0x004A0780), g++ model of the save branch and the bookkeeping.
#include <cstdio>
#include <string>
#include <vector>
struct World {
    bool betweenScreens, sourceExists, serialisingNonPersistent;
    int mostRecentSaveType; std::string mostRecentManualName;
    std::vector<std::string> log;
    void SaveGameStateManual(const std::string& name)
    {
        if (betweenScreens) {
            std::string path = "profile/" + name;
            std::string source = "autosave";
            if (sourceExists) log.push_back("copy " + source + " " + path);
            serialisingNonPersistent = false;
            log.push_back("hero " + name + (serialisingNonPersistent ? " np" : ""));
            serialisingNonPersistent = true;
        } else
            log.push_back("game " + name);
        mostRecentSaveType = 1;
        mostRecentManualName = name;
    }
};
int main()
{
    World w = { true, true, true, 0, "", std::vector<std::string>() };
    w.SaveGameStateManual("slot1");
    int ok = w.log.size() == 2 && w.log[0] == "copy autosave profile/slot1" && w.log[1] == "hero slot1" && w.serialisingNonPersistent
          && w.mostRecentSaveType == 1 && w.mostRecentManualName == "slot1";
    World v = { true, false, true, 0, "", std::vector<std::string>() }; v.SaveGameStateManual("s2");
    ok = ok && v.log.size() == 1 && v.log[0] == "hero s2";
    World u = { false, true, true, 0, "", std::vector<std::string>() }; u.SaveGameStateManual("s3");
    ok = ok && u.log.size() == 1 && u.log[0] == "game s3" && u.mostRecentManualName == "s3";
    std::printf(ok ? "004a0780_TEST PASS\n" : "004a0780_TEST FAIL\n");
    return ok ? 0 : 1;
}
