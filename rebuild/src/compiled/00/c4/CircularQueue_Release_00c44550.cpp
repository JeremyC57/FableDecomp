// CMarkedBuffer_00c44550::Release() -- when the buffer at +8 is held: clear the marks, free the block at +0 through the pool free, and zero the three words
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C44550, length 48 bytes.
// Callees: ClearMarks @ 0x00C443D0 (on `this`; the manifest label "CTCInventory::ClearMarkedItems" is propagated), the `__cdecl` block free @ 0x00C3C370
// (the label "CTCHeroStats::InformOfEvent" is propagated). Shape: the three zero stores are immediates after the free's `add esp,4`.
// The chunk label "CEngineResourceManager::WorldFrameUpdate" is propagated (the 0x00C4xxxx module); the class is named by address.
void __cdecl FreeBlock_00c3c370(void* block);      // 0x00C3C370

struct CMarkedBuffer_00c44550 {
    void* m_pBlock;                                // +0x00
    void* m_pCursor;                               // +0x04
    void* m_pBuffer;                               // +0x08
    void ClearMarks();                             // 0x00C443D0
    void Release();
};

void CMarkedBuffer_00c44550::Release()
{
    if (m_pBuffer) {
        ClearMarks();
        FreeBlock_00c3c370(m_pBlock);
        m_pBlock = 0;
        m_pCursor = 0;
        m_pBuffer = 0;
    }
}