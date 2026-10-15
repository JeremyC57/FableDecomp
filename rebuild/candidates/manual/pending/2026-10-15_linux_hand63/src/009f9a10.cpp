// CWideStringBufferRecord_009f9a10::`scalar deleting destructor'(unsigned int) -- implicit destructor inlined: the buffer at +0x4c freed, then the CWideString at +0x30 destroyed; frees `this` when bit 0 of the flags is set
// Leaf: ??_GCWideStringBufferRecord_009f9a10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F9A10, length 49 bytes.
// Callees: free @ 0x00BFEA14, CWideString::~CWideString() @ 0x0099B510, operator delete @ 0x00BFE9BC.
// Identity: the manifest label "CParticleEffect::`scalar_deleting_destructor'" is propagated across
// byte-identical bodies. Nothing in the image references this body (no call, no vtable slot) and the
// destroyed object stores no vtable, so its class is unknown and named by address; the members it
// does not touch (+0x00..+0x30, +0x34..+0x4c) are plain data.
// Shape: a non-polymorphic record (non-virtual `??_G` for a `delete`, the helper below) whose
// implicit destructor runs its members in reverse order: the 12-byte buffer at +0x4c frees its block
// under a null test, then the wide string at +0x30 (`lea ecx,[esi+30h]` at the call -- a direct
// member, unlike the nested-member form of 0x009B6C50).

extern "C" void __cdecl free(void* block);                                // 0x00BFEA14

struct CWideString {
    void* m_pData;
    ~CWideString();                                                       // 0x0099B510
};

struct CRawBuffer {                                                       // retail vector-style buffer (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    ~CRawBuffer()
    {
        if (m_first)
            free(m_first);
    }
};

struct CWideStringBufferRecord_009f9a10 {
    unsigned char m_header[0x30];                                         // +0x00 plain data
    CWideString   m_text;                                                 // +0x30
    unsigned char m_fields[0x18];                                         // +0x34 plain data
    CRawBuffer    m_buffer;                                               // +0x4c
};

void DeleteWideStringBufferRecord(CWideStringBufferRecord_009f9a10* record)
{
    delete record;
}
