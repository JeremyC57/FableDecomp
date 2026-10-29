// CPatchTesselationEdgeStrip::CPatchTesselationEdgeStrip(unsigned short startPos, long size, bool vertical, CMovableResource* resource) -- CBase, vtable, four empty vertex arrays, record the start/size, size every array for size+1 vertices, then set Vertical and clear Tesselated
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFC2E0, length 137 bytes (+ int3 padding).
// Leaf: ??0CPatchTesselationEdgeStrip
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0; CEdgeStipVertexArray<T>::Resize(long, CMovableResource*) for CVertex @ 0x00BFBE40,
// CWaterVertex @ 0x00BFC040, CFanBaseVertex @ 0x00BFBF40, CWaterFanBaseVertex @ 0x00BFC140 (bool results ignored).
// Layout: Ego_r.pdb CPatchTesselationEdgeStrip (0x2c bytes, vtable 0x012A8318; same model as the landed Reset 0x00BFC3B0).
// Shape: the arrays' inline constructors zero +8..+0x24 with one register after the vtable store; Size then StartPos are
// body assignments (as initialisers they would store before the vtable store); the body then repeats Reset's resize order (Verts, WaterVerts,
// FanBaseVerts, WaterFanBaseVerts) with size+1 kept in edi, then Vertical and Tesselated; `ret 10h`.
class CMovableResource;

class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                       // 0x0099A2F0
};

template <class T>
class CEdgeStipVertexArray { public:
    T*   Data;                                     // +0x00
    long Count;                                    // +0x04
    CEdgeStipVertexArray() : Data(0), Count(0) { }
    bool Resize(long count, CMovableResource* resource);
};

class CPatchTesselationEdgeStrip : public CBaseClassNonCopyable { public:        // vtable 0x012A8318, 0x2c bytes
    struct CVertex;
    struct CFanBaseVertex;
    struct CWaterVertex;
    struct CWaterFanBaseVertex;
    unsigned short StartPos;                                            // +0x04
    unsigned short Size;                                                // +0x06
    CEdgeStipVertexArray<CVertex>             Verts;                    // +0x08
    CEdgeStipVertexArray<CFanBaseVertex>      FanBaseVerts;             // +0x10
    CEdgeStipVertexArray<CWaterVertex>        WaterVerts;               // +0x18
    CEdgeStipVertexArray<CWaterFanBaseVertex> WaterFanBaseVerts;        // +0x20
    bool Tesselated;                                                    // +0x28
    bool Vertical;                                                      // +0x29
    CPatchTesselationEdgeStrip(unsigned short startPos, long size, bool vertical, CMovableResource* resource);
};

CPatchTesselationEdgeStrip::CPatchTesselationEdgeStrip(unsigned short startPos, long size, bool vertical, CMovableResource* resource)
{
    Size = (unsigned short)size;
    StartPos = startPos;
    Verts.Resize(size + 1, resource);
    WaterVerts.Resize(size + 1, resource);
    FanBaseVerts.Resize(size + 1, resource);
    WaterFanBaseVerts.Resize(size + 1, resource);
    Vertical = vertical;
    Tesselated = false;
}