// 00bdc980 behaviour test: CEngineLandscapeMap::GetEdgeStrip
#ifndef _MSC_VER
#define __forceinline inline
#include <stdint.h>
#endif
#include <stdio.h>
#include <string.h>
struct CPatchTesselationEdgeStrip;
enum LANDSCAPE_EDGE_STRIP_SIDE { EDGE_SIDE_0, EDGE_SIDE_1, EDGE_SIDE_2, EDGE_SIDE_3 };

struct CLandscapeBackgroundTreeNode {
    CPatchTesselationEdgeStrip* GetEdgeStrip(long x, long y, LANDSCAPE_EDGE_STRIP_SIDE side);   // 0x00BF6930
};
struct CEngineLandscapeMap;
struct CEngineMap {
    CEngineLandscapeMap* Landscape;                // +0x00
    unsigned char  _pad04[0x58];
    unsigned short MapWidth;                       // +0x5c
    unsigned short MapHeight;                      // +0x5e
    unsigned short WorldPosX;                      // +0x60
    unsigned short WorldPosY;                      // +0x62
};
template <class T> struct CArray {
    T* m_pBegin;
    T* m_pEnd;
    T* m_pCapacity;
    T& operator[](unsigned long i) { return m_pBegin[i]; }
};
struct CEngineWorldMap {
    void*               _vptr;                     // +0x00
    unsigned char       _pad04[0x8];
    short*              Map;                       // +0x0c
    long                MapArrayWidth;             // +0x10
    long                MapArrayHeight;            // +0x14
    long                MapOffsetX;                // +0x18
    long                MapOffsetY;                // +0x1c
    CArray<CEngineMap*> MapList;                   // +0x20
    __forceinline CEngineMap* PeekMap(long x, long y)
    {
        long cellX = x - MapOffsetX;
        long cellY = y - MapOffsetY;
        if (cellX >= 0 && cellY >= 0 && cellX < MapArrayWidth * 32 && cellY < MapArrayHeight * 32) {
            long index = Map[(cellY >> 5) * MapArrayWidth + (cellX >> 5)];
            if (index > 0)
                return MapList[index];
        }
        return 0;
    }
};
CEngineWorldMap* g_pEngineWorldMap;

struct CEngineLandscapeMap {
    void*                         _vptr;           // +0x00
    CLandscapeBackgroundTreeNode* BackgroundTree;  // +0x04
    unsigned char                 _pad08[0xc];
    CEngineMap*                   Map;             // +0x14
    CPatchTesselationEdgeStrip* GetEdgeStrip(long x, long y, long length, LANDSCAPE_EDGE_STRIP_SIDE side);
};

CPatchTesselationEdgeStrip* CEngineLandscapeMap::GetEdgeStrip(long x, long y, long length, LANDSCAPE_EDGE_STRIP_SIDE side)
{
    CEngineWorldMap* world = g_pEngineWorldMap;
    long testX = x;
    long testY = y;
    if (side == EDGE_SIDE_1)
        --testX;
    else if (side == EDGE_SIDE_3)
        --testY;
    if (testX >= Map->WorldPosX && testX < Map->WorldPosX + Map->MapWidth
        && testY >= Map->WorldPosY && testY < Map->WorldPosY + Map->MapHeight) {
        if (!BackgroundTree)
            return 0;
        return BackgroundTree->GetEdgeStrip(x, y, side);
    }
    CEngineMap* other = world->PeekMap(testX, testY);
    if (!other) {
        switch (side) {
        case EDGE_SIDE_2:
        case EDGE_SIDE_3:
            for (testX += 32; !other && testX < x + length; testX += 32)
                other = world->PeekMap(testX, testY);
            if (!other)
                return 0;
            return other->Landscape->GetEdgeStrip(other->WorldPosX, y, length + (x - other->WorldPosX), side);
        case EDGE_SIDE_0:
        case EDGE_SIDE_1:
            for (long walkY = testY + 32; !other && walkY < y + length; walkY += 32)
                other = world->PeekMap(testX, walkY);
            if (!other)
                return 0;
            return other->Landscape->GetEdgeStrip(x, other->WorldPosY, length + (y - other->WorldPosY), side);
        }
    }
    return other->Landscape->GetEdgeStrip(x, y, length, side);
}

