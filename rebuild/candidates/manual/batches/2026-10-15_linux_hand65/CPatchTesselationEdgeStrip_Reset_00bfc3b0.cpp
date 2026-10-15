// CPatchTesselationEdgeStrip::Reset(long startPos, long size, bool vertical, CMovableResource* resource) -- size all four vertex arrays for size+1 vertices, then record the start/size, clear Tesselated and set Vertical
// Leaf: Reset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFC3B0, length 88 bytes (+ int3 padding).
// Callees: CEdgeStipVertexArray<T>::Resize(long, CMovableResource*) for CVertex @ 0x00BFBE40, CWaterVertex @ 0x00BFC040,
// CFanBaseVertex @ 0x00BFBF40, CWaterFanBaseVertex @ 0x00BFC140 (their bool results ignored).
// Layout: Ego_r.pdb CPatchTesselationEdgeStrip (0x2c bytes, no engine header): StartPos +4, Size +6, Verts +8, FanBaseVerts
// +0x10, WaterVerts +0x18, WaterFanBaseVerts +0x20, Tesselated +0x28, Vertical +0x29; vtable 0x012A8318.
// Shape: `size + 1` computed once into edi and the resource kept in ebx for the four calls (Verts, WaterVerts,
// FanBaseVerts, WaterFanBaseVerts -- the source order); the size is a long argument stored as a word (`mov [esi+6],bp`),
// the start position loaded as a word, the vertical flag as a byte; `ret 10h`.
struct CMovableResource;

template <class T>
struct CEdgeStipVertexArray {
    T*   Data;                                     // +0x00
    long Count;                                    // +0x04
    bool Resize(long count, CMovableResource* resource);
};

struct CPatchTesselationEdgeStrip {                // vtable 0x012A8318 (CBase-derived), 0x2c bytes
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
    void Reset(long startPos, long size, bool vertical, CMovableResource* resource);
};

void CPatchTesselationEdgeStrip::Reset(long startPos, long size, bool vertical, CMovableResource* resource)
{
    Verts.Resize(size + 1, resource);
    WaterVerts.Resize(size + 1, resource);
    FanBaseVerts.Resize(size + 1, resource);
    WaterFanBaseVerts.Resize(size + 1, resource);
    Size = (unsigned short)size;
    StartPos = (unsigned short)startPos;
    Tesselated = false;
    Vertical = vertical;
}
