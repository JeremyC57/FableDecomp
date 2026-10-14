// CBufferPair_00b7ec90::~CBufferPair_00b7ec90() -- free the malloc'd buffers at +0x14, +0x4 (each when held)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B7EC90.
// Callees: free @ 0x00BFEA14. Shape: members freed in reverse declaration order, each `push ; call ; add esp,4`; the second pointer is loaded into esi (`this` is dead by then). The chunk label is propagated across the free-buffers destructor family; the class is named by address.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

struct CBufferPair_00b7ec90 {
    unsigned char _pad00[0x4];
    void* m_buffer04;                      // +0x04
    unsigned char _pad08[0xc];
    void* m_buffer14;                      // +0x14
    ~CBufferPair_00b7ec90();
};

CBufferPair_00b7ec90::~CBufferPair_00b7ec90()
{
    if (m_buffer14)
        free(m_buffer14);
    if (m_buffer04)
        free(m_buffer04);
}
