// CLandscapeBackgroundPatch::UpdateTesselation() -- when landscape tesselation is enabled, fill a CTesselationData record for this patch (world origin, size, map, index buffer, the four edge strips and the bridge buffers) and tesselate it: water-only patches through the water sub-patch; otherwise through the engine's landscape tesselator, taking back the bridge buffers and poly counts it produced, then the water sub-patch if any; on success mark all four edge strips tesselated
// Leaf: UpdateTesselation
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE61B0, 573 bytes (+ int3 padding).
// Callees: CTesselationData ctor (inline; out-of-line copy 0x00BE59C0, base CBase ctor 0x0099A2D0, vtable 0x012A800C) and dtor
//   (CBase dtor 0x0099A2E0); CLandscapeTesselatorBase::TesselatePatch 0x00BE3910 on the engine global 0x01436EA8 (+0x66C);
//   CEngineWaterBackgroundSubPatch::TesselatePatch(CTesselationData&) 0x00BFD290; enable flag global 0x0139C40C.
// Layout: CLandscapeBackgroundPatch and CTesselationData as Ego_r.pdb (Parent +4, EdgeStrips[4] +8 stride 0x2c with Tesselated
//   +0x28, IndexBuffer +0xbc, BridgeIndexBuffers +0xc4, BridgeVertexBuffers +0xd4, BridgePolyCount +0xe4, WaterSubPatch +0xf4,
//   MapX..Height +0x10c..+0x112, WaterOnlyPatch +0x11a; CTesselationData 0x5c bytes). The parent tree node's +0xA4 pointer
//   reaches the landscape map whose +0x14 engine map holds the world-map origin (+0x60 X, +0x62 Y), as in RenderWithOcclusion.
// Shape: the world origin goes through inline GetWorldX/GetWorldY accessors that re-walk Parent->Map->EngineMap (a named
//   origin sum or a plain argument expression schedules the MapX/MapY loads ahead of the map); the record fill and the
//   bridge-buffer take-back are four-trip loops that VC7.1 unrolls (index buffers before vertex buffers).
struct CEngineMap { unsigned char _pad[0x60]; unsigned short OriginX, OriginY; };   // +0x60 / +0x62
struct CIndexBuffer;
struct CVertexBufferWin32;
struct CBase {
    virtual ~CBase();                               // 0x0099A2E0
    CBase();                                        // 0x0099A2D0
};
struct CPatchTesselationEdgeStrip {                 // 0x2c bytes
    unsigned char _pad[0x28];
    bool          Tesselated;                       // +0x28
    bool          Vertical;                         // +0x29
    unsigned char _pad2a[2];
};
struct CTesselationData : CBase {                   // vtable 0x012A800C, 0x5c bytes
    long                        PatchWorldX;        // +0x04
    long                        PatchWorldY;        // +0x08
    long                        Width;              // +0x0c
    long                        Height;             // +0x10
    CEngineMap*                 EngineMap;          // +0x14
    CIndexBuffer*               IndexBuffer;        // +0x18
    CPatchTesselationEdgeStrip* EdgeStrips[4];      // +0x1c
    CVertexBufferWin32*         BridgeVertexBuffers[4];   // +0x2c
    CIndexBuffer*               BridgeIndexBuffers[4];    // +0x3c
    long                        BridgePolyCount[4]; // +0x4c
    CTesselationData(CEngineMap* map, long worldX, long worldY, unsigned short width, unsigned short height)
        : PatchWorldX(worldX), PatchWorldY(worldY), Width(width), Height(height), EngineMap(map) { }
};
struct CLandscapeTesselatorBase {
    bool TesselatePatch(CTesselationData* data, void* resource);   // 0x00BE3910
};
struct CEngineGlobal { unsigned char _pad[0x66c]; CLandscapeTesselatorBase Tesselator; };   // +0x66C
extern CEngineGlobal* g_pEngine;                    // 0x01436EA8
extern bool g_bLandscapeTesselation;                // 0x0139C40C
struct CEngineWaterBackgroundSubPatch {
    bool TesselatePatch(CTesselationData& data);    // 0x00BFD290
};
struct CLandscapeMap { unsigned char _pad[0x14]; CEngineMap* EngineMap; };   // +0x14
struct CLandscapeBackgroundTreeNode { unsigned char _pad[0xa4]; CLandscapeMap* Map; };   // +0xA4
struct CLandscapeBackgroundPatch {
    void*                           vptr;
    CLandscapeBackgroundTreeNode*   Parent;             // +0x04
    CPatchTesselationEdgeStrip      EdgeStrips[4];      // +0x08
    void*                           Texture;            // +0xb8
    CIndexBuffer*                   IndexBuffer;        // +0xbc
    CVertexBufferWin32*             VertexBuffer;       // +0xc0
    CIndexBuffer*                   BridgeIndexBuffers[4];    // +0xc4
    CVertexBufferWin32*             BridgeVertexBuffers[4];   // +0xd4
    unsigned short                  BridgePolyCount[4]; // +0xe4
    void*                           UVLockTreeU;        // +0xec
    void*                           UVLockTreeV;        // +0xf0
    CEngineWaterBackgroundSubPatch* WaterSubPatch;      // +0xf4
    float                           MinZ;               // +0xf8
    float                           MaxZ;               // +0xfc
    long                            ZSorterHandle;      // +0x100
    unsigned short                  VertexCount;        // +0x104
    unsigned short                  PolyCountIncDegenerates;   // +0x106
    unsigned short                  TextureWidthNeeded; // +0x108
    unsigned short                  TextureHeightNeeded;   // +0x10a
    unsigned short                  MapX;               // +0x10c
    unsigned short                  MapY;               // +0x10e
    unsigned short                  Width;              // +0x110
    unsigned short                  Height;             // +0x112
    unsigned short                  LockedGridSpacingX; // +0x114
    unsigned short                  LockedGridSpacingY; // +0x116
    unsigned char                   LODBand;            // +0x118
    bool                            SharedIndexBuffer;  // +0x119
    bool                            WaterOnlyPatch;     // +0x11a
    bool                            IsDefaultSection;   // +0x11b
    long GetWorldX() const { return Parent->Map->EngineMap->OriginX + MapX; }
    long GetWorldY() const { return Parent->Map->EngineMap->OriginY + MapY; }
    void UpdateTesselation();
};

