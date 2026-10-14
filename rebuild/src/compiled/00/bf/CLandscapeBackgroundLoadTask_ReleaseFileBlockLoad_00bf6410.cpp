// CLandscapeBackgroundLoadTask::ReleaseFileBlockLoad() -- when the file-block request (+8) was issued (+0xe) release it from the block manager (+4, 0x00BE0400) and clear the flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF6410, length 28 bytes.
// Callee: CEngineStaticMapFileBlockManager::ReleaseRequest @ 0x00BE0400. Shape: the flag byte in al, the request loaded before the manager, `this` in esi, `ret`.
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

void CLandscapeBackgroundLoadTask::ReleaseFileBlockLoad()
{
    if (m_requested) {
        m_pManager->ReleaseRequest(m_pRequest);
        m_requested = false;
    }
}