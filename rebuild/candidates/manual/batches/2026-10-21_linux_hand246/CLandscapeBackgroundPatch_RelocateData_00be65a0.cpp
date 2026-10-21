// CLandscapeBackgroundPatch::RelocateData(long reason, void* oldBase, void* newBase, long size) -- the landscape memory manager moved one of this patch's blocks from oldBase to newBase: find the pointer that held it (water sub-patch, then per edge the bridge index/vertex buffers and the edge strip's arrays, then the texture, the unshared index buffer and the vertex buffer), rebase it and notify relocated sub-patches and index buffers
// Leaf: RelocateData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE65A0, length 332 bytes, `ret 10h`
//   (`?RelocateData@CLandscapeBackgroundPatch@@UAEXJPAX0J@Z`, arguments 1 and 4 unused).
// Callees: CEngineWaterBackgroundSubPatch::OnPostRelocation @ 0x00BFD3B0, CPatchTesselationEdgeStrip::RelocateData(void*,
//   void*) @ 0x00BFC590, CIndexBuffer's relocation notification @ 0x00AC1F60 (all on the rebased / strip pointer).
// Layout: engine/CLandscapeBackgroundPatch.h (EdgeStrips[4] +0x08 as 0x2C-byte CPatchTesselationEdgeStrip, Texture +0xB8,
//   IndexBuffer +0xBC, VertexBuffer +0xC0, BridgeIndexBuffers +0xC4, BridgeVertexBuffers +0xD4, WaterSubPatch +0xF4,
//   SharedIndexBuffer +0x119); the method is attached through a derived shell as in the landed sibling
//   CEngineWaterBackgroundSubPatch::RelocateData 0x00BFD5D0 (whose RelocatePointer helper this reuses).
// Shape: every arm returns on its own; the strip pointer and the bridge-vertex pointer are stepped per edge (the strip
//   pointer lives in the dead oldBase argument slot).
#include "engine/CLandscapeBackgroundPatch.h"
struct CIndexBuffer {
    void OnRelocated();                            // 0x00AC1F60
};
struct CEngineWaterBackgroundSubPatch {
    void OnPostRelocation();                       // 0x00BFD3B0
};
struct CPatchTesselationEdgeStrip {                // vtable 0x012A8318, 0x2c bytes
    virtual void v00();
    unsigned char _body[0x28];
    bool RelocateData(void* oldBase, void* newBase);   // 0x00BFC590
};

template <class T> inline void RelocatePointer(T*& p, void* oldBase, void* newBase)
{
    if (p)
        p = (T*)((char*)p - (char*)oldBase + (long)newBase);
}

struct CLandscapeBackgroundPatch_RelocateData : CLandscapeBackgroundPatch {
    CPatchTesselationEdgeStrip* GetEdgeStrips() { return (CPatchTesselationEdgeStrip*)EdgeStrips; }
    void RelocateData(long reason, void* oldBase, void* newBase, long size);
};

void CLandscapeBackgroundPatch_RelocateData::RelocateData(long reason, void* oldBase, void* newBase, long size)
{
    if (WaterSubPatch == oldBase) {
        RelocatePointer(WaterSubPatch, oldBase, newBase);
        WaterSubPatch->OnPostRelocation();
        return;
    }
    for (long i = 0; i < 4; ++i) {
        if (oldBase == BridgeIndexBuffers[i]) {
            RelocatePointer(BridgeIndexBuffers[i], oldBase, newBase);
            BridgeIndexBuffers[i]->OnRelocated();
            return;
        }
        if (oldBase == BridgeVertexBuffers[i]) {
            RelocatePointer(BridgeVertexBuffers[i], oldBase, newBase);
            return;
        }
        if (GetEdgeStrips()[i].RelocateData(oldBase, newBase))
            return;
    }
    if (oldBase == Texture) {
        RelocatePointer(Texture, oldBase, newBase);
        return;
    }
    if (oldBase == IndexBuffer && !SharedIndexBuffer) {
        RelocatePointer(IndexBuffer, oldBase, newBase);
        IndexBuffer->OnRelocated();
        return;
    }
    if (oldBase == VertexBuffer)
        RelocatePointer(VertexBuffer, oldBase, newBase);
}
