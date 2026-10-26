#include <stdio.h>
#include <string.h>
#include <new>
// Behaviour test for CLandscapeBackgroundTreeNode::FreeableResourceFreeBlock (0x00BF7F70).
static int g_deleted = 0, g_freed = 0, g_released = 0, g_split = 0;
class CMovableResource { public:
    virtual ~CMovableResource() { ++g_deleted; }
};
class CEngineLandscapeMap { public:
    unsigned char _pad00[0x5c];
    bool          MemoryFreeingLocked;          // +0x5c
};
class CEngineLandscapeMemoryManager { public:
    void FreeVirtual(void* block) { ++g_freed; }
};
class CEngineLandscapeRenderer { public:
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* MemoryManager;   // +0x6b0
};
CEngineLandscapeRenderer* g_pLandscapeRenderer;
class CEngineStaticMapFileBlockManager { public:
    void ReleaseRequest(void*) { ++g_released; }
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
class CLandscapeBackgroundPatch : public CLandscapeBackgroundSubNodeBase { public:
    unsigned char _pad04[0x114];
    unsigned char LODBand;                      // +0x118
};
class CLandscapeBackgroundSubNodeSplitBranch;
class CLandscapeBackgroundTreeNode : public CMovableResource { public:
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
    CLandscapeBackgroundPatch* patch;
    CLandscapeBackgroundPatch* GetPatch() { return patch; }
    CLandscapeBackgroundSubNodeSplitBranch* GetSplitBranch() { ++g_split; return 0; }
    long range;
    long CalcCameraRangeState() { return range; }
    bool FreeableResourceFreeBlock();
};

bool CLandscapeBackgroundTreeNode::FreeableResourceFreeBlock()
{
    if (Map->MemoryFreeingLocked)
        return false;
    if (!SubNode)
        return false;
    if (VisibleLastFrame)
        return false;
    if (!VisibleLastFrameHasBeenUpdated)
        return false;
    if (FirstNonSplitBand == 8)
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

static unsigned char g_taskMem[sizeof(CLandscapeBackgroundLoadTask)];
int main()
{
    int ok = 1;
    CEngineLandscapeMemoryManager mm; CEngineLandscapeRenderer rend; rend.MemoryManager = &mm; g_pLandscapeRenderer = &rend;
    CEngineLandscapeMap map; map.MemoryFreeingLocked = false;
    CLandscapeBackgroundTreeNode n; memset((char*)&n + 4, 0, sizeof(n) - 4);
    n.Map = &map; n.VisibleLastFrameHasBeenUpdated = true; n.FirstNonSplitBand = 2; n.LastBand = 3; n.LODLoadInfo[3].OptimizedBandRemap = 5;
    if (n.FreeableResourceFreeBlock()) ok = 0;                // no sub node
    CLandscapeBackgroundSubNodeBase* sub = new CLandscapeBackgroundSubNodeBase; n.SubNode = sub;
    map.MemoryFreeingLocked = true; if (n.FreeableResourceFreeBlock()) ok = 0; map.MemoryFreeingLocked = false;
    n.VisibleLastFrame = true; if (n.FreeableResourceFreeBlock()) ok = 0; n.VisibleLastFrame = false;
    n.VisibleLastFrameHasBeenUpdated = false; if (n.FreeableResourceFreeBlock()) ok = 0; n.VisibleLastFrameHasBeenUpdated = true;
    n.FirstNonSplitBand = 8; if (n.FreeableResourceFreeBlock()) ok = 0; n.FirstNonSplitBand = 2;
    if (g_split != 0) ok = 0;
    n.range = 1; if (n.FreeableResourceFreeBlock() || g_split != 1) ok = 0; n.range = 0;
    CLandscapeBackgroundPatch patch; patch.LODBand = 5; n.patch = &patch;
    if (n.FreeableResourceFreeBlock() || g_deleted != 0) ok = 0;
    patch.LODBand = 4;
    CLandscapeBackgroundLoadTask* task = new (g_taskMem) CLandscapeBackgroundLoadTask; task->m_pOwner = 0; task->m_requested = true;
    CEngineStaticMapFileBlockManager fbm; task->m_pManager = &fbm; n.AsyncLoadTask = task;
    if (!n.FreeableResourceFreeBlock()) ok = 0;
    if (n.SubNode != 0 || n.AsyncLoadTask != 0 || g_deleted != 1 || g_freed != 1 || g_released != 1) ok = 0;
    n.patch = 0; n.SubNode = new CLandscapeBackgroundSubNodeBase; if (!n.FreeableResourceFreeBlock() || g_deleted != 2 || g_freed != 1) ok = 0;
    if (ok) { printf("00bf7f70_TEST PASS\n"); return 0; }
    printf("00bf7f70_TEST FAIL\n"); return 1;
}