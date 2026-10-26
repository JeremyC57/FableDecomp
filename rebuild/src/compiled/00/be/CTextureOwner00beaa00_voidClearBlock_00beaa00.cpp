// CTextureOwner_00beaa00::void ClearBlock() -- free the landscape block at +0x10 through the landscape memory manager (engine 0x01436EA8 -> +0x6b0, 0x00BDA0D0) and clear it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEAA00, length 48 bytes.
// The chunk label is propagated; named by its effect. Shape: the block pushed before the manager is loaded, `ret`.
class CEngineLandscapeMemoryManager { public:
    void FreeVirtual(void* block);                 // 0x00BD9B90
    void FreeBlock(void* block);                   // 0x00BDA0D0 (labelled "LandscapeLayerList::InsertNode")
};
struct SEngine_01436ea8 {
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* m_pLandscapeMemory;   // +0x6b0
};
extern SEngine_01436ea8* g_pEngine_01436ea8;      // 0x01436EA8
struct CTextureOwner_00beaa00 {
    unsigned char _pad[0x10];
    void*         m_pBlock;                        // +0x10
    void ClearBlock();
};

void CTextureOwner_00beaa00::ClearBlock()
{
    if (m_pBlock) {
        g_pEngine_01436ea8->m_pLandscapeMemory->FreeBlock(m_pBlock);
        m_pBlock = 0;
    }
}