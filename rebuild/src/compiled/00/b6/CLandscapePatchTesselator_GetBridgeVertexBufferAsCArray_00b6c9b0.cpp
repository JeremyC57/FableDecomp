// CLandscapePatchTesselator::GetBridgeVertexBufferAsCArray() -- the bridge vertex vector at +0x14 (24-byte vertices) viewed as a C array: its first element when it holds any, else the vector object itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6C9B0, length 48 bytes.
// Shape: the vector's address in esi, `size()` is the signed pointer difference divided by 24 (the 0x2AAAAAAB multiply and `sar edx,2`), `ret`.
struct SBridgeVertex_00b6c9b0 {                    // 24 bytes: position + normal (the size divide is by 24: magic 0x2AAAAAAB, `sar edx,2`)
    float x, y, z;
    float nx, ny, nz;
};

template<class T> struct CRetailVector {           // VC7.1 Dinkumware vector: first / last / end
    T* m_first;
    T* m_last;
    T* m_end;
    long size() const { return m_last - m_first; }
};

struct CLandscapePatchTesselator {
    unsigned char                        _pad00[0x14];
    CRetailVector<SBridgeVertex_00b6c9b0> m_bridgeVertices;   // +0x14
    SBridgeVertex_00b6c9b0* GetBridgeVertexBufferAsCArray();
};

SBridgeVertex_00b6c9b0* CLandscapePatchTesselator::GetBridgeVertexBufferAsCArray()
{
    CRetailVector<SBridgeVertex_00b6c9b0>* vertices = &m_bridgeVertices;
    return vertices->size() != 0 ? vertices->m_first : (SBridgeVertex_00b6c9b0*)vertices;
}