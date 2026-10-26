// CLandscapeBackgroundTreeNode::ProcessLoadTask(long, double, bool, CEngineStaticMapFileBlockManager*) -- advance this node's asynchronous load task: past the deadline report still-loading; drop the task when it may be aborted; once its file block is resident build the sub-node (a patch loaded with the node's Z range, or a split branch whose own load is then started) and, for a branch, pump its background loading, adopting it as the node's sub-node when it completes (LS_LOADED 1 / LS_LOADING 2 / LS_FAILED 3)
// Leaf: ProcessLoadTask
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BF84C0, 842 bytes (+ int3 padding), `ret 14h`.
// Callees: GFGetFastSubFrameTimer 0x009E1C70; TestIfLoadTaskCanBeAborted 0x00BF6490; on the file-block manager 0x00BE0400 (unlock),
//   0x00BE0420 (prioritise), 0x00BE0480 (is loaded), 0x00BE04C0 (block data); CMemoryDataInputStream(data, size) 0x00994930 /
//   dtor 0x00993B10; CLandscapeBackgroundPatch operator new(size, CMovableResource*) 0x00BE5BB0 (__fastcall) / ctor 0x00BE5FA0 /
//   Load(CDataInputStream&, float, float) 0x00BE8920; split-branch operator new 0x00BF6260 / ctor 0x00BF8C80 / Load 0x00BF7B90 /
//   background loading 0x00BF8430; load task start 0x00BF6410 and scalar deleting destructor 0x00BF8C10; the engine resource
//   pool (g_pEngine 0x01436EA8 +0x6B0) Free 0x00BD9B90; OnSubNodeLoaded 0x00BF6750 (descriptive name).
// Layout: Ego_r.pdb CLandscapeBackgroundTreeNode shifted -8 from LODLoadInfo on (retail AsyncLoadTask +0x98 vs PDB +0xa0;
//   LODLoadInfo +0x0c stride 0x10, FileBlockSize +0x90, OffsetIntoFileBlock +0x94, SubNode +0xa0, Map +0xa4, BoundingBox +0xa8
//   (Min.Z +0xb0, Max.Z +0xbc), CreationFlags +0xd4, VisibleLastFrame +0xe4, VisibleLastFrameHasBeenUpdated +0xe5);
//   CLandscapeBackgroundLoadTask and CLODLoadInfo as Ego_r.pdb; CMemoryDataInputStream 0x1c bytes (its ctor stores +0..+0x18).
// Shape: retail expands the load task's scalar deleting destructor inline once (the abort path: LoadingTree delete, block
//   unlock, pool free) and calls the out-of-line copy 0x00BF8C10 at every other delete, so the abort path is written out and
//   the other deletes call the destructor stand-in by name (the inline-once band); the split-branch path sits in an else
//   block so both streams share the frame word.
typedef unsigned int size_t;
double __fastcall GFGetFastSubFrameTimer();         // 0x009E1C70
enum LS_LOADING_STATUS { LS_NONE, LS_LOADED, LS_LOADING, LS_FAILED };
class CDataInputStream;
class CMemoryDataInputStream { public:
    unsigned char _d[0x1c - 4];
    CMemoryDataInputStream(const char* data, long size);   // 0x00994930
    virtual ~CMemoryDataInputStream();                      // 0x00993B10
    operator CDataInputStream&() { return *reinterpret_cast<CDataInputStream*>(this); }
};
class CEngineStaticMapFileBlockManager { public:
    void PrioritiseBlock(long pos);                 // 0x00BE0420
    bool IsBlockLoaded(long pos);                   // 0x00BE0480
    const char* GetBlockData(long pos);             // 0x00BE04C0
public:
    void ReleaseRequest(long);   // 0x00BE0400
};
class CMovableResource;
class CEngineLandscapeMemoryManager { public: public:
    void FreeVirtual(void*);   // 0x00BD9B90
};   // 0x00BD9B90
struct CEngineGlobal { unsigned char _pad[0x6b0]; CEngineLandscapeMemoryManager* Pool; };   // +0x6B0
extern CEngineGlobal* g_pEngine;                    // 0x01436EA8
class CEngineLandscapeMap;
class CLandscapeBackgroundTreeNode;
struct CLandscapeBackgroundSubNodeBase {
    virtual ~CLandscapeBackgroundSubNodeBase();     // slot 0 (scalar deleting)
};
class CLandscapeBackgroundSubNodeSplitBranch : public CLandscapeBackgroundSubNodeBase { public:
    static void* __fastcall operator new(size_t size, CMovableResource* owner);   // 0x00BF6260
    static void __fastcall operator delete(void* p, CMovableResource* owner);
    CLandscapeBackgroundSubNodeSplitBranch(CEngineLandscapeMap* map, CLandscapeBackgroundTreeNode* parent, long flags);   // 0x00BF8C80
    void Load(CDataInputStream& stream);            // 0x00BF7B90
    unsigned char _body[0x1d4 - 4];
    LS_LOADING_STATUS ProcessBackgroundLoading(double time, bool abort, CEngineStaticMapFileBlockManager* manager, bool visible, bool hidden);   // 0x00BF8430
};
class CLandscapeBackgroundPatch : public CLandscapeBackgroundSubNodeBase { public:
    static void* __fastcall operator new(size_t size, CMovableResource* owner);   // 0x00BE5BB0
    static void __fastcall operator delete(void* p, CMovableResource* owner);
    CLandscapeBackgroundPatch(CLandscapeBackgroundTreeNode* parent);   // 0x00BE5FA0
    bool Load(CDataInputStream& stream, float minZ, float maxZ);      // 0x00BE8920
    unsigned char _body[0x11c - 4];
};
struct CLandscapeBackgroundLoadTask {               // 16 bytes
    CLandscapeBackgroundSubNodeSplitBranch* LoadingTree;   // +0x00
    CEngineStaticMapFileBlockManager*       FileBlockManager;   // +0x04
    long                                    FileBlockPos;  // +0x08
    unsigned char                           LOD;           // +0x0c
    bool                                    IsPatchLoadTask;   // +0x0d
    bool                                    BlockLoading;  // +0x0e
    void* ScalarDeletingDestructor(unsigned int flags);   // 0x00BF8C10 (delete: release LoadingTree, unlock the block, pool free)
    void StartLoading();                            // 0x00BF6410
};
struct CLODLoadInfo { long FileBlockPos, FileBlockSize, OffsetIntoFileBlock; unsigned char OptimizedBandRemap; };
class CLandscapeBackgroundTreeNode { public:
    void*                            vptr;
    unsigned char                    _pad4[8];
    CLODLoadInfo                     LODLoadInfo[8];    // +0x0c
    long                             FileBlockPos;      // +0x8c
    long                             FileBlockSize;     // +0x90
    long                             OffsetIntoFileBlock;   // +0x94
    CLandscapeBackgroundLoadTask*    AsyncLoadTask;     // +0x98
    CLandscapeBackgroundTreeNode*    Parent;            // +0x9c
    CLandscapeBackgroundSubNodeBase* SubNode;           // +0xa0
    CEngineLandscapeMap*             Map;               // +0xa4
    float                            BoundingBox[6];    // +0xa8 (min xyz, max xyz)
    float                            CameraUpdateThreshold[4];   // +0xc0
    long                             OcclusionTestHandle;   // +0xd0
    long                             CreationFlags;     // +0xd4
    unsigned short                   MapX, MapY, Width, Height;   // +0xd8
    unsigned char                    FirstBand, FirstNonSplitBand, LastBand;   // +0xe0
    bool                             TesselationValid;  // +0xe3
    bool                             VisibleLastFrame;  // +0xe4
    bool                             VisibleLastFrameHasBeenUpdated;   // +0xe5
    float PeekMinZ() const { return BoundingBox[2]; }
    float PeekMaxZ() const { return BoundingBox[5]; }
    bool TestIfLoadTaskCanBeAborted(long lod, bool abort);   // 0x00BF6490
    void OnSubNodeLoaded();                         // 0x00BF6750
    LS_LOADING_STATUS ProcessLoadTask(long lod, double time, bool abort, CEngineStaticMapFileBlockManager* manager);
};

