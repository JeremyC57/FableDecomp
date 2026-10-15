// CLandscapeBackgroundTreeNode::StartPatchLoadTask(long, CEngineStaticMapFileBlockManager*, bool) -- allocate the node's async load task for one LOD band's sub-patch data and queue that band's file block with the block manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF65B0, length 124 bytes.
// Callees: CEngineLandscapeMemoryManager::AllocVirtual @ 0x00BD9AF0 (engine 0x01436EA8 -> +0x6b0, class operator new),
//          CEngineStaticMapFileBlockManager::AddRequest @ 0x00BE06B0.
// Shape: retail CLandscapeBackgroundTreeNode sits 8 bytes below the PDB from +0x14 on (two containers are 4 bytes
// smaller in the shipped build): LODLoadInfo +0x0c, FileBlockPos +0x8c, FileBlockSize +0x90, AsyncLoadTask +0x98.
// The task constructor (owner 0, manager, file position, LOD byte, patch flag) is inline and the `new` null test
// selects 0; here the flag store and `return true` are separate statements (`mov byte ptr [ecx+0Eh],1 ; mov al,1`).
struct CMovableResource;
struct CEngineLandscapeMemoryManager {
    void* AllocVirtual(long size, CMovableResource* owner);   // 0x00BD9AF0
};
struct SEngine_01436ea8 {
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* m_pLandscapeMemory;   // +0x6b0
};
extern SEngine_01436ea8* g_pEngine_01436ea8;      // 0x01436EA8
struct CEngineStaticMapFileBlockManager {
    void AddRequest(long filePos, long size, bool highPriority);   // 0x00BE06B0
};
struct CRequestOwner;
struct CLandscapeBackgroundLoadTask {              // 16 bytes, no vtable, allocated from the landscape memory manager
    CRequestOwner*                      m_pOwner;      // +0x00
    CEngineStaticMapFileBlockManager*   m_pManager;    // +0x04
    long                                m_filePos;     // +0x08 the file-block request
    unsigned char                       m_lod;         // +0x0c
    bool                                m_isPatchLoad; // +0x0d
    bool                                m_requested;   // +0x0e
    unsigned char                       _pad0f;
    CLandscapeBackgroundLoadTask(CEngineStaticMapFileBlockManager* manager, long filePos, unsigned char lod, bool isPatchLoad)
        : m_pOwner(0), m_pManager(manager), m_filePos(filePos), m_lod(lod), m_isPatchLoad(isPatchLoad) { }
    static void* operator new(unsigned int size, CMovableResource* owner)
    {
        return g_pEngine_01436ea8->m_pLandscapeMemory->AllocVirtual(size, owner);
    }
    static void operator delete(void*, CMovableResource*) { }
};
struct CLODLoadInfo {                              // 16 bytes
    long FileBlockPos;                             // +0x00
    long FileBlockSize;                            // +0x04
    unsigned char _pad08[8];
};
struct CLandscapeBackgroundTreeNode {
    void*                         __vftable;               // +0x00 (CMovableResource)
    unsigned char                 _pad04[8];
    CLODLoadInfo                  LODLoadInfo[8];          // +0x0c
    long                          FileBlockPos;            // +0x8c
    long                          FileBlockSize;           // +0x90
    long                          OffsetIntoFileBlock;     // +0x94
    CLandscapeBackgroundLoadTask* AsyncLoadTask;           // +0x98
    bool StartPatchLoadTask(long lod, CEngineStaticMapFileBlockManager* manager, bool highPriority);
};

bool CLandscapeBackgroundTreeNode::StartPatchLoadTask(long lod, CEngineStaticMapFileBlockManager* manager, bool highPriority)
{
    AsyncLoadTask = new ((CMovableResource*)this) CLandscapeBackgroundLoadTask(manager, LODLoadInfo[lod].FileBlockPos, (unsigned char)lod, true);
    if (AsyncLoadTask)
    {
        manager->AddRequest(LODLoadInfo[lod].FileBlockPos, LODLoadInfo[lod].FileBlockSize, highPriority);
        AsyncLoadTask->m_requested = true;
        return true;
    }
    return false;
}
