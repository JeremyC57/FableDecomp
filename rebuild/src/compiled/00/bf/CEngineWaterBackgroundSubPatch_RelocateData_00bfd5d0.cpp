// CEngineWaterBackgroundSubPatch::RelocateData(long, void*, void*, long) -- the landscape memory manager moved one of this sub-patch's blocks from oldBase to newBase: find which buffer pointer held it (bridge pairs first, then the main index and vertex buffers), rebase it, and tell a moved index buffer
// Leaf: RelocateData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFD5D0, length 147 bytes (+ int3 padding); private virtual
// `?RelocateData@CEngineWaterBackgroundSubPatch@@EAEXJPAX0J@Z` (`ret 10h`, arguments 1 and 4 unused).
// Callees: CIndexBuffer's relocation notification @ 0x00AC1F60 (an empty `ret` in retail; on the rebased pointer).
// Layout: engine/CEngineWaterBackgroundSubPatch.h -- IndexBuffer +0x08, VertexBuffer +0x0c, BridgeIndexBuffers[4] +0x10,
// BridgeVertexBuffers[4] +0x20; the method is attached through a derived shell (non-virtual there: the header is a plain
// layout struct whose vptr is a data word).
// Shape: the bridge search compares `oldBase == member` (`cmp esi,[edx-10h]`, the old base in esi) and returns from inside
// the loop; each rebase is the inline `if (p) p = p - old + new` (the new base re-read from its slot); the index arms
// re-read the rebased member for the notification call; the vertex arms just return.
#include "engine/CEngineWaterBackgroundSubPatch.h"

class CIndexBuffer { public:
    void OnRelocated();                            // 0x00AC1F60
};

template <class T> inline void RelocatePointer(T*& p, void* oldBase, void* newBase)
{
    if (p)
        p = (T*)((char*)p - (char*)oldBase + (long)newBase);
}

struct CEngineWaterBackgroundSubPatch_RelocateData : CEngineWaterBackgroundSubPatch {
    void RelocateData(long reason, void* oldBase, void* newBase, long size);
};

void CEngineWaterBackgroundSubPatch_RelocateData::RelocateData(long reason, void* oldBase, void* newBase, long size)
{
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
    }
    if (oldBase == IndexBuffer) {
        RelocatePointer(IndexBuffer, oldBase, newBase);
        IndexBuffer->OnRelocated();
        return;
    }
    if (oldBase == VertexBuffer)
        RelocatePointer(VertexBuffer, oldBase, newBase);
}