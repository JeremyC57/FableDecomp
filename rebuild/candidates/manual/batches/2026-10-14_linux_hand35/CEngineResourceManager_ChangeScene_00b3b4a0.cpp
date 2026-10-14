// CEngineResourceManager::ChangeScene() -- defragment the three handle-based pools at +0x8f0, +0x8f8 and +0x900 (each when present) and the engine global's buffer owner, then the graphic data bank at +0x954 changes scene
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3B4A0, length 75 bytes.
// Callees: CHandleBasedMemoryPool::Defragment @ 0x00A02730 three times, the buffer owner's defragment @ 0x00BD9B50, CGraphicDataBank::ChangeScene @ 0x009FD0C0
// (a tail jump). Shape: as DefragmentBuffers @ 0x00B3B1A0 with the bank call appended; `this` stays in esi until the final `pop`.
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

struct CGraphicDataBank {
    void ChangeScene();                            // 0x009FD0C0
};

struct CEngineResourceManager {
    unsigned char           _pad[0x8f0];
    CHandleBasedMemoryPool* m_pPoolA;              // +0x8f0
    unsigned char           _pad8f4[4];
    CHandleBasedMemoryPool* m_pPoolB;              // +0x8f8
    unsigned char           _pad8fc[4];
    CHandleBasedMemoryPool* m_pPoolC;              // +0x900
    unsigned char           _pad904[0x50];
    CGraphicDataBank*       m_pDataBank;           // +0x954
    void ChangeScene();
};

void CEngineResourceManager::ChangeScene()
{
    if (m_pPoolA)
        m_pPoolA->Defragment();
    if (m_pPoolB)
        m_pPoolB->Defragment();
    if (m_pPoolC)
        m_pPoolC->Defragment();
    if (g_pEngine_01436ea8)
        ((CBufferOwner_00bd9b50*)g_pEngine_01436ea8->m_pBuffers)->Defragment();
    m_pDataBank->ChangeScene();
}
