// Behaviour test for CThingManager::LoadLoadableGlobalThingsFromFile @ 0x005223F0: the control flow and argument plumbing.
#include <cstdio>

class CAFile { public: int refs; };
class CThing { public: int id; };
struct CThingVector { CThing* items[8]; int count; CThingVector() : count(0) {} };
struct CSectionSet { int count; CSectionSet() : count(0) {} void insert() { ++count; } };

class CWorldMap { public:
    int sectionsAsked;
    CSectionSet GetLoadableSections() const { CSectionSet s; s.count = 3; ++const_cast<CWorldMap*>(this)->sectionsAsked; return s; }
};

class CThingManager { public:
    CWorldMap* WorldMap;
    int GlobalThingLoadingBehaviour;
    int loadCalls, activateCalls, lastSections, lastMode, activatedCount;
    long lastVersion;
    CThing pool[2];
    void LoadFromFile(CAFile* file, long version, CSectionSet names, CThingVector& things, void* process, int mode)
    {
        ++loadCalls; lastVersion = version; lastSections = names.count; lastMode = mode + (process ? 100 : 0);
        ++file->refs;
        things.items[things.count++] = &pool[0];
        things.items[things.count++] = &pool[1];
    }
    void ActivateThings(const CThingVector& things) { ++activateCalls; activatedCount = things.count; }
    void LoadLoadableGlobalThingsFromFile(CAFile* file, long version)
    {
        if (GlobalThingLoadingBehaviour == 1) {
            CSectionSet sections;
            sections.insert();
            CWorldMap* worldMap = WorldMap;
            {
                CThingVector things;
                LoadFromFile(file, version, worldMap->GetLoadableSections(), things, 0, 2);
                ActivateThings(things);
            }
        }
    }
};

int main()
{
    CWorldMap map = { 0 };
    CThingManager m;
    m.WorldMap = &map; m.loadCalls = m.activateCalls = 0; m.activatedCount = 0;
    CAFile f = { 1 };
    m.GlobalThingLoadingBehaviour = 0;
    m.LoadLoadableGlobalThingsFromFile(&f, 7);
    bool ok = m.loadCalls == 0 && m.activateCalls == 0 && map.sectionsAsked == 0;
    m.GlobalThingLoadingBehaviour = 1;
    m.LoadLoadableGlobalThingsFromFile(&f, 7);
    ok = ok && m.loadCalls == 1 && m.activateCalls == 1 && m.lastVersion == 7 && m.lastSections == 3 && m.lastMode == 2 &&
         m.activatedCount == 2 && map.sectionsAsked == 1;
    if (!ok) { std::printf("005223f0_TEST FAIL\n"); return 1; }
    std::printf("005223f0_TEST PASS\n");
    return 0;
}