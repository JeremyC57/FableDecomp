// CEngineWaterBackgroundSubPatch::RelocateData(long, void*, void*, long) -- the landscape memory manager moved one of this sub-patch's blocks from oldBase to newBase: find which buffer pointer held it (bridge pairs first, then the main index and vertex buffers), rebase it, and tell a moved index buffer
// Leaf: ?RelocateData@CEngineWaterBackgroundSubPatch@@EAEXJPAX0J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFD5D0, length 147 bytes (+ int3 padding); private virtual (`ret 10h`, arguments 1 and 4 unused).
// Identity (NF279, 2026-11-22): ?RelocateData@CEngineWaterBackgroundSubPatch@@EAEXJPAX0J@Z (Ego_r.pdb 00a4bd00 + FableWin.pdb 02e06160), slot 1 of
//   the CEngineWaterBackgroundSubPatch vtable 0x012A8334 (RTTI), which the destructor row 0x00BFD480's emitted vtable binds. No call site (exe scan:
//   the .rdata slot only). Was the stand-in ?RelocateData@CEngineWaterBackgroundSubPatch_RelocateData@@QAEXJPAX0J@Z on a derived shell of the
//   flattened header; now the member itself on engine/CEngineWaterBackgroundSubPatch.h (VIRTUALS opt-in on the retail CMovableResource base).
// Callees: CIndexBuffer::OnPostRelocation @ 0x00AC1F60 (an empty `ret` in retail; on the rebased pointer), from engine/CIndexBuffer.h (was a
//   row-local CIndexBuffer model).
// Layout: engine/CEngineWaterBackgroundSubPatch.h -- IndexBuffer +0x08, VertexBuffer +0x0c, BridgeIndexBuffers[4] +0x10,
// BridgeVertexBuffers[4] +0x20.
// Shape: the bridge search compares `oldBase == member` (`cmp esi,[edx-10h]`, the old base in esi) and returns from inside
// the loop; each rebase is the inline `if (p) p = p - old + new` (the new base re-read from its slot); the index arms
// re-read the rebased member for the notification call; the vertex arms just return.
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_WATER_OCCLUSION_ORIGINAL_CHAIN_PASS22          // the class on its retail base CMovableResource
#define FABLE_RETAIL_CENGINEWATERBACKGROUNDSUBPATCH_VIRTUALS        // the virtual destructor and the two private overrides
#include "engine/CEngineWaterBackgroundSubPatch.h"
#include "engine/CMovableResource.h"
#include "engine/CIndexBuffer.h"

template <class T> inline void RelocatePointer(T*& p, void* oldBase, void* newBase)
{
    if (p)
        p = (T*)((char*)p - (char*)oldBase + (long)newBase);
}

void CEngineWaterBackgroundSubPatch::RelocateData(long reason, void* oldBase, void* newBase, long size)
{
    for (long i = 0; i < 4; ++i) {
        if (oldBase == BridgeIndexBuffers[i]) {
            RelocatePointer(BridgeIndexBuffers[i], oldBase, newBase);
            BridgeIndexBuffers[i]->OnPostRelocation();
            return;
        }
        if (oldBase == BridgeVertexBuffers[i]) {
            RelocatePointer(BridgeVertexBuffers[i], oldBase, newBase);
            return;
        }
    }
    if (oldBase == IndexBuffer) {
        RelocatePointer(IndexBuffer, oldBase, newBase);
        IndexBuffer->OnPostRelocation();
        return;
    }
    if (oldBase == VertexBuffer)
        RelocatePointer(VertexBuffer, oldBase, newBase);
}
