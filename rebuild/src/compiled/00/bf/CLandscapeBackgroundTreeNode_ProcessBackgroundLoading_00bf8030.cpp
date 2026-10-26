// CLandscapeBackgroundTreeNode::ProcessBackgroundLoading(double, bool, CEngineStaticMapFileBlockManager*, bool, bool) -- background-load this node: past the deadline report still-loading; work out the wanted LOD band and mark the node visible; drop a pending load task that no longer suits the band; with no sub-node yet, start a patch or tree load task (unless its block is not yet resident), then once the block is resident build the patch (loaded with the node's Z range) or split branch from it; a loaded patch is done, a split branch is loaded recursively (LS_LOADED 1 / LS_LOADING 2 / LS_FAILED 3)
// Leaf: ProcessBackgroundLoading
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BF8030, 1022 bytes, `ret 18h`.
// Callees: GFGetFastSubFrameTimer 0x009E1C70; the LOD band calculation 0x00BF7DF0; StartTreeLoadTask 0x00BF6530 and
//   StartPatchLoadTask 0x00BF65B0 (FableWin.pdb names); the sub-node's slot 4 (+0x10, is-patch test); the file-block manager
//   0x00BE0420 (prioritise), 0x00BE0480 (is loaded), 0x00BE04C0 (block data); CMemoryDataInputStream(data, size) 0x00994930 /
//   dtor 0x00993B10; patch operator new 0x00BE5BB0 / ctor 0x00BE5FA0 / Load 0x00BE8920; the engine resource pool
//   (g_pEngine 0x01436EA8 +0x6B0) Alloc 0x00BD9AF0 (the split branch's operator new, expanded inline here); split-branch ctor
//   0x00BF8C80 / Load 0x00BF7B90 / ProcessBackgroundLoading 0x00BF8430; load-task scalar deleting destructor 0x00BF8C10.
// Layout: as ProcessLoadTask 0x00BF84C0 (Ego_r.pdb CLandscapeBackgroundTreeNode shifted -8 from LODLoadInfo on; FileBlockPos
//   +0x8c, FirstNonSplitBand +0xe1).
// Shape: the GetPatch / GetSplitBranch accessors are inline (the first GetPatch result is dropped but its virtual call stays);
//   the split branch is placement-constructed in the pool block (the new-expression keeps its null test); the load task's
//   delete is the named scalar-deleting-destructor stand-in, as in ProcessLoadTask.
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
    void UnlockBlock(long pos);                     // 0x00BE0400
    void PrioritiseBlock(long pos);                 // 0x00BE0420
    bool IsBlockLoaded(long pos);                   // 0x00BE0480
    const char* GetBlockData(long pos);             // 0x00BE04C0
};
class CMovableResource;
struct CMovableResourcePool {
    void* Alloc(size_t size, CMovableResource* owner);   // 0x00BD9AF0
    void Free(void* p);                             // 0x00BD9B90
};
struct CEngineGlobal { unsigned char _pad[0x6b0]; CMovableResourcePool* Pool; };   // +0x6B0
extern CEngineGlobal* g_pEngine;                    // 0x01436EA8
class CEngineLandscapeMap;
class CLandscapeBackgroundTreeNode;
struct CLandscapeBackgroundSubNodeBase {
    virtual ~CLandscapeBackgroundSubNodeBase();     // slot 0 (scalar deleting)
    virtual void n1(); virtual void n2(); virtual void n3();
    virtual bool IsPatch();                         // slot 4 (+0x10)
};
class CLandscapeBackgroundSubNodeSplitBranch : public CLandscapeBackgroundSubNodeBase { public:
    static void* operator new(size_t size, void* place) { return place; }
    static void operator delete(void* p, void* place) { }
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
    long CalculateLODBand();                        // 0x00BF7DF0 (descriptive name)
    bool StartTreeLoadTask(CEngineStaticMapFileBlockManager* manager, bool block);   // 0x00BF6530
    bool StartPatchLoadTask(long lod, CEngineStaticMapFileBlockManager* manager, bool block);   // 0x00BF65B0
    CLandscapeBackgroundPatch* GetPatch() { return SubNode && SubNode->IsPatch() ? static_cast<CLandscapeBackgroundPatch*>(SubNode) : 0; }
    CLandscapeBackgroundSubNodeSplitBranch* GetSplitBranch() { return SubNode && !SubNode->IsPatch() ? static_cast<CLandscapeBackgroundSubNodeSplitBranch*>(SubNode) : 0; }
    LS_LOADING_STATUS ProcessBackgroundLoading(double time, bool abort, CEngineStaticMapFileBlockManager* manager, bool visible, bool block);
};


