// Test for CEngineMap::~CEngineMap (0x00B42A90): CloseStaticMap, the two conditional DeleteMap calls (only when the map has the
// matching sub-map), FileBlockManager.Reset, then the members in reverse order (manager, async entry release, map name).
#include <cstdio>
#include <cstring>
static char g_log[64]; static int g_n = 0;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
struct Info { unsigned int RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteEntry(void*) { Log('E'); }
struct Name { ~Name() { Log('N'); } };
struct Counted {
    void* Data; Info* PInfo;
    ~Counted()
    {
        if (PInfo) { --PInfo->RefCount; if (PInfo->RefCount == 0) { PInfo->DeleteFunc(PInfo->Data); delete PInfo; } }
        Data = 0; PInfo = 0;
    }
};
struct Manager { void Reset() { Log('R'); } ~Manager() { Log('M'); } };
struct Map;
struct Renderer { void DeleteMap(Map*) { Log('L'); } };
struct Generator { void DeleteMap(Map*) { Log('D'); } };
static Renderer g_renderer; static Generator g_generator;
struct Map {
    void* LandscapeMap; void* LocalDetailCacheMap;
    Name StaticMapName;
    Counted AsyncFileEntry;
    Manager FileBlockManager;
    void CloseStaticMap() { Log('C'); }
    ~Map()
    {
        CloseStaticMap();
        if (LandscapeMap) g_renderer.DeleteMap(this);
        if (LocalDetailCacheMap) g_generator.DeleteMap(this);
        FileBlockManager.Reset();
    }
};
int main()
{
    int x = 0;
    {
        Map m; m.LandscapeMap = &x; m.LocalDetailCacheMap = 0; m.AsyncFileEntry.Data = &x;
        m.AsyncFileEntry.PInfo = new Info; m.AsyncFileEntry.PInfo->RefCount = 1; m.AsyncFileEntry.PInfo->DeleteFunc = DeleteEntry;
        m.AsyncFileEntry.PInfo->Data = &x;
    }
    if (std::strcmp(g_log, "CLRMEN") != 0) { std::printf("00b42a90_TEST FAIL\n"); return 1; }
    g_n = 0; g_log[0] = 0;
    {
        Map m; m.LandscapeMap = 0; m.LocalDetailCacheMap = &x; m.AsyncFileEntry.Data = 0; m.AsyncFileEntry.PInfo = 0;
    }
    if (std::strcmp(g_log, "CDRMN") != 0) { std::printf("00b42a90_TEST FAIL\n"); return 1; }
    std::printf("00b42a90_TEST PASS\n");
    return 0;
}