// Grade: DIFFER(189v177) -- the shared `xor al,al ; ret` false exit of the five-test early-out is placed at the end of the body; retail places it right after the chain (before `push edi`) and keeps a second false exit for the later tests
// CLandscapeBackgroundTreeNode::FreeableResourceFreeBlock() -- landscape memory pressure hook: drop this node's sub node (and its pending load task) unless the map is locked, the node is visible, still at the top band or needed for the camera range
// Leaf: ?FreeableResourceFreeBlock@CLandscapeBackgroundTreeNode@@EAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF7F70, length 177 bytes (`ret`, private virtual returning bool).
// Callees: CLandscapeBackgroundTreeNode::GetPatch @ 0x00BF6130, GetSplitBranch @ 0x00BF6160 (result unused), the camera-range classifier
// @ 0x00BF7DF0 (labelled CEngineLandscapePatch::DebugCalcInForgroundRange; it returns a long compared with 1), the sub node's virtual deleting
// destructor (slot 0), CLandscapeBackgroundLoadTask's scalar deleting destructor @ 0x00BF8C10.
// Layout: retail CLandscapeBackgroundTreeNode sits 8 bytes below Ego_r.pdb from LODLoadInfo on (landed 0x00BF6B70 model), so
// engine/CLandscapeBackgroundTreeNode.h is not used: LODLoadInfo[8] +0x0C (OptimizedBandRemap at +0x0C of each 16-byte entry),
// AsyncLoadTask +0x98, SubNode +0xA0, Map +0xA4, LastBand +0xE2, FirstNonSplitBand +0xE1, VisibleLastFrame +0xE4,
// VisibleLastFrameHasBeenUpdated +0xE5; CEngineLandscapeMap::MemoryFreeingLocked at retail +0x5C (PDB +0x60).
// Shape: the load task is the landed 0x00BF8C10 model (inline dtor plus inline class operator delete: VC7.1 then calls the
// out-of-line ??_G with 1). One `||` early-out sharing the `xor al,al` exit; `delete SubNode; SubNode = 0;` (clear after the join) versus
// `if (AsyncLoadTask) { delete AsyncLoadTask; AsyncLoadTask = 0; }` (clear inside the branch).
struct CMovableResource {
    virtual ~CMovableResource();
};
struct CEngineLandscapeMap {
    unsigned char _pad00[0x5c];
    bool          MemoryFreeingLocked;          // +0x5c
};
struct CEngineLandscapeMemoryManager {
    void FreeVirtual(void* block);              // 0x00BD9B90
};
struct CEngineLandscapeRenderer {
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* MemoryManager;   // +0x6b0
};
extern CEngineLandscapeRenderer* g_pLandscapeRenderer;   // 0x01436EA8
struct CEngineStaticMapFileBlockManager {
    void ReleaseRequest(void* request);         // 0x00BE0400
};
struct CLandscapeBackgroundLoadTask {           // 16 bytes, no vtable; non-virtual ??_G @ 0x00BF8C10 (landed model)
    CMovableResource*                 m_pOwner;     // +0x00
    CEngineStaticMapFileBlockManager* m_pManager;   // +0x04
    void*                             m_pRequest;   // +0x08
    unsigned char                     _pad0c[2];
    bool                              m_requested;  // +0x0e
    unsigned char                     _pad0f;
    ~CLandscapeBackgroundLoadTask()
    {
        if (m_pOwner)
            delete m_pOwner;
        if (m_requested)
            m_pManager->ReleaseRequest(m_pRequest);
    }
    static void operator delete(void* block) { g_pLandscapeRenderer->MemoryManager->FreeVirtual(block); }
};
struct CLODLoadInfo {                           // 16 bytes
    long          FileBlockPos;
    long          FileBlockSize;
    long          OffsetIntoFileBlock;
    unsigned char OptimizedBandRemap;           // +0x0c
};
struct CLandscapeBackgroundSubNodeBase : CMovableResource {
};
struct CLandscapeBackgroundPatch : CLandscapeBackgroundSubNodeBase {
    unsigned char _pad04[0x114];
    unsigned char LODBand;                      // +0x118
};
struct CLandscapeBackgroundSubNodeSplitBranch;
struct CLandscapeBackgroundTreeNode : CMovableResource {
    void*                            DynamicBuildTemplate;    // +0x04
    void*                            GPUUpdateTask;           // +0x08
    CLODLoadInfo                     LODLoadInfo[8];          // +0x0c
    long                             FileBlockPos;            // +0x8c
    long                             FileBlockSize;           // +0x90
    long                             OffsetIntoFileBlock;     // +0x94
    CLandscapeBackgroundLoadTask*    AsyncLoadTask;           // +0x98
    CLandscapeBackgroundTreeNode*    Parent;                  // +0x9c
    CLandscapeBackgroundSubNodeBase* SubNode;                 // +0xa0
    CEngineLandscapeMap*             Map;                     // +0xa4
    unsigned char                    BoundingBox[0x18];       // +0xa8
    unsigned char                    CameraUpdateThreshold[0x10]; // +0xc0
    long                             OcclusionTestHandle;     // +0xd0
    long                             CreationFlags;           // +0xd4
    unsigned short                   MapX;                    // +0xd8
    unsigned short                   MapY;                    // +0xda
    unsigned short                   Width;                   // +0xdc
    unsigned short                   Height;                  // +0xde
    unsigned char                    FirstBand;               // +0xe0
    unsigned char                    FirstNonSplitBand;       // +0xe1
    unsigned char                    LastBand;                // +0xe2
    bool                             TesselationValid;        // +0xe3
    bool                             VisibleLastFrame;        // +0xe4
    bool                             VisibleLastFrameHasBeenUpdated; // +0xe5
    bool                             HasSubPatchLoadInfo;     // +0xe6
    unsigned char                    _pad0e7;
    CLandscapeBackgroundPatch* GetPatch();                    // 0x00BF6130
    CLandscapeBackgroundSubNodeSplitBranch* GetSplitBranch(); // 0x00BF6160
    long CalcCameraRangeState();                              // 0x00BF7DF0
    bool FreeableResourceFreeBlock();
};

bool CLandscapeBackgroundTreeNode::FreeableResourceFreeBlock()
{
    if (Map->MemoryFreeingLocked || !SubNode || VisibleLastFrame || !VisibleLastFrameHasBeenUpdated || FirstNonSplitBand == 8)
        return false;
    CLandscapeBackgroundPatch* patch = GetPatch();
    GetSplitBranch();
    if (CalcCameraRangeState() == 1)
        return false;
    if (patch && patch->LODBand >= LODLoadInfo[LastBand].OptimizedBandRemap)
        return false;
    delete SubNode;
    SubNode = 0;
    if (AsyncLoadTask) {
        delete AsyncLoadTask;
        AsyncLoadTask = 0;
    }
    return true;
}
