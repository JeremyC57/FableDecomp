// CNavQuadTree::GetNodeAt(const C2DVector& pos, unsigned long layer) const
// Grade: DIFFER(496v533) -- retail_asm candidate: the C++ cast stand-in replaced by the landed __forceinline inline-asm
//   helper (FloatToLong / FloorToLong: bare dword fistp after the frame store/reload) with the Retail-asm evidence line;
//   105 diff lines from 165; residue: surrounding x87 scheduling, frame slots and inlining extent
// Retail-asm: 0x00A771A6 `fistp dword ptr [ebp + 8]` (fld / fsub 0.5 / fistp / fild / fadd 1.0 / compare / jne / inc, per cell coordinate) -- the engine's inline-assembly floor helper (GFFloatToLongFloor 0x0048D940; the landed FloorToLong of CThingPhysical::ModifyHealth 0x006A68F0); VC7.1 emits no bare dword fistp from C++.
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

// The engine's inline-assembly FPU floor helper (standalone copy GFFloatToLongFloor 0x0048D940; the same __forceinline
// FloorToLong as the landed CThingPhysical::ModifyHealth 0x006A68F0 / CStatBar::DrawStatBarOnLeft 0x00646010).
__forceinline long FloorToLong(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
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
