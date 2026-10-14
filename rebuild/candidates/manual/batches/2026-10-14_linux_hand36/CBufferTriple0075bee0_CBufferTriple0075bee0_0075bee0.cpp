// CBufferTriple_0075bee0::~CBufferTriple_0075bee0() -- free the malloc'd buffers at +0x1ac, +0xf8, +0x44 (each when held)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075BEE0.
// Callees: free @ 0x00BFEA14. Shape: members freed in reverse declaration order, each `push ; call ; add esp,4`; the last pointer is loaded into esi. The chunk label is propagated across the free-buffers destructor family; the class is named by address.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

struct CBufferTriple_0075bee0 {
    unsigned char _pad00[0x44];
    void* m_buffer44;                      // +0x44
    unsigned char _pad48[0xb0];
    void* m_bufferf8;                      // +0xf8
    unsigned char _padfc[0xb0];
    void* m_buffer1ac;                     // +0x1ac
    ~CBufferTriple_0075bee0();
};

CBufferTriple_0075bee0::~CBufferTriple_0075bee0()
{
    if (m_buffer1ac)
        free(m_buffer1ac);
    if (m_bufferf8)
        free(m_bufferf8);
    if (m_buffer44)
        free(m_buffer44);
}
