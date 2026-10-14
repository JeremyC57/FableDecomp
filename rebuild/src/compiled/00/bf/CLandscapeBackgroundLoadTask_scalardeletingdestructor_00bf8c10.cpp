// CLandscapeBackgroundLoadTask::`scalar deleting destructor'(unsigned int flags) -- delete the owned request object (+0, its virtual slot 0 with 1), release the file-block request (+8) from the block manager (+4, 0x00BE0400) when it was issued (+0xe), then free this task through the landscape memory manager (engine 0x01436EA8 -> +0x6b0, FreeVirtual @ 0x00BD9B90) for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF8C10, length 80 bytes.
// Leaf: ??_GCLandscapeBackgroundLoadTask@@QAEPAXI@Z
// Shape: no vtable (a non-polymorphic class with its own `operator delete`), `this` in esi, `ret 4`.
struct CEngineLandscapeMemoryManager {
    void FreeVirtual(void* block);                 // 0x00BD9B90
};
struct SEngine_01436ea8 {
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* m_pLandscapeMemory;   // +0x6b0
};
extern SEngine_01436ea8* g_pEngine_01436ea8;      // 0x01436EA8
struct CRequestOwner_00bf8c10 {                   // the owned object: a virtual deleting destructor in slot 0
    virtual ~CRequestOwner_00bf8c10();
};
struct CEngineStaticMapFileBlockManager {
    void ReleaseRequest(void* request);            // 0x00BE0400
};
struct CLandscapeBackgroundLoadTask {              // 16 bytes, no vtable: freed through the landscape memory manager
    CRequestOwner_00bf8c10*             m_pOwner;      // +0x00
    CEngineStaticMapFileBlockManager*   m_pManager;    // +0x04
    void*                               m_pRequest;    // +0x08
    unsigned char                       _pad0c[2];
    bool                                m_requested;   // +0x0e
    unsigned char                       _pad0f;
    ~CLandscapeBackgroundLoadTask()
    {
        if (m_pOwner)
            delete m_pOwner;
        if (m_requested)
            m_pManager->ReleaseRequest(m_pRequest);
    }
    static void operator delete(void* block) { g_pEngine_01436ea8->m_pLandscapeMemory->FreeVirtual(block); }
};

void KillLoadTask_00bf8c10(CLandscapeBackgroundLoadTask* task)   // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete task;
}