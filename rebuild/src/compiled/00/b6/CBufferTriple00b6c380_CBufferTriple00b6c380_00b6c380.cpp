// CBufferTriple_00b6c380::~CBufferTriple_00b6c380() -- free the malloc'd buffers at +0x18, +0xc and +0 (each when held)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6C380, length 52 bytes.
// Callees: free @ 0x00BFEA14 three times. Shape: members freed in reverse declaration order, each `test ; je ; push ; call ; add esp,4`; the last pointer is loaded into
// esi. The chunk label is propagated; the class is named by address.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

struct CBufferTriple_00b6c380 {
    void*         m_bufferA;                       // +0x00
    unsigned char _pad04[8];
    void*         m_bufferB;                       // +0x0c
    unsigned char _pad10[8];
    void*         m_bufferC;                       // +0x18
    ~CBufferTriple_00b6c380();
};

CBufferTriple_00b6c380::~CBufferTriple_00b6c380()
{
    if (m_bufferC)
        free(m_bufferC);
    if (m_bufferB)
        free(m_bufferB);
    if (m_bufferA)
        free(m_bufferA);
}