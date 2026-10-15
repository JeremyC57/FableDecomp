// CEngineWaterBackgroundSubPatch::~CEngineWaterBackgroundSubPatch() -- when the sub-patch owns its buffers, hand every index buffer back to the landscape memory manager's free edge list and free every vertex block (main pair first, then the four bridge pairs); then the CMovableResource base destructor
// Leaf: ??1CEngineWaterBackgroundSubPatch
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFD480, length 144 bytes.
// Callees: the landscape memory manager (engine global @ 0x01436EA8 -> +0x6b0, re-read for every call) AppendFreeEdgeNode
// @ 0x00BDA090 (index buffers) and FreeBlock @ 0x00BDA0D0 (vertex buffers), as in the landed ~CLandscapeLayerMesh @ 0x00BFDE20.
// Vtables: 0x012A8334 (this class) on entry, 0x0129B860 (CMovableResource, its inline destructor) on exit.
// Layout: the PDB member layout of engine/CEngineWaterBackgroundSubPatch.h (IndexBuffer +0x08, VertexBuffer +0x0c,
// BridgeIndexBuffers +0x10, BridgeVertexBuffers +0x20, DeletableBuffers +0x44) -- modelled locally because the header's
// layout struct carries its vptr as a data word and cannot take the vtable stores.
// Shape: a non-virtual destructor in a polymorphic class (keeps the deleting `??_G` out of the TU); the bridge loop is a
// `dec ebx ; jne` count with one pointer (`[esi-10h]` reaches the index array).
struct CIndexBuffer;
struct CVertexBufferWin32;
struct CEngineLandscapeMap;

struct CEngineLandscapeMemoryManager {
    void AppendFreeEdgeNode(void* block);          // 0x00BDA090
    void FreeBlock(void* block);                   // 0x00BDA0D0
};
struct SEngine_01436ea8 {
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* m_pLandscapeMemory;   // +0x6b0
};
extern SEngine_01436ea8* g_pEngine_01436ea8;      // 0x01436EA8

struct CMovableResource {                          // vtable 0x0129B860
    virtual void v00();
    CMovableResource() { }
    ~CMovableResource() { }                        // inline: the vtable store
};

struct CEngineWaterBackgroundSubPatch : CMovableResource {   // vtable 0x012A8334
    CEngineLandscapeMap* Map;                      // +0x04
    CIndexBuffer*        IndexBuffer;              // +0x08
    CVertexBufferWin32*  VertexBuffer;             // +0x0c
    CIndexBuffer*        BridgeIndexBuffers[4];    // +0x10
    CVertexBufferWin32*  BridgeVertexBuffers[4];   // +0x20
    unsigned char        _pad30[0x14];
    bool                 DeletableBuffers;         // +0x44
    virtual void v00();
    ~CEngineWaterBackgroundSubPatch();
};

CEngineWaterBackgroundSubPatch::~CEngineWaterBackgroundSubPatch()
{
    if (DeletableBuffers) {
        if (IndexBuffer)
            g_pEngine_01436ea8->m_pLandscapeMemory->AppendFreeEdgeNode(IndexBuffer);
        if (VertexBuffer)
            g_pEngine_01436ea8->m_pLandscapeMemory->FreeBlock(VertexBuffer);
        for (long i = 0; i < 4; ++i) {
            if (BridgeIndexBuffers[i])
                g_pEngine_01436ea8->m_pLandscapeMemory->AppendFreeEdgeNode(BridgeIndexBuffers[i]);
            if (BridgeVertexBuffers[i])
                g_pEngine_01436ea8->m_pLandscapeMemory->FreeBlock(BridgeVertexBuffers[i]);
        }
    }
}