static long g_lastX, g_lastY, g_lastSide, g_calls;
CPatchTesselationEdgeStrip* CLandscapeBackgroundTreeNode::GetEdgeStrip(long x, long y, LANDSCAPE_EDGE_STRIP_SIDE side)
{
    g_lastX = x; g_lastY = y; g_lastSide = side; ++g_calls;
    return (CPatchTesselationEdgeStrip*)this;
}
static CLandscapeBackgroundTreeNode trees[6];
static CEngineMap maps[6];
static CEngineLandscapeMap lands[6];
static short grid[16];
static CEngineMap* mapList[8];
static CEngineWorldMap world;
static int fails;
static void setmap(int i, int wx, int wy, int w, int h, bool tree)
{
    memset(&maps[i], 0, sizeof(maps[i]));
    memset(&lands[i], 0, sizeof(lands[i]));
    maps[i].Landscape = &lands[i];
    maps[i].WorldPosX = (unsigned short)wx; maps[i].WorldPosY = (unsigned short)wy;
    maps[i].MapWidth = (unsigned short)w; maps[i].MapHeight = (unsigned short)h;
    lands[i].Map = &maps[i];
    lands[i].BackgroundTree = tree ? &trees[i] : 0;
    mapList[i] = &maps[i];
}
static void check(const char* name, CPatchTesselationEdgeStrip* got, int tree, long x, long y, long side)
{
    bool ok;
    if (tree < 0)
        ok = got == 0;
    else
        ok = got == (CPatchTesselationEdgeStrip*)&trees[tree] && g_lastX == x && g_lastY == y && g_lastSide == side;
    if (!ok) { ++fails; printf("FAIL %s got=%p last=(%ld,%ld,%ld)\n", name, (void*)got, g_lastX, g_lastY, g_lastSide); }
}
int main()
{
    // 4x4 grid of 32-unit cells; A=1 (0..63,0..63), B=2 (64..127,0..63), C=3 (96..127,64..95),
    // D=4 (32..63,96..127), E=5 (0..31,96..127, no tree)
    setmap(1, 0, 0, 64, 64, true);
    setmap(2, 64, 0, 64, 64, true);
    setmap(3, 96, 64, 32, 32, true);
    setmap(4, 32, 96, 32, 32, true);
    setmap(5, 0, 96, 32, 32, false);
    short g[16] = { 1, 1, 2, 2,
                    1, 1, 2, 2,
                    0, 0, 0, 3,
                    5, 4, 0, 0 };
    memcpy(grid, g, sizeof(grid));
    world.Map = grid; world.MapArrayWidth = 4; world.MapArrayHeight = 4;
    world.MapOffsetX = 0; world.MapOffsetY = 0;
    world.MapList.m_pBegin = mapList; world.MapList.m_pEnd = mapList + 6; world.MapList.m_pCapacity = mapList + 8;
    g_pEngineWorldMap = &world;

    check("inside", lands[1].GetEdgeStrip(10, 10, 5, EDGE_SIDE_0), 1, 10, 10, 0);
    check("side1 dec stays", lands[1].GetEdgeStrip(64, 10, 5, EDGE_SIDE_1), 1, 64, 10, 1);
    check("neighbour", lands[1].GetEdgeStrip(64, 10, 5, EDGE_SIDE_0), 2, 64, 10, 0);
    check("side3 dec", lands[2].GetEdgeStrip(80, 64, 5, EDGE_SIDE_3), 2, 80, 64, 3);
    check("walk x", lands[1].GetEdgeStrip(64, 70, 64, EDGE_SIDE_2), 3, 96, 70, 2);
    check("walk y", lands[1].GetEdgeStrip(40, 64, 64, EDGE_SIDE_0), 4, 40, 96, 0);
    check("walk y none", lands[1].GetEdgeStrip(80, 64, 32, EDGE_SIDE_1), -1, 0, 0, 0);
    check("walk x short", lands[1].GetEdgeStrip(64, 70, 32, EDGE_SIDE_3), -1, 0, 0, 0);
    check("outside", lands[1].GetEdgeStrip(0, 10, 5, EDGE_SIDE_1), -1, 0, 0, 0);
    g_calls = 0;
    check("no tree", lands[1].GetEdgeStrip(10, 100, 5, EDGE_SIDE_0), -1, 0, 0, 0);
    if (g_calls != 0) { ++fails; printf("FAIL no tree called\n"); }
    check("walk y to no tree", lands[1].GetEdgeStrip(10, 64, 64, EDGE_SIDE_1), -1, 0, 0, 0);
    if (fails == 0)
        printf("00bdc980_TEST PASS\n");
    return fails;
}