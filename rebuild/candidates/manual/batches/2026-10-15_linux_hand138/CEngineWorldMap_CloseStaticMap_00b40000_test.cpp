// Behaviour test for 0x00b40000 CEngineWorldMap::CloseStaticMap
#include <cstdio>
static int g_mapCloses = 0, g_waterCloses = 0, g_bankCloses = 0;
struct CEngineMap { int m_closed; void CloseStaticMap() { ++m_closed; ++g_mapCloses; } };
struct CEngineWaterRenderer { void CloseStaticMap() { ++g_waterCloses; } };
static CEngineWaterRenderer* g_pEngineWaterRenderer = 0;
struct CStaticMapBankFile { virtual void Close() { ++g_bankCloses; } };
template <class T> struct CArray { T* m_pBegin; T* m_pEnd; T* m_pCapacity;
    unsigned long Size() const { return m_pEnd - m_pBegin; } T& operator[](unsigned long i) { return m_pBegin[i]; } };
struct CEngineWorldMap {
    CArray<CEngineMap*> MapList; CStaticMapBankFile StaticMapFile; long StaticMapOpenState; long StaticMapBuildStatus;
    void CloseStaticMap()
    {
        if (StaticMapOpenState == 0) return;
        for (unsigned long i = 1; i < MapList.Size(); ++i) { CEngineMap* map = MapList[i]; if (map) map->CloseStaticMap(); }
        if (g_pEngineWaterRenderer) g_pEngineWaterRenderer->CloseStaticMap();
        StaticMapFile.Close();
        StaticMapOpenState = 0; StaticMapBuildStatus = 0;
    }
};
int main()
{
    CEngineMap m0 = { 0 }, m1 = { 0 }, m3 = { 0 }; CEngineMap* maps[4] = { &m0, &m1, 0, &m3 }; CEngineWaterRenderer water;
    CEngineWorldMap w; w.MapList.m_pBegin = maps; w.MapList.m_pEnd = w.MapList.m_pCapacity = maps + 4;
    w.StaticMapOpenState = 0; w.StaticMapBuildStatus = 5;
    w.CloseStaticMap();
    bool ok = g_mapCloses == 0 && g_bankCloses == 0 && w.StaticMapBuildStatus == 5;   // closed: nothing happens
    g_pEngineWaterRenderer = &water; w.StaticMapOpenState = 2;
    w.CloseStaticMap();
    ok = ok && m0.m_closed == 0 && m1.m_closed == 1 && m3.m_closed == 1 && g_waterCloses == 1 && g_bankCloses == 1
            && w.StaticMapOpenState == 0 && w.StaticMapBuildStatus == 0;
    if (!ok) { std::printf("00b40000_TEST FAIL\n"); return 1; }
    std::printf("00b40000_TEST PASS\n");
    return 0;
}
