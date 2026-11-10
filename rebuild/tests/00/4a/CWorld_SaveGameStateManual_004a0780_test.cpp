// Behaviour test for CWorld::SaveGameStateManual (0x004A0780): the same control flow over a host model.
#include <cstdio>
#include <cstring>
#include <string>

static bool g_betweenScreens;
static bool g_exists[2];                     // [0] plain autosave, [1] quest-start autosave
static std::string g_copyFrom, g_copyTo, g_heroSaved, g_fullSaved;
static bool g_serialisingDuringHeroSave;

struct World {
    bool SerialisingNonPersistentQuestItems;
    int MostRecentSaveType, MostRecentSaveTypeBeforeManualSave;
    std::string MostRecentManualSaveName;
    void SaveGameStateManual(const std::string& name)
    {
        if (g_betweenScreens) {
            std::string path = "profile/" + name;
            if (MostRecentSaveTypeBeforeManualSave == 2) {
                std::string source = "profile/AutoSave";
                if (g_exists[0]) { g_copyFrom = source; g_copyTo = path; }
            } else {
                std::string source = "profile/AutoSave.qs";
                if (g_exists[1]) { g_copyFrom = source; g_copyTo = path; }
            }
            SerialisingNonPersistentQuestItems = false;
            g_heroSaved = name; g_serialisingDuringHeroSave = SerialisingNonPersistentQuestItems;
            SerialisingNonPersistentQuestItems = true;
        } else
            g_fullSaved = name;
        MostRecentSaveType = 1;
        MostRecentManualSaveName = name;
    }
};

int main()
{
    int fails = 0;
    World w; w.SerialisingNonPersistentQuestItems = true; w.MostRecentSaveType = 0; w.MostRecentSaveTypeBeforeManualSave = 2;
    w.SaveGameStateManual("Slot1");                       // outside the quest screens: full save
    if (g_fullSaved != "Slot1" || !g_heroSaved.empty() || w.MostRecentSaveType != 1 || w.MostRecentManualSaveName != "Slot1") ++fails;
    g_betweenScreens = true; g_exists[0] = true;
    w.SaveGameStateManual("Slot2");                       // plain autosave copied, hero state only
    if (g_copyFrom != "profile/AutoSave" || g_copyTo != "profile/Slot2" || g_heroSaved != "Slot2" || g_serialisingDuringHeroSave) ++fails;
    if (!w.SerialisingNonPersistentQuestItems || w.MostRecentManualSaveName != "Slot2") ++fails;
    w.MostRecentSaveTypeBeforeManualSave = 3; g_copyFrom = "";
    w.SaveGameStateManual("Slot3");                       // quest-start save missing: no copy
    if (!g_copyFrom.empty() || g_heroSaved != "Slot3") ++fails;
    g_exists[1] = true;
    w.SaveGameStateManual("Slot4");
    if (g_copyFrom != "profile/AutoSave.qs" || g_copyTo != "profile/Slot4") ++fails;
    if (fails) { std::printf("004a0780_TEST FAIL %d\n", fails); return 1; }
    std::printf("004a0780_TEST PASS\n");
    return 0;
}