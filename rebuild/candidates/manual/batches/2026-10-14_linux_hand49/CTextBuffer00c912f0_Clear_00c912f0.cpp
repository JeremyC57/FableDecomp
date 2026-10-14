// CTextBuffer_00c912f0::Clear() -- reset the buffer: the write pointer (+0xc) back to the buffer start (+8), its first byte cleared, the two words at +0 and +4 cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C912F0, length 23 bytes.
// The chunk label "CIndexBuffer::CIndexBuffer" is propagated; named by its effect. Shape: no frame, `ret`.
struct CTextBuffer_00c912f0 {
    long  m_0;                                     // +0x00
    long  m_4;                                     // +0x04
    char* m_pBuffer;                               // +0x08
    char* m_pWrite;                                // +0x0c
    void Clear();
};

void CTextBuffer_00c912f0::Clear()
{
    m_pWrite = m_pBuffer;
    m_pBuffer[0] = 0;
    m_0 = 0;
    m_4 = 0;
}
