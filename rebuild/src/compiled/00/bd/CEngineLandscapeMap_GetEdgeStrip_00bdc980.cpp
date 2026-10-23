//   (`for (testX += 32; !other && testX < x + length; testX += 32)`) instead of a fresh walkX local: one variable for the
//   probe and the walk keeps testX in ebx for the whole body (retail `add ebx,20h` in place), which puts the map in ebp and
//   side in ecx and settles the whole colouring. The side-0/1 walk keeps its own walkY (walking testY in place gives 548v556).
// CEngineLandscapeMap::GetEdgeStrip(long x, long y, long length, LANDSCAPE_EDGE_STRIP_SIDE side) -- find the tesselation edge strip on the given side of a landscape span: when the probe cell lies inside this map, ask its background tree; otherwise find the neighbouring map through the world map grid (walking along the edge in 32-unit cells when the first cell is empty), clip the span to that map and recurse into its landscape
// Leaf: GetEdgeStrip
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDC980, length 543 bytes + the 4-entry switch table (`ret 10h`).
// Callees: CLandscapeBackgroundTreeNode::GetEdgeStrip @ 0x00BF6930 (landed). Globals: the world map @ 0x01436E8C.
// Layout: CEngineLandscapeMap: background tree +0x04, owning CEngineMap +0x14; CEngineMap: landscape +0x00, MapWidth/Height
//   +0x5c/+0x5e, WorldPosX/Y +0x60/+0x62; CEngineWorldMap grid as the landed AutoBuildWorldMapGrid 0x00B40070.
// Shape: the self call on the neighbour's landscape is a tail call VC turns into a loop (`jmp` to the top with the new this);
//   the grid lookup is an inline PeekMap (cell index by `>> 5`, a short map index > 0 selects MapList[index]); the switch
//   groups sides 0/1 (walk along y) and 2/3 (walk along x).
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
extern CEngineWorldMap* g_pEngineWorldMap;         // 0x01436E8C

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