// C3DMeshFileUniqueVerticesChunk::`scalar deleting destructor'(unsigned int) -- free the unique-vertex array, ~CChunkedFileChunk, optional delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABECF0, length 54 bytes.
// Callees: free @ 0x00BFEA14 (__cdecl, result dropped), CChunkedFileChunk::~CChunkedFileChunk
// @ 0x00AA75C0 (stores vtable 0x0129E474 then tail-jumps to ~CBaseClass), operator delete
// @ 0x00BFE9BC when bit 0 of the flag is set. Vtable 0x0129EC6C C3DMeshFileUniqueVerticesChunk
// (rtti-vtables.tsv); the manifest's "CAInputProcess" label is propagated.
// Shape: this class's vtable, then the UniqueVerticesList member (engine header: CArray<unsigned
// long> at +0x10, retail 12 bytes: data pointer first) is destroyed by its inline destructor
// `if (m_pData) free(m_pData)`, the chunk base destructor, the deleting tail. The vtable (and this
// `??_G`) is emitted because the TU constructs the object (MakeOne).
extern "C" void __cdecl free(void* block);      // 0x00BFEA14

struct CArrayULong {
    unsigned long* m_pData;                     // +0x00
    long m_count;                               // +0x04
    long m_capacity;                            // +0x08
    ~CArrayULong()
    {
        if (m_pData != 0)
            free(m_pData);
    }
};

struct CChunkedFileChunk {
    virtual void v00();
    unsigned char _pad[0xc];                    // +0x04..+0x0f chunk header state
    ~CChunkedFileChunk();                       // 0x00AA75C0
};

struct C3DMeshFileUniqueVerticesChunk : CChunkedFileChunk {
    CArrayULong UniqueVerticesList;             // +0x10
    virtual ~C3DMeshFileUniqueVerticesChunk() {}
};

// VC7.1 emits a class's vtable (and the deleting destructor it references) in every translation
// unit that constructs the object; this helper is that construction, nothing more.
void* MakeOne()
{
    return new C3DMeshFileUniqueVerticesChunk;
}