// CEngineWorldMap::AutoBuildWorldMapGrid() -- when no grid exists yet, take the bounding box (in world cells) of every loaded map, build a 32-cell aligned grid of map indices around it (filled with -1) and link every map into it
// Leaf: AutoBuildWorldMapGrid
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B40070, length 368 bytes (with int3 padding; body 0x169), plain `ret`.
// Callees: operator new[] @ 0x00BFEB22 (MSVCR71 import), CEngineWorldMap::LinkMapToWorld(CEngineMap*, long) @ 0x00B3EDF0.
// Layout: engine/CEngineWorldMap.h names (Map +0x0c, MapArrayWidth +0x10, MapArrayHeight +0x14, MapOffsetX +0x18,
//   MapOffsetY +0x1c, MapList +0x20) with the retail 12-byte CArray (as the landed CloseStaticMap 0x00B40000); the header
//   flattens MapList to bytes, so the class is modelled locally. CEngineMap is the PDB layout less 4 (retail MapWidth
//   +0x5c, MapHeight +0x5e, WorldPosX +0x60, WorldPosY +0x62).
// Shape: signed `/ 32` (cdq ; and 1Fh ; add ; sar 5); the min/max scan keeps max X/Y in ebp/ebx and the found flag in dl;
//   the grid is cleared with an inline memset(-1) of width*height shorts after the members are re-read; the count is
//   re-derived from the CArray each pass.
extern "C" void* __cdecl memset(void* dest, int value, unsigned int size);
#pragma intrinsic(memset)
void* __cdecl operator new[](unsigned int size);       // 0x00BFEB22

struct CEngineMap {
    unsigned char  _pad00[0x5c];
    unsigned short MapWidth;                     // +0x5c
    unsigned short MapHeight;                    // +0x5e
    unsigned short WorldPosX;                    // +0x60
    unsigned short WorldPosY;                    // +0x62
};

template <class T> struct CArray {               // retail CArray: begin, end, capacity
    T* m_pBegin;
    T* m_pEnd;
    T* m_pCapacity;
    unsigned long Size() const { return m_pEnd - m_pBegin; }
    T& operator[](unsigned long i) { return m_pBegin[i]; }
};

struct CEngineWorldMap {
    void*               _vptr;                   // +0x00
    unsigned char       _pad04[0x8];
    short*              Map;                     // +0x0c
    long                MapArrayWidth;           // +0x10
    long                MapArrayHeight;          // +0x14
    long                MapOffsetX;              // +0x18
    long                MapOffsetY;              // +0x1c
    CArray<CEngineMap*> MapList;                 // +0x20
    void LinkMapToWorld(CEngineMap* map, long index);   // 0x00B3EDF0
    void AutoBuildWorldMapGrid();
};

void CEngineWorldMap::AutoBuildWorldMapGrid()
{
    if (Map != 0)
        return;
    long minX = 0x7fffffff;
    long minY = 0x7fffffff;
    long maxX = 0;
    long maxY = 0;
    bool found = false;
    for (unsigned long i = 0; i < MapList.Size(); ++i) {
        CEngineMap* map = MapList[i];
        if (map) {
            long x = map->WorldPosX;
            if (minX > x)
                minX = x;
            long y = map->WorldPosY;
            if (minY > y)
                minY = y;
            if (maxX < x + map->MapWidth)
                maxX = x + map->MapWidth;
            if (maxY < y + map->MapHeight)
                maxY = y + map->MapHeight;
            found = true;
        }
    }
    if (!found)
        return;
    long cellX = minX / 32 - 1;
    long cellY = minY / 32 - 1;
    MapOffsetX = cellX * 32;
    MapOffsetY = cellY * 32;
    MapArrayWidth = (maxX + 31) / 32 - cellX + 1;
    MapArrayHeight = (maxY + 31) / 32 - cellY + 1;
    Map = (short*)operator new[](MapArrayWidth * MapArrayHeight * 2);
    memset(Map, -1, MapArrayWidth * MapArrayHeight * 2);
    for (unsigned long j = 0; j < MapList.Size(); ++j) {
        if (MapList[j])
            LinkMapToWorld(MapList[j], j);
    }
}
