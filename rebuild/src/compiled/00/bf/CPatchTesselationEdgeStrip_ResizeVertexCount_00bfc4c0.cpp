// CPatchTesselationEdgeStrip::ResizeVertexCount(long verts, long fanBaseVerts, long waterVerts, long waterFanBaseVerts, CMovableResource* resource) -- resize each of the four vertex arrays to its own count
// Leaf: ResizeVertexCount
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFC4C0, length 69 bytes (+ int3 padding).
// Callees: CEdgeStipVertexArray<T>::Resize(long, CMovableResource*) for CVertex @ 0x00BFBE40, CFanBaseVertex @ 0x00BFBF40,
// CWaterVertex @ 0x00BFC040, CWaterFanBaseVertex @ 0x00BFC140 (their bool results ignored).
// Layout: Ego_r.pdb CPatchTesselationEdgeStrip (0x2c bytes, no engine header): Verts +8, FanBaseVerts +0x10, WaterVerts
// +0x18, WaterFanBaseVerts +0x20; vtable 0x012A8318.
// Shape: the resource kept in edi for all four calls, each count reloaded from its slot right before its call (the
// first one loaded ahead of the register saves), the arrays in layout order; `ret 14h`.
class CMovableResource;

template <class T>
class CEdgeStipVertexArray { public:
    T*   Data;                                     // +0x00
    long Count;                                    // +0x04
    bool Resize(long count, CMovableResource* resource);
};

class CPatchTesselationEdgeStrip { public:                // vtable 0x012A8318 (CBase-derived), 0x2c bytes
    struct CVertex;
    struct CFanBaseVertex;
    struct CWaterVertex;
    struct CWaterFanBaseVertex;
    virtual void v00();
    unsigned short StartPos;                                            // +0x04
    unsigned short Size;                                                // +0x06
    CEdgeStipVertexArray<CVertex>             Verts;                    // +0x08  Resize @ 0x00BFBE40
    CEdgeStipVertexArray<CFanBaseVertex>      FanBaseVerts;             // +0x10  Resize @ 0x00BFBF40
    CEdgeStipVertexArray<CWaterVertex>        WaterVerts;               // +0x18  Resize @ 0x00BFC040
    CEdgeStipVertexArray<CWaterFanBaseVertex> WaterFanBaseVerts;        // +0x20  Resize @ 0x00BFC140
    bool Tesselated;                                                    // +0x28
    bool Vertical;                                                      // +0x29
    void ResizeVertexCount(long verts, long fanBaseVerts, long waterVerts, long waterFanBaseVerts, CMovableResource* resource);
};

void CPatchTesselationEdgeStrip::ResizeVertexCount(long verts, long fanBaseVerts, long waterVerts, long waterFanBaseVerts, CMovableResource* resource)
{
    Verts.Resize(verts, resource);
    FanBaseVerts.Resize(fanBaseVerts, resource);
    WaterVerts.Resize(waterVerts, resource);
    WaterFanBaseVerts.Resize(waterFanBaseVerts, resource);
}