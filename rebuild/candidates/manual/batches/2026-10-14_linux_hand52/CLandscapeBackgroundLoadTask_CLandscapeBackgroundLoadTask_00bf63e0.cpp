// CLandscapeBackgroundLoadTask::~CLandscapeBackgroundLoadTask() -- delete the owned request object (+0, its virtual slot 0 with 1) and release the file-block request (+8) from the block manager (+4, 0x00BE0400) when it was issued (+0xe)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF63E0, length 48 bytes.
// Callees: the owner's deleting destructor (slot 0), CEngineStaticMapFileBlockManager::ReleaseRequest @ 0x00BE0400. The 0x00BF8C10 deleting destructor (agent53) inlines this body. Shape: no vtable (a non-polymorphic 16-byte task), `this` in esi, `ret`.
struct CRequestOwner_00bf63e0 {                   // the owned object: a virtual deleting destructor in slot 0
    virtual ~CRequestOwner_00bf63e0();
};
struct CEngineStaticMapFileBlockManager {
    void ReleaseRequest(void* request);            // 0x00BE0400
};
struct CLandscapeBackgroundLoadTask {              // 16 bytes, no vtable
    CRequestOwner_00bf63e0*             m_pOwner;      // +0x00
    CEngineStaticMapFileBlockManager*   m_pManager;    // +0x04
    void*                               m_pRequest;    // +0x08
    unsigned char                       _pad0c[2];
    bool                                m_requested;   // +0x0e
    unsigned char                       _pad0f;
    ~CLandscapeBackgroundLoadTask();
    void ReleaseFileBlockLoad();
};

CLandscapeBackgroundLoadTask::~CLandscapeBackgroundLoadTask()
{
    if (m_pOwner)
        delete m_pOwner;
    if (m_requested)
        m_pManager->ReleaseRequest(m_pRequest);
}
