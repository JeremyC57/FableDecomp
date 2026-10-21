#include <cstdio>
#include <cstring>
// behaviour test: CEngineWorldMap::AutoBuildWorldMapGrid builds a 32-aligned grid around the maps and links each map
struct CEngineMap { unsigned short MapWidth, MapHeight, WorldPosX, WorldPosY; };
static int g_links = 0; static long g_lastIndex = -1;
template <class T> struct CArray {
    T* m_pBegin; T* m_pEnd; T* m_pCapacity;
    unsigned long Size() const { return (unsigned long)(m_pEnd - m_pBegin); }
    T& operator[](unsigned long i) { return m_pBegin[i]; }
};
struct CEngineWorldMap {
    short* Map; long MapArrayWidth, MapArrayHeight, MapOffsetX, MapOffsetY;
    CArray<CEngineMap*> MapList;
    void LinkMapToWorld(CEngineMap* map, long index) { ++g_links; g_lastIndex = index; }
    void AutoBuildWorldMapGrid();
};
void CEngineWorldMap::AutoBuildWorldMapGrid()
{
    if (Map != 0) return;
    long minX = 0x7fffffff, minY = 0x7fffffff, maxX = 0, maxY = 0;
    bool found = false;
    for (unsigned long i = 0; i < MapList.Size(); ++i) {
        CEngineMap* map = MapList[i];
        if (map) {
            long x = map->WorldPosX;
            if (minX > x) minX = x;
            long y = map->WorldPosY;
            if (minY > y) minY = y;
            if (maxX < x + map->MapWidth) maxX = x + map->MapWidth;
            if (maxY < y + map->MapHeight) maxY = y + map->MapHeight;
            found = true;
        }
    }
    if (!found) return;
    long cellX = minX / 32 - 1, cellY = minY / 32 - 1;
    MapOffsetX = cellX * 32;
    MapOffsetY = cellY * 32;
    MapArrayWidth = (maxX + 31) / 32 - cellX + 1;
    MapArrayHeight = (maxY + 31) / 32 - cellY + 1;
    Map = new short[MapArrayWidth * MapArrayHeight];
    std::memset(Map, -1, MapArrayWidth * MapArrayHeight * 2);
    for (unsigned long j = 0; j < MapList.Size(); ++j)
        if (MapList[j]) LinkMapToWorld(MapList[j], j);
}
int main()
{
    CEngineMap a = { 64, 32, 100, 200 }, b = { 32, 32, 300, 40 };
    CEngineMap* list[3] = { 0, &a, &b };
    CEngineWorldMap w; w.Map = 0; w.MapList.m_pBegin = list; w.MapList.m_pEnd = list + 3; w.MapList.m_pCapacity = list + 3;
    w.AutoBuildWorldMapGrid();
    // minX 100 -> cell 2 (=3-1), minY 40 -> cell 0; maxX 332 -> 11, maxY 232 -> 8
    bool ok = w.Map != 0 && w.MapOffsetX == 64 && w.MapOffsetY == 0 && w.MapArrayWidth == 11 - 2 + 1
        && w.MapArrayHeight == 8 - 0 + 1 && w.Map[0] == -1 && w.Map[w.MapArrayWidth * w.MapArrayHeight - 1] == -1
        && g_links == 2 && g_lastIndex == 2;
    short* old = w.Map;
    w.AutoBuildWorldMapGrid();
    ok = ok && w.Map == old && g_links == 2;
    delete[] w.Map;
    CEngineMap* empty[2] = { 0, 0 };
    CEngineWorldMap e; e.Map = 0; e.MapList.m_pBegin = empty; e.MapList.m_pEnd = empty + 2; e.MapList.m_pCapacity = empty + 2;
    e.AutoBuildWorldMapGrid();
    ok = ok && e.Map == 0;
    if (!ok) { std::printf("00b40070_TEST FAIL\n"); return 1; }
    std::printf("00b40070_TEST PASS\n");
    return 0;
}
