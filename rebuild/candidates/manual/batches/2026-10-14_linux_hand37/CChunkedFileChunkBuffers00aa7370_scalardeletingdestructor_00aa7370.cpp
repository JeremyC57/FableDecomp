// CChunkedFileChunkBuffers_00aa7370::`scalar deleting destructor'(unsigned int) -- non-virtual: free the buffers at +0x1c and +0x10 (each when held), the CChunkedFileChunk base destructor, then the flag-tested operator delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA7370, length 64 bytes.
// Callees: free @ 0x00BFEA14 twice, CChunkedFileChunk::~CChunkedFileChunk @ 0x00AA75C0 (on `this`: a non-polymorphic base at +0), operator delete @ 0x00BFE9BC. Shape:
// the landed CBufferOwner family with a base destructor call before `test byte ptr [esp+8],1`. The chunk label is propagated; the class is named by address.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

struct CChunkedFileChunk {                        // non-polymorphic: 0x10 bytes
    unsigned char _body[0x10];
    ~CChunkedFileChunk();                          // 0x00AA75C0
};

struct CChunkedFileChunkBuffers_00aa7370 : CChunkedFileChunk {
    void*         m_bufferA;                       // +0x10
    unsigned char _pad14[8];
    void*         m_bufferB;                       // +0x1c
    ~CChunkedFileChunkBuffers_00aa7370()
    {
        if (m_bufferB)
            free(m_bufferB);
        if (m_bufferA)
            free(m_bufferA);
    }
};

// The delete-expression that makes the compiler emit the scalar deleting destructor.
void Destroy_CChunkedFileChunkBuffers_00aa7370(CChunkedFileChunkBuffers_00aa7370* object)
{
    delete object;
}
