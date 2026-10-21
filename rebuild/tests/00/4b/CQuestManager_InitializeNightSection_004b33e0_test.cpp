#include <cstdio>
#include <cstring>
// Behaviour model of CQuestManager::InitializeNightSection (0x004b33e0): already active -> nothing; otherwise mark active,
// collect the load sections whose "%..." suffix is "NightOnly" (a sorted, duplicate-free set) and load those sections.
struct SectionSet {
    const char* items[16];
    int count;
    SectionSet() : count(0) {}
    void Insert(const char* value)
    {
        int i = 0;
        while (i < count && std::strcmp(items[i], value) < 0) ++i;
        if (i < count && std::strcmp(items[i], value) == 0) return;
        for (int j = count; j > i; --j) items[j] = items[j - 1];
        items[i] = value; ++count;
    }
};
static const char* Suffix(const char* section)
{
    const char* p = std::strchr(section, '%');
    return p ? p + 1 : "";
}
struct WorldMap {
    int loads;
    SectionSet loaded;
    void LoadThingsWithSectionNames(const SectionSet& sections, bool) { ++loads; loaded = sections; }
};
struct QuestManager {
    WorldMap* Map;
    SectionSet LoadSections;
    bool DayActive, NightActive;
    void BuildLoadSections(SectionSet* out) { *out = LoadSections; }
    void InitializeNightSection()
    {
        if (NightActive)
            return;
        NightActive = true;
        SectionSet sections;
        BuildLoadSections(&sections);
        SectionSet subset;
        for (int i = 0; i < sections.count; ++i) {
            bool match = std::strcmp(Suffix(sections.items[i]), "NightOnly") == 0;
            if (match)
                subset.Insert(sections.items[i]);
        }
        Map->LoadThingsWithSectionNames(subset, false);
    }
};
int main()
{
    WorldMap map; map.loads = 0;
    QuestManager qm; qm.Map = &map; qm.DayActive = false; qm.NightActive = false;
    qm.LoadSections.Insert("Bowerstone");
    qm.LoadSections.Insert("Bowerstone%DayOnly");
    qm.LoadSections.Insert("Bowerstone%NightOnly");
    qm.LoadSections.Insert("Oakvale%DayOnly");
    qm.LoadSections.Insert("Oakvale%NightOnly");
    qm.LoadSections.Insert("Oakvale%DayOnlyX");
    qm.InitializeNightSection();
    bool ok = qm.NightActive && !qm.DayActive && map.loads == 1 && map.loaded.count == 2
        && std::strcmp(map.loaded.items[0], "Bowerstone%NightOnly") == 0 && std::strcmp(map.loaded.items[1], "Oakvale%NightOnly") == 0;
    // second call: already active, nothing reloaded
    qm.InitializeNightSection();
    ok = ok && map.loads == 1;
    if (!ok) { std::printf("004b33e0_TEST FAIL\n"); return 1; }
    std::printf("004b33e0_TEST PASS\n");
    return 0;
}