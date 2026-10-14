// CGraphicFrameData::Uninitialise() -- clear the initialised bit (+0x1a bit 4) and, when the data owns a texture (bit 2), release the counted texture at +8 (destroy and delete its block at zero, the count word then the object word cleared)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FCBA0, length 80 bytes.
// Callees: the count block's destroy (slot +4 with the object at +8 in ecx), operator delete @ 0x00BFE9BC. Shape: the flag byte updated through cl with the bit-2 test
// on the new value, the counted pointer's Reset (count word cleared before the object word).
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct SCountBlock_009fcba0 {
    long  m_references;                            // +0x00
    void (__fastcall* m_pfnDestroy)(void* object); // +0x04
    void* m_pObject;                               // +0x08
};
struct CCountedPointer_009fcba0 {                  // 8 bytes: the object and its count block
    void*                 m_pObject;               // +0x00
    SCountBlock_009fcba0* m_pCount;                // +0x04
    void Reset()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
        m_pCount = 0;
        m_pObject = 0;
    }
};

struct CGraphicFrameData {
    unsigned char           _pad[8];
    CCountedPointer_009fcba0 m_texture;            // +0x08
    unsigned char           _pad10[0xa];
    unsigned char           m_flags;               // +0x1a (bit 2 = owns a texture, bit 4 = initialised)
    void Uninitialise();
};

void CGraphicFrameData::Uninitialise()
{
    m_flags &= ~0x10;
    if (m_flags & 4)
        m_texture.Reset();
}
