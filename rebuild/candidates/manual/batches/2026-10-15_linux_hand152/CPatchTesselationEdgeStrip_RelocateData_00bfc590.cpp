// CPatchTesselationEdgeStrip::RelocateData(void* oldBase, void* newBase) -- CMovableResource relocation callback: the first of the four vertex arrays whose data pointer equals oldBase is re-based onto newBase (true), none -> false
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFC590, length 109 bytes (+ int3 padding). No callees.
// Layout: Ego_r.pdb CPatchTesselationEdgeStrip (0x2c bytes, no engine header; same model as the landed Reset
// 0x00BFC3B0): Verts +8, FanBaseVerts +0x10, WaterVerts +0x18, WaterFanBaseVerts +0x20, each {T* Data; long Count}.
// Shape: four identical arms in member order, each `mov eax,Data ; cmp old,eax ; jne next ; test ; je ; Data - old + new ;
// mov al,1 ; ret 8`: the vertex array's own inline bool RelocateData (Data read into a local first, then compared with
// oldBase -- comparing the member directly loads oldBase first), called in member order with an early `return true`.
struct CPatchTesselationEdgeStrip;

template <class T>
struct CEdgeStipVertexArray {
    T*   Data;                                     // +0x00
    long Count;                                    // +0x04
    bool RelocateData(void* oldBase, void* newBase)
    {
        T* data = Data;
        if (oldBase != (void*)data)
            return false;
        if (data)
            Data = (T*)((char*)data - (char*)oldBase + (long)newBase);
        return true;
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
    bool RelocateData(void* oldBase, void* newBase);
};

bool CPatchTesselationEdgeStrip::RelocateData(void* oldBase, void* newBase)
{
    if (Verts.RelocateData(oldBase, newBase))
        return true;
    if (FanBaseVerts.RelocateData(oldBase, newBase))
        return true;
    if (WaterVerts.RelocateData(oldBase, newBase))
        return true;
    if (WaterFanBaseVerts.RelocateData(oldBase, newBase))
        return true;
    return false;
}
