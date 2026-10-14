// CTextureOwner_00c40800::void ClearErrorMessage() -- clear the error message at +0x7c through the manager singleton (0x0143BB5C, 0x00C3E730) and raise bit 24 of the global flags word (0x0143BC28)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C40800, length 48 bytes.
// The chunk label is propagated; named by its effect. Shape: `or dword ptr [g],1000000h`, `ret`.
struct CManager_00c3e730 {
    void ClearErrorMessage(void* message);         // 0x00C3E730
};
extern CManager_00c3e730* g_pManager_0143bb5c;     // 0x0143BB5C
extern unsigned long g_flags_0143bc28;           // 0x0143BC28
struct CTextureOwner_00c40800 {
    unsigned char _pad[0x7c];
    void*         m_pMessage;                      // +0x7c
    void ClearErrorMessage();
};

void CTextureOwner_00c40800::ClearErrorMessage()
{
    if (m_pMessage) {
        g_pManager_0143bb5c->ClearErrorMessage(m_pMessage);
        m_pMessage = 0;
    }
    g_flags_0143bc28 |= 0x1000000;
}