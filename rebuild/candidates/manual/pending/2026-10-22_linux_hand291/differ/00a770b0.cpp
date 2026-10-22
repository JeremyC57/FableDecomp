// Grade: DIFFER(408v533) -- lane gap (inline-asm fistp rounding helper): retail rounds each cell coordinate with an inlined `fistp (f - 0.5)` plus a float-bit `== i + 1` bump, which compiler-emitted C++ can only reach through _ftol; the body here is the behavioural reconstruction (bounds check, cell index, blocked-node fill-in, node slot 11 recursion) with a C++ floor helper
// CNavQuadTree::GetNodeAt(const C2DVector& pos, unsigned long layer) const
// Leaf: GetNodeAt
#pragma pack(push, 1)
struct C2DVector {
    float X, Y;
};
#pragma pack(pop)

struct CNavNode {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual CNavNode* GetNodeAt(const C2DVector& pos);   // slot 11 (+0x2c)
};

struct CBlockedNavNode : CNavNode {
    C2DVector Pos;          // +0x04
    bool Flag;              // +0x0c
    unsigned char Layer;    // +0x0d
};
extern CBlockedNavNode g_BlockedNavNode;   // 0x013D2D2C

struct CNavigationLayer {
    CNavNode** Nodes;
    unsigned char _pad[0x24];
};

static inline long FloorToLong(float f)
{
    long i = (long)(f - 0.5f);
    float next = (float)i + 1.0f;
    if (*(long*)&f == *(long*)&next)
        ++i;
    return i;
}

#pragma pack(push, 1)
struct CNavQuadTree {
    void* vfptr;                        // +0x00
    bool Loaded;                        // +0x04
    bool Loaded2;                       // +0x05
    bool Loaded3;                       // +0x06
    C2DVector MapPos;                   // +0x07
    unsigned char _pad_0x0f;
    float MapWidth;                     // +0x10
    float MapHeight;                    // +0x14
    unsigned char _pad_0x18[4];
    CNavigationLayer* LayersBegin;      // +0x1c
    CNavigationLayer* LayersEnd;        // +0x20
    CNavNode* GetNodeAt(const C2DVector& pos, unsigned long layer) const;
};
#pragma pack(pop)

CNavNode* CNavQuadTree::GetNodeAt(const C2DVector& pos, unsigned long layer) const
{
    static float invCellSize = 1.0f / 32.0f;
    if (!Loaded2 && !Loaded3)
        return 0;
    C2DVector minPos = MapPos;
    C2DVector maxPos;
    maxPos.X = MapWidth + MapPos.X;
    maxPos.Y = MapHeight + MapPos.Y;
    if (layer >= (unsigned long)(LayersEnd - LayersBegin) || pos.X < minPos.X || !(pos.X <= maxPos.X)
        || pos.Y < minPos.Y || !(pos.Y <= maxPos.Y))
        return 0;
    float dy = pos.Y - MapPos.Y;
    unsigned long ix = FloorToLong(invCellSize * (pos.X - MapPos.X));
    unsigned long iy = FloorToLong(invCellSize * dy);
    long width = FloorToLong(invCellSize * MapWidth);
    CNavNode* node = LayersBegin[layer].Nodes[width * iy + ix];
    if (node == &g_BlockedNavNode) {
        g_BlockedNavNode.Pos.X = (float)ix * 32.0f + MapPos.X + 16.0f;
        g_BlockedNavNode.Pos.Y = (float)iy * 32.0f + MapPos.Y + 16.0f;
        g_BlockedNavNode.Flag = false;
        g_BlockedNavNode.Layer = (unsigned char)layer;
        return &g_BlockedNavNode;
    }
    return node->GetNodeAt(pos);
}
