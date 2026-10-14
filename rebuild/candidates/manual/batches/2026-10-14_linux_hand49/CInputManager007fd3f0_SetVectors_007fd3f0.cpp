// CInputManager_007fd3f0::SetVectors(const C3DVector& a, const C3DVector& b, long c) -- copy two 3-vectors into +0xc and +0x18 and the word into +0x28
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FD3F0, length 64 bytes.
// The chunk label "CInputManager::UpdateChecksum" is propagated; named by its effect. Shape: the member addresses in eax (`lea eax,[ecx+0Ch]`), three dword copies each, `ret 0Ch`.
struct C3DVector { float X, Y, Z; };
struct CInputManager_007fd3f0 {
    unsigned char _pad[0xc];
    C3DVector     m_a;                             // +0x0c
    C3DVector     m_b;                             // +0x18
    unsigned char _pad24[4];
    long          m_c;                             // +0x28
    void SetVectors(const C3DVector& a, const C3DVector& b, long c);
};

void CInputManager_007fd3f0::SetVectors(const C3DVector& a, const C3DVector& b, long c)
{
    m_a = a;
    m_b = b;
    m_c = c;
}
