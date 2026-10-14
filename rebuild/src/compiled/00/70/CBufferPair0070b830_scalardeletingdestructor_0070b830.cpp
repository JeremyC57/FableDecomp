// CBufferPair_0070b830::`scalar deleting destructor'(unsigned int) -- non-virtual: free the malloc'd buffers at +0x10 and +4 (each when held), then the flag-tested operator delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070B830, length 55 bytes.
// Callees: free @ 0x00BFEA14 twice, operator delete @ 0x00BFE9BC. Shape: the landed CBufferOwner family (@ 0x00AD4F80 ...) with two buffers: members in reverse
// declaration order, each `test ; je ; push ; call ; add esp,4`, then `test byte ptr [esp+8],1`. The chunk label is propagated; the class is named by address.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

struct CBufferPair_0070b830 {
    unsigned char _pad[4];
    void*         m_bufferA;                       // +0x04
    unsigned char _pad08[8];
    void*         m_bufferB;                       // +0x10
    ~CBufferPair_0070b830()
    {
        if (m_bufferB)
            free(m_bufferB);
        if (m_bufferA)
            free(m_bufferA);
    }
};

// The delete-expression that makes the compiler emit the scalar deleting destructor.
void Destroy_CBufferPair_0070b830(CBufferPair_0070b830* object)
{
    delete object;
}