LS_LOADING_STATUS CLandscapeBackgroundTreeNode::ProcessLoadTask(long lod, double time, bool abort, CEngineStaticMapFileBlockManager* manager)
{
    if (GFGetFastSubFrameTimer() > time)
        return LS_LOADING;
    if (TestIfLoadTaskCanBeAborted(lod, abort)) {
        CLandscapeBackgroundLoadTask* task = AsyncLoadTask;
        if (task) {
            if (task->LoadingTree)
                delete task->LoadingTree;
            if (task->BlockLoading)
                task->FileBlockManager->ReleaseRequest(task->FileBlockPos);
            g_pEngine->Pool->FreeVirtual(task);
        }
        AsyncLoadTask = 0;
        return LS_LOADED;
    }
    if (AsyncLoadTask->LoadingTree == 0) {
        if (VisibleLastFrame)
            manager->PrioritiseBlock(AsyncLoadTask->FileBlockPos);
        if (!manager->IsBlockLoaded(AsyncLoadTask->FileBlockPos))
            return LS_LOADING;
        if (AsyncLoadTask->IsPatchLoadTask) {
            CLODLoadInfo& info = LODLoadInfo[AsyncLoadTask->LOD];
            CMemoryDataInputStream stream(manager->GetBlockData(AsyncLoadTask->FileBlockPos) + info.OffsetIntoFileBlock, info.FileBlockSize - info.OffsetIntoFileBlock);
            CLandscapeBackgroundPatch* patch = new ((CMovableResource*)this) CLandscapeBackgroundPatch(this);
            if (!patch) {
                if (AsyncLoadTask) AsyncLoadTask->ScalarDeletingDestructor(1);
                AsyncLoadTask = 0;
                return LS_FAILED;
            }
            if (!patch->Load(stream, PeekMinZ(), PeekMaxZ())) {
                if (AsyncLoadTask) AsyncLoadTask->ScalarDeletingDestructor(1);
                AsyncLoadTask = 0;
                delete patch;
                return LS_FAILED;
            }
            delete SubNode;
            SubNode = patch;
            if (AsyncLoadTask) AsyncLoadTask->ScalarDeletingDestructor(1);
            AsyncLoadTask = 0;
            OnSubNodeLoaded();
            return LS_LOADED;
        }
        else {
            CLandscapeBackgroundSubNodeSplitBranch* branch = new ((CMovableResource*)this) CLandscapeBackgroundSubNodeSplitBranch(Map, this, CreationFlags);
            if (!branch)
                return LS_FAILED;
            CMemoryDataInputStream stream(manager->GetBlockData(AsyncLoadTask->FileBlockPos) + OffsetIntoFileBlock, FileBlockSize - OffsetIntoFileBlock);
            branch->Load(stream);
            AsyncLoadTask->LoadingTree = branch;
            AsyncLoadTask->StartLoading();
        }
    }
    LS_LOADING_STATUS status = AsyncLoadTask->LoadingTree->ProcessBackgroundLoading(time, abort, manager,
        VisibleLastFrame || !VisibleLastFrameHasBeenUpdated, !VisibleLastFrame);
    if (status == LS_LOADED) {
        delete SubNode;
        SubNode = AsyncLoadTask->LoadingTree;
        AsyncLoadTask->LoadingTree = 0;
        if (AsyncLoadTask) AsyncLoadTask->ScalarDeletingDestructor(1);
        AsyncLoadTask = 0;
        OnSubNodeLoaded();
        return LS_LOADED;
    }
    if (status == LS_FAILED) {
        if (AsyncLoadTask) AsyncLoadTask->ScalarDeletingDestructor(1);
        AsyncLoadTask = 0;
        return LS_FAILED;
    }
    return status;
}