// CLandscapeBackgroundPatch::OnPostRelocation() -- after the patch moved in landscape memory, re-register it as the handler of every resource block it owns and refresh its z-sorter registration
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE5D80, length 206 bytes (`ret`, virtual slot 7).
// Callees: CEngineLandscapeMemoryManager::SetMovableResourceHandler(block, handler) @ 0x00BD9AB0 (renderer 0x01436EA8 -> +0x6B0),
// CPatchTesselationEdgeStrip::SetMovableResourceHandler(CMovableResource*) @ 0x00BFC510,
// CEngineLandscapeRenderer::OnRegistedZSortingPatchRelocation(long, CLandscapeBackgroundPatch*) @ 0x00B67780.
// Layout: engine/CLandscapeBackgroundPatch.h offsets (they agree with retail), modelled locally with the same member names because the
// generated header's `void* __vftable` cannot carry the CMovableResource virtuals (same choice as the landed dtor 0x00BE6090).
// Shape: the memory manager is read once into a local (ebp); the four-iteration loop over the bridge buffers and edge strips runs on a
// down-counter in the `push ecx` frame word with the buffer/strip pointers stepped in edi/ebx; the renderer is re-read for the last call.
struct CMovableResource {
    virtual ~CMovableResource();
};
struct CTexture;
struct CIndexBuffer;
struct CVertexBufferWin32;
struct CSubPatchTreeLODLockElement;
struct CEngineWaterBackgroundSubPatch;
struct CLandscapeBackgroundTreeNode;
struct CLandscapeBackgroundPatch;
struct CEngineLandscapeMemoryManager {
    void SetMovableResourceHandler(void* block, CMovableResource* handler);   // 0x00BD9AB0
};
struct CEngineLandscapeRenderer {
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* MemoryManager;            // +0x6b0
    void OnRegistedZSortingPatchRelocation(long handle, CLandscapeBackgroundPatch* patch);   // 0x00B67780
};
extern CEngineLandscapeRenderer* g_pLandscapeRenderer;     // 0x01436EA8
struct CPatchTesselationEdgeStrip {                         // 0x2c bytes
    unsigned char _body[0x2c];
    void SetMovableResourceHandler(CMovableResource* handler);   // 0x00BFC510
};
struct CLandscapeBackgroundPatch : CMovableResource {
    CLandscapeBackgroundTreeNode*   Parent;                  // +0x04
    CPatchTesselationEdgeStrip      EdgeStrips[4];           // +0x08
    CTexture*                       Texture;                 // +0xb8
    CIndexBuffer*                   IndexBuffer;             // +0xbc
    CVertexBufferWin32*             VertexBuffer;            // +0xc0
    CIndexBuffer*                   BridgeIndexBuffers[4];   // +0xc4
    CVertexBufferWin32*             BridgeVertexBuffers[4];  // +0xd4
    unsigned short                  BridgePolyCount[4];      // +0xe4
    CSubPatchTreeLODLockElement*    UVLockTreeU;             // +0xec
    CSubPatchTreeLODLockElement*    UVLockTreeV;             // +0xf0
    CEngineWaterBackgroundSubPatch* WaterSubPatch;           // +0xf4
    float                           MinZ;                    // +0xf8
    float                           MaxZ;                    // +0xfc
    long                            ZSorterHandle;           // +0x100
    unsigned short                  VertexCount;             // +0x104
    unsigned short                  PolyCountIncDegenerates; // +0x106
    unsigned short                  TextureWidthNeeded;      // +0x108
    unsigned short                  TextureHeightNeeded;     // +0x10a
    unsigned short                  MapX;                    // +0x10c
    unsigned short                  MapY;                    // +0x10e
    unsigned short                  Width;                   // +0x110
    unsigned short                  Height;                  // +0x112
    unsigned short                  LockedGridSpacingX;      // +0x114
    unsigned short                  LockedGridSpacingY;      // +0x116
    unsigned char                   LODBand;                 // +0x118
    bool                            SharedIndexBuffer;       // +0x119
    bool                            WaterOnlyPatch;          // +0x11a
    bool                            IsDefaultSection;        // +0x11b
    void OnPostRelocation();
};

void CLandscapeBackgroundPatch::OnPostRelocation()
{
    CEngineLandscapeMemoryManager* memory = g_pLandscapeRenderer->MemoryManager;
    if (WaterSubPatch)
        memory->SetMovableResourceHandler(WaterSubPatch, this);
    for (long i = 0; i < 4; ++i) {
        if (BridgeIndexBuffers[i])
            memory->SetMovableResourceHandler(BridgeIndexBuffers[i], this);
        if (BridgeVertexBuffers[i])
            memory->SetMovableResourceHandler(BridgeVertexBuffers[i], this);
        EdgeStrips[i].SetMovableResourceHandler(this);
    }
    if (Texture)
        memory->SetMovableResourceHandler(Texture, this);
    if (IndexBuffer && !SharedIndexBuffer)
        memory->SetMovableResourceHandler(IndexBuffer, this);
    if (VertexBuffer)
        memory->SetMovableResourceHandler(VertexBuffer, this);
    if (ZSorterHandle >= 0)
        g_pLandscapeRenderer->OnRegistedZSortingPatchRelocation(ZSorterHandle, this);
}
