#pragma optimize("s", on)
// CBufferPair_004e25f1::~CBufferPair_004e25f1() -- free the malloc'd buffers at +0x20, +0x14 (each when held)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E25F1.
// Callees: free @ 0x00BFEA14. Shape: members freed in reverse declaration order, each `push ; call ; pop ecx` (the size-optimised 0x004Exxxx module); `this` in esi. The chunk label is propagated across the free-buffers destructor family; the class is named by address.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

struct CBufferPair_004e25f1 {
    unsigned char _pad00[0x14];
    void* m_buffer14;                      // +0x14
    unsigned char _pad18[0x8];
    void* m_buffer20;                      // +0x20
    ~CBufferPair_004e25f1();
};

CBufferPair_004e25f1::~CBufferPair_004e25f1()
{
    if (m_buffer20)
        free(m_buffer20);
    if (m_buffer14)
        free(m_buffer14);
}