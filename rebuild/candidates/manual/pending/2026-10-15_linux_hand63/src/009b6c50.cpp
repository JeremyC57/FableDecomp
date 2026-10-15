// CNamedBufferRecord_009b6c50::`scalar deleting destructor'(unsigned int) -- implicit destructor inlined: the embedded named buffer at +4 frees its block (+0xc) and then destroys its CCharString name; frees `this` when bit 0 of the flags is set
// Leaf: ??_GCNamedBufferRecord_009b6c50
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B6C50, length 53 bytes.
// Callees: free @ 0x00BFEA14, CCharString::~CCharString() @ 0x0099EAE0 (fable_string.h; manifest
//          label "_Cons_val<...>" is propagated), operator delete @ 0x00BFE9BC.
// Identity: the manifest label "CParticleEffect::`scalar_deleting_destructor'" is propagated across
// byte-identical bodies. Nothing in the image references this body (no call, no vtable slot), it sits
// between SGI heap helpers, and the object it destroys has no vtable store, so its class is unknown
// and named by address; the members are named by role.
// Shape: a non-polymorphic record (`??_G` emitted by VC7.1 for a `delete` of it -- the helper below)
// whose second member is an object with a user destructor: the member's address (`lea edi,[esi+4]`,
// its `this`) is formed once before the null test and kept for the string destructor that follows
// the inline `if (m_data) free(m_data);`. Flattening the members into the record instead puts a late
// `lea ecx,[esi+4]` at the call (tried).

extern "C" void __cdecl free(void* block);                                // 0x00BFEA14

struct CCharString {
    void* m_pData;
    ~CCharString();                                                       // 0x0099EAE0
};

struct CNamedBuffer {                                                     // +0x04 in the record
    CCharString m_name;                                                   // +0x00
    long        m_value;                                                  // +0x04
    void*       m_data;                                                   // +0x08
    ~CNamedBuffer()
    {
        if (m_data)
            free(m_data);
    }
};

struct CNamedBufferRecord_009b6c50 {
    long         m_key;                                                   // +0x00
    CNamedBuffer m_buffer;                                                // +0x04
};

void DeleteNamedBufferRecord(CNamedBufferRecord_009b6c50* record)
{
    delete record;
}