void CLandscapeBackgroundPatch::UpdateTesselation()
{
    if (!g_bLandscapeTesselation)
        return;
    CEngineMap* map = Parent->Map->EngineMap;
    CTesselationData data(map, GetWorldX(), GetWorldY(), Width, Height);
    data.IndexBuffer = IndexBuffer;
    for (long i = 0; i < 4; ++i) {
        data.EdgeStrips[i] = &EdgeStrips[i];
        data.BridgeIndexBuffers[i] = BridgeIndexBuffers[i];
        data.BridgeVertexBuffers[i] = BridgeVertexBuffers[i];
        data.BridgePolyCount[i] = BridgePolyCount[i];
    }
    bool tesselated;
    if (WaterOnlyPatch) {
        tesselated = WaterSubPatch->TesselatePatch(data);
    } else {
        tesselated = g_pEngine->Tesselator.TesselatePatch(&data, this);
        for (long i = 0; i < 4; ++i) {
            BridgeIndexBuffers[i] = data.BridgeIndexBuffers[i];
            BridgeVertexBuffers[i] = data.BridgeVertexBuffers[i];
            BridgePolyCount[i] = (unsigned short)data.BridgePolyCount[i];
            data.BridgeIndexBuffers[i] = 0;
            data.BridgeVertexBuffers[i] = 0;
        }
        data.IndexBuffer = 0;
        if (tesselated && WaterSubPatch)
            tesselated = WaterSubPatch->TesselatePatch(data);
    }
    if (tesselated) {
        for (long i = 0; i < 4; ++i)
            EdgeStrips[i].Tesselated = true;
    }
}
