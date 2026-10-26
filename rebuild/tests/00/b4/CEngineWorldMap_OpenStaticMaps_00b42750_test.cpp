#include <cstdio>
// behaviour test: CEngineWorldMap::OpenStaticMaps -- development state opens every map, retail state opens the bank
static int g_grids = 0, g_progress = 0, g_opens = 0, g_lastState = -1, g_water = 0, g_pool = 0, g_mapFile = 0;
static bool g_bankOpens = true; static long g_entry = 0;
class CStaticMapBankFile { public:
    bool Open(int, unsigned long flags) { return flags == 0x62 && g_bankOpens; }
    void SetMemoryPool() { ++g_pool; }
    long FindEntry() { return g_entry; }
};
class CEngineMap { public: void OpenDevelopmentStaticMap(CStaticMapBankFile&, int state) { ++g_opens; g_lastState = state; } };
class CEngineWorldMap { public:
    CEngineMap* Maps[3]; unsigned long Count;
    int StaticMapBankName;
    CStaticMapBankFile StaticMapFile;
    int StaticMapOpenState;
    void AutoBuildWorldMapGrid() { ++g_grids; }
    void OpenMapFile(long) { ++g_mapFile; }
    void OpenStaticMaps(int state);
};
void CEngineWorldMap::OpenStaticMaps(int state)
{
    if (StaticMapOpenState == state) return;
    AutoBuildWorldMapGrid();
    if (state == 2) {
        for (unsigned long i = 0; i < Count; ++i)
            if (Maps[i]) { ++g_progress; Maps[i]->OpenDevelopmentStaticMap(StaticMapFile, state); }
    }
    else {
        bool opened = StaticMapFile.Open(StaticMapBankName, 0x62);
        StaticMapFile.SetMemoryPool();
        if (!opened) return;
        ++g_water;
        long entry = StaticMapFile.FindEntry();
        if (entry > 0) OpenMapFile(entry);
        else {
            for (unsigned long i = 0; i < Count; ++i)
                if (Maps[i]) { ++g_progress; Maps[i]->OpenDevelopmentStaticMap(StaticMapFile, state); }
        }
    }
    StaticMapOpenState = state;
}
int main()
{
    CEngineMap m;
    CEngineWorldMap w; w.Maps[0] = 0; w.Maps[1] = &m; w.Maps[2] = &m; w.Count = 3; w.StaticMapOpenState = 0;
    w.OpenStaticMaps(2);
    bool ok = g_grids == 1 && g_opens == 2 && g_lastState == 2 && w.StaticMapOpenState == 2;
    w.OpenStaticMaps(2);
    ok = ok && g_grids == 1;
    g_bankOpens = false;
    w.OpenStaticMaps(1);
    ok = ok && g_pool == 1 && w.StaticMapOpenState == 2 && g_water == 0;
    g_bankOpens = true; g_entry = 5;
    w.OpenStaticMaps(1);
    ok = ok && g_mapFile == 1 && w.StaticMapOpenState == 1 && g_opens == 2;
    g_entry = 0;
    w.OpenStaticMaps(0);
    ok = ok && g_opens == 4 && g_lastState == 0 && w.StaticMapOpenState == 0;
    if (!ok) { std::printf("00b42750_TEST FAIL\n"); return 1; }
    std::printf("00b42750_TEST PASS\n");
    return 0;
}