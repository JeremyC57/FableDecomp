// CEngineResourceManager::DefragmentBuffers() -- defragment the three handle-based pools at +0x8f0, +0x8f8 and +0x900 (each when present), then the engine global's buffer owner at +0x6b0 (when the global exists)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3B1A0, length 70 bytes.
// Callees: CHandleBasedMemoryPool::Defragment @ 0x00A02730 three times, the buffer owner's defragment @ 0x00BD9B50 (a tail jump; an empty `ret` in retail; the
// manifest label "$E2" is propagated). Shape: `pop esi` is scheduled before the third pool's `je`; the global is tested through eax.
struct CHandleBasedMemoryPool {
    void Defragment();                             // 0x00A02730
};

struct CEngineGlobal_01436ea8 {
    unsigned char _pad[0x6b0];
    void*         m_pBuffers;                      // +0x6b0
};
extern CEngineGlobal_01436ea8* g_pEngine_01436ea8;   // 0x01436EA8

struct CBufferOwner_00bd9b50 {
    void Defragment();                             // 0x00BD9B50 (an empty `ret` in retail)
};

struct CEngineResourceManager {
    unsigned char           _pad[0x8f0];
    CHandleBasedMemoryPool* m_pPoolA;              // +0x8f0
    unsigned char           _pad8f4[4];
    CHandleBasedMemoryPool* m_pPoolB;              // +0x8f8
    unsigned char           _pad8fc[4];
    CHandleBasedMemoryPool* m_pPoolC;              // +0x900
    void DefragmentBuffers();
};

void CEngineResourceManager::DefragmentBuffers()
{
    if (m_pPoolA)
        m_pPoolA->Defragment();
    if (m_pPoolB)
        m_pPoolB->Defragment();
    if (m_pPoolC)
        m_pPoolC->Defragment();
    if (g_pEngine_01436ea8)
        ((CBufferOwner_00bd9b50*)g_pEngine_01436ea8->m_pBuffers)->Defragment();
}