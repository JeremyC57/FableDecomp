// CPatchTesselationEdgeStrip::SetMovableResourceHandler(CMovableResource* handler) -- register `handler` as the relocation handler of each allocated vertex-array block with the landscape memory manager, in member order
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFC510, length 115 bytes (+ int3 padding).
// Callees: the landscape memory manager's per-block handler setter @ 0x00BD9AB0 (engine global 0x01436EA8 -> +0x6b0;
// the catalog names 0x00BD9AB0 "$E2"), two arguments (block, handler).
// Layout: Ego_r.pdb CPatchTesselationEdgeStrip (0x2c bytes, vtable 0x012A8318; same model as the landed Reset 0x00BFC3B0).
// Shape: the handler kept in edi, one `if (Data) manager->SetHandler(Data, handler)` per array (Verts, FanBaseVerts,
// WaterVerts, WaterFanBaseVerts), the vertex array's inline member; `ret 4`.
struct CMovableResource;
struct CEngineLandscapeMemoryManager {
    void SetMovableResourceHandler(void* block, CMovableResource* handler);   // 0x00BD9AB0
};
struct SEngine_01436ea8 {
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* m_pLandscapeMemory;   // +0x6b0
};
extern SEngine_01436ea8* g_pEngine_01436ea8;      // 0x01436EA8

template <class T>
struct CEdgeStipVertexArray {
    T*   Data;                                     // +0x00
    long Count;                                    // +0x04
    void SetMovableResourceHandler(CMovableResource* handler)
    {
        if (Data)
            g_pEngine_01436ea8->m_pLandscapeMemory->SetMovableResourceHandler(Data, handler);
    }
};

struct CPatchTesselationEdgeStrip {                // vtable 0x012A8318 (CBase-derived), 0x2c bytes
    struct CVertex;
    struct CFanBaseVertex;
    struct CWaterVertex;
    struct CWaterFanBaseVertex;
    virtual void v00();
    unsigned short StartPos;                                            // +0x04
    unsigned short Size;                                                // +0x06
    CEdgeStipVertexArray<CVertex>             Verts;                    // +0x08
    CEdgeStipVertexArray<CFanBaseVertex>      FanBaseVerts;             // +0x10
    CEdgeStipVertexArray<CWaterVertex>        WaterVerts;               // +0x18
    CEdgeStipVertexArray<CWaterFanBaseVertex> WaterFanBaseVerts;        // +0x20
    bool Tesselated;                                                    // +0x28
    bool Vertical;                                                      // +0x29
    void SetMovableResourceHandler(CMovableResource* handler);
};

void CPatchTesselationEdgeStrip::SetMovableResourceHandler(CMovableResource* handler)
{
    Verts.SetMovableResourceHandler(handler);
    FanBaseVerts.SetMovableResourceHandler(handler);
    WaterVerts.SetMovableResourceHandler(handler);
    WaterFanBaseVerts.SetMovableResourceHandler(handler);
}