LS_LOADING_STATUS CLandscapeBackgroundTreeNode::ProcessBackgroundLoading(double time, bool abort, CEngineStaticMapFileBlockManager* manager, bool visible, bool block)
{
    if (GFGetFastSubFrameTimer() > time)
        return LS_LOADING;
    bool wasUpdated = VisibleLastFrameHasBeenUpdated;
    long band = CalculateLODBand();
    VisibleLastFrame = true;
    VisibleLastFrameHasBeenUpdated = true;
    GetPatch();
    if (VisibleLastFrameHasBeenUpdated) {
        if (VisibleLastFrame) {
            if (band >= FirstNonSplitBand && FirstNonSplitBand < 8) {
                if (AsyncLoadTask && AsyncLoadTask->IsPatchLoadTask && LODLoadInfo[band].OptimizedBandRemap < AsyncLoadTask->LOD) {
                    AsyncLoadTask->ScalarDeletingDestructor(1);
                    AsyncLoadTask = 0;
                }
            } else if (band < FirstNonSplitBand) {
                if (AsyncLoadTask && AsyncLoadTask->IsPatchLoadTask) {
                    AsyncLoadTask->ScalarDeletingDestructor(1);
                    AsyncLoadTask = 0;
                }
            }
        } else if (!wasUpdated) {
            if (band >= FirstNonSplitBand && FirstNonSplitBand < 8 && AsyncLoadTask
                && (!AsyncLoadTask->IsPatchLoadTask || LODLoadInfo[band].OptimizedBandRemap > AsyncLoadTask->LOD)) {
                AsyncLoadTask->ScalarDeletingDestructor(1);
                AsyncLoadTask = 0;
            }
        }
    }
    if (SubNode == 0) {
        if (AsyncLoadTask == 0) {
            if (band >= FirstNonSplitBand && FirstNonSplitBand != 8) {
                long lod = LODLoadInfo[band].OptimizedBandRemap;
                if (!VisibleLastFrameHasBeenUpdated && !manager->IsBlockLoaded(LODLoadInfo[lod].FileBlockPos))
                    return LS_LOADING;
                if (!StartPatchLoadTask(lod, manager, block))
                    return LS_FAILED;
            } else {
                if (!VisibleLastFrameHasBeenUpdated && !manager->IsBlockLoaded(FileBlockPos))
                    return LS_LOADING;
                if (!StartTreeLoadTask(manager, block))
                    return LS_FAILED;
            }
        }
        if (!block)
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
            SubNode = patch;
            if (AsyncLoadTask) AsyncLoadTask->ScalarDeletingDestructor(1);
            AsyncLoadTask = 0;
            return LS_LOADED;
        } else {
            CMemoryDataInputStream stream(manager->GetBlockData(AsyncLoadTask->FileBlockPos) + OffsetIntoFileBlock, FileBlockSize - OffsetIntoFileBlock);
            CLandscapeBackgroundSubNodeSplitBranch* branch = new (g_pEngine->Pool->Alloc(0x1d4, (CMovableResource*)this)) CLandscapeBackgroundSubNodeSplitBranch(Map, this, CreationFlags);
            if (!branch) {
                if (AsyncLoadTask) AsyncLoadTask->ScalarDeletingDestructor(1);
                AsyncLoadTask = 0;
                return LS_FAILED;
            }
            branch->Load(stream);
            SubNode = branch;
            if (AsyncLoadTask) AsyncLoadTask->ScalarDeletingDestructor(1);
            AsyncLoadTask = 0;
        }
    }
    if (GetPatch())
        return LS_LOADED;
    return GetSplitBranch()->ProcessBackgroundLoading(time, abort, manager, VisibleLastFrame, block);
}