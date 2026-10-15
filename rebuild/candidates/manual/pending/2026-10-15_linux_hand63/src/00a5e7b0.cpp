// CObjectBufferRecord_00a5e7b0::`scalar deleting destructor'(unsigned int) -- implicit destructor inlined: the embedded object at +4 frees its block (+8) and then runs its polymorphic member's destructor; frees `this` when bit 0 of the flags is set
// Leaf: ??_GCObjectBufferRecord_00a5e7b0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5E7B0, length 53 bytes.
// Callees: free @ 0x00BFEA14, the root CBaseClass destructor @ 0x0099A2E0 (out of line; it only stores
//          the root vtable 0x01231710), operator delete @ 0x00BFE9BC.
// Identity: the manifest label "CParticleEffect::`scalar_deleting_destructor'" is propagated across
// byte-identical bodies. Nothing in the image references this body (no call, no vtable slot) and the
// destroyed object stores no vtable of its own, so its class is unknown and named by address.
// Shape: the same form as 0x009B6C50 -- a non-polymorphic record (non-virtual `??_G` for a
// `delete`, the helper below) whose member at +4 has a user destructor: its address is formed once
// in edi before the null test, `if (m_data) free(m_data);` runs, then the member's own first member
// (a CBaseClass-derived object, destructor out of line) is destroyed through edi.

extern "C" void __cdecl free(void* block);                                // 0x00BFEA14

struct CBaseClass {
    virtual ~CBaseClass();                                                // 0x0099A2E0
};

struct CObjectBuffer {                                                    // +0x04 in the record
    CBaseClass m_object;                                                  // +0x00 (vptr only)
    void*      m_data;                                                    // +0x04
    ~CObjectBuffer()
    {
        if (m_data)
            free(m_data);
    }
};

struct CObjectBufferRecord_00a5e7b0 {
    long          m_key;                                                  // +0x00
    CObjectBuffer m_buffer;                                               // +0x04
};

void DeleteObjectBufferRecord(CObjectBufferRecord_00a5e7b0* record)
{
    delete record;
}
