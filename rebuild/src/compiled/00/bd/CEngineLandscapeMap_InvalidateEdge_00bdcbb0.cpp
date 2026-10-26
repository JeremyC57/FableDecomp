// CEngineLandscapeMap::InvalidateEdge(long x, long y, long length, long direction) -- invalidate a landscape edge span: parts of the span that fall outside this map are forwarded (cell by cell through the world map grid) to the neighbouring maps' landscapes, the in-map remainder goes to this map's background tree; directions 0/1 run along y (1 probes the cell to the left), 2/3 along x (3 probes the cell above)
// Leaf: InvalidateEdge
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDCBB0, length 1328 bytes (`ret 10h`, no EH frame).
// Callees: CLandscapeBackgroundTreeNode::InvalidateEdge @ 0x00BF67F0, itself (neighbour landscapes; the self calls on `this` are
//   tail calls VC7.1 turns into the loop back to the top). Globals: the world map @ 0x01436E8C.
// Layout: as the landed sibling CEngineLandscapeMap::GetEdgeStrip 0x00BDC980 (background tree +0x04, owning CEngineMap +0x14;
//   CEngineMap MapWidth/Height +0x5c/+0x5e, WorldPosX/Y +0x60/+0x62; the inline world-map PeekMap).
// Shape: for directions 0/1 the outside walk has no return, so it falls into the in-map clipping exactly as retail does
//   (retail reaches the y < WorldPosY block after the walk); the 2/3 walk returns. The world map global is read at each
//   PeekMap (retail reloads it after every call).
class CLandscapeBackgroundTreeNode { public:
    void InvalidateEdge(long x, long y, long length, long direction);   // 0x00BF67F0
};
class CEngineLandscapeMap;
class CEngineMap { public:
    CEngineLandscapeMap* Landscape;                // +0x00
    unsigned char  _pad04[0x58];
    unsigned short MapWidth;                       // +0x5c
    unsigned short MapHeight;                      // +0x5e
    unsigned short WorldPosX;                      // +0x60
    unsigned short WorldPosY;                      // +0x62
};
template <class T> class CArray { public:
    T* m_pBegin;
    T* m_pEnd;
    T* m_pCapacity;
    T& operator[](unsigned long i) { return m_pBegin[i]; }
};
class CEngineWorldMap { public:
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

class CEngineLandscapeMap { public:
    void*                         _vptr;           // +0x00
    CLandscapeBackgroundTreeNode* BackgroundTree;  // +0x04
    unsigned char                 _pad08[0xc];
    CEngineMap*                   Map;             // +0x14
    void InvalidateEdge(long x, long y, long length, long direction);
};

void CEngineLandscapeMap::InvalidateEdge(long x, long y, long length, long direction)
{
    if (direction == 0 || direction == 1) {
        long testX = x;
        if (direction == 1)
            --testX;
        if (testX < Map->WorldPosX || testX >= Map->WorldPosX + Map->MapWidth
            || y + length <= Map->WorldPosY || y >= Map->WorldPosY + Map->MapHeight) {
            for (long walkY = y; walkY < y + length; ) {
                CEngineMap* other = g_pEngineWorldMap->PeekMap(testX, walkY);
                if (other) {
                    if (other->Landscape) {
                        long count = y - walkY + length;
                        if (other->MapHeight < count)
                            count = other->MapHeight;
                        other->Landscape->InvalidateEdge(x, walkY, count, direction);
                    }
                    walkY += other->MapHeight;
                } else {
                    walkY += 32;
                }
            }
        }
        if (y < Map->WorldPosY) {
            CEngineMap* other = g_pEngineWorldMap->PeekMap(testX, y);
            if (other && other->Landscape)
                other->Landscape->InvalidateEdge(x, y, Map->WorldPosY - y, direction);
            InvalidateEdge(x, Map->WorldPosY, length + (y - Map->WorldPosY), direction);
            return;
        }
        if (y + length > Map->WorldPosY + Map->MapHeight) {
            CEngineMap* other = g_pEngineWorldMap->PeekMap(testX, Map->WorldPosY + Map->MapHeight);
            if (other && other->Landscape)
                other->Landscape->InvalidateEdge(x, Map->WorldPosY + Map->MapHeight, y + length - (Map->WorldPosY + Map->MapHeight), direction);
            InvalidateEdge(x, y, Map->WorldPosY + Map->MapHeight - y, direction);
            return;
        }
    } else {
        long testY = y;
        if (direction == 3)
            --testY;
        if (testY < Map->WorldPosY || testY >= Map->WorldPosY + Map->MapHeight
            || x + length <= Map->WorldPosX || x >= Map->WorldPosX + Map->MapWidth) {
            for (long walkX = x; walkX < x + length; ) {
                CEngineMap* other = g_pEngineWorldMap->PeekMap(walkX, testY);
                if (other) {
                    if (other->Landscape) {
                        long count = x - walkX + length;
                        if (other->MapWidth < count)
                            count = other->MapWidth;
                        other->Landscape->InvalidateEdge(walkX, y, count, direction);
                    }
                    walkX += other->MapWidth;
                } else {
                    walkX += 32;
                }
            }
            return;
        }
        if (x < Map->WorldPosX) {
            CEngineMap* other = g_pEngineWorldMap->PeekMap(x, testY);
            if (other && other->Landscape)
                other->Landscape->InvalidateEdge(x, y, Map->WorldPosX - x, direction);
            InvalidateEdge(Map->WorldPosX, y, length + (x - Map->WorldPosX), direction);
            return;
        }
        if (x + length > Map->WorldPosX + Map->MapWidth) {
            CEngineMap* other = g_pEngineWorldMap->PeekMap(Map->WorldPosX + Map->MapWidth, testY);
            if (other && other->Landscape)
                other->Landscape->InvalidateEdge(Map->WorldPosX + Map->MapWidth, y, x + length - (Map->WorldPosX + Map->MapWidth), direction);
            InvalidateEdge(x, y, Map->WorldPosX + Map->MapWidth - x, direction);
            return;
        }
    }
    if (BackgroundTree)
        BackgroundTree->InvalidateEdge(x, y, length, direction);
}