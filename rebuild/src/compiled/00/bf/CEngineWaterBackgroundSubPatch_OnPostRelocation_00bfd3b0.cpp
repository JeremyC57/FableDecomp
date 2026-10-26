// CEngineWaterBackgroundSubPatch::OnPostRelocation() -- after this sub-patch moved, re-register it as the owner of every landscape-memory buffer it holds: the four bridge index/vertex buffer pairs, then the main index and vertex buffers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFD3B0, length 106 bytes (+ int3 padding).
// Callees: the landscape memory manager's RebindBlockOwner(block, owner) @ 0x00BD9AB0 (engine global @ 0x01436EA8 -> +0x6b0),
// as in the landed CLandscapeLayerMesh::OnPostRelocation @ 0x00BFDD70.
// Layout: engine/CEngineWaterBackgroundSubPatch.h -- IndexBuffer +0x08, VertexBuffer +0x0c, BridgeIndexBuffers[4] +0x10,
// BridgeVertexBuffers[4] +0x20 (the method is attached through a derived shell: the header is a plain layout struct).
// Shape: the manager is read once into a local (ebx); the bridge loop is rotated into a `dec ebp ; jne` count with one
// pointer walking the vertex array (`[edi-10h]` reaches the index array); each buffer is null-tested before its call.
#include "engine/CEngineWaterBackgroundSubPatch.h"

class CEngineLandscapeMemoryManager { public:
    void RebindBlockOwner(void* block, void* owner);   // 0x00BD9AB0
};
struct SEngine_01436ea8 {
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* m_pLandscapeMemory;   // +0x6b0
};
extern SEngine_01436ea8* g_pEngine_01436ea8;      // 0x01436EA8

struct CEngineWaterBackgroundSubPatch_OnPostRelocation : CEngineWaterBackgroundSubPatch {
    void OnPostRelocation();
};

void CEngineWaterBackgroundSubPatch_OnPostRelocation::OnPostRelocation()
{
    CEngineLandscapeMemoryManager* memory = g_pEngine_01436ea8->m_pLandscapeMemory;
    for (long i = 0; i < 4; ++i) {
        if (BridgeIndexBuffers[i])
            memory->RebindBlockOwner(BridgeIndexBuffers[i], this);
        if (BridgeVertexBuffers[i])
            memory->RebindBlockOwner(BridgeVertexBuffers[i], this);
    }
    if (IndexBuffer)
        memory->RebindBlockOwner(IndexBuffer, this);
    if (VertexBuffer)
        memory->RebindBlockOwner(VertexBuffer, this);
}