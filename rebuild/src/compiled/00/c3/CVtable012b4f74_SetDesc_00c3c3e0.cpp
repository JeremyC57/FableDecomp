// CVtable_012b4f74::SetDesc(const SBufferDesc& desc) -- copy the 28-byte buffer descriptor in, allocate the buffer when the descriptor carries none (-5 on failure, state bit 1 = owned), flag format 0x80000000 (bit 7) and mark the descriptor set (bit 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3C3E0, length 98 bytes (`ret 4`).
// Callees: AllocBuffer @ 0x00C3C360 (__cdecl, forwards to the allocation hook at 0x0139C97C).
// Label: the catalog's CShaderRenderManager::SetProjectionTransform is propagated. The object is the no-RTTI class
// with vtable 0x012B4F74 (landed ctor 0x00C3C3B0 clears the seven-word record at +0x10 and the state at +0x2c;
// landed Close 0x00C3C450 frees +0x28 when state bit 1 is set): the record's seventh word IS the buffer pointer
// (+0x28), which is why the slot-3 setup 0x00C3C4E0 reads the caller's buffer from desc+0x18.
// Shape: one struct assignment (`rep movsd` x7 through ebp = &m_desc); the null test re-reads the copied member.
// Leaf: SetDesc
void* __cdecl AllocBuffer(unsigned long size);   // 0x00C3C360

struct SBufferDesc {
    unsigned long m_format;     // +0x00 (0x80000000 = flagged format)
    long          m_a, m_b;
    unsigned long m_size;       // +0x0c
    long          m_c, m_d;
    void*         m_pBuffer;  // +0x18
};

struct CVtable_012b4f74 {
    virtual void v00();
    unsigned char _pad[0xc];
    SBufferDesc   m_desc;       // +0x10
    unsigned long m_state;      // +0x2c
    long SetDesc(const SBufferDesc& desc);
};

long CVtable_012b4f74::SetDesc(const SBufferDesc& desc)
{
    m_desc = desc;
    if (!m_desc.m_pBuffer) {
        m_desc.m_pBuffer = AllocBuffer(m_desc.m_size);
        if (!m_desc.m_pBuffer)
            return -5;
        m_state |= 2;
    }
    if (m_desc.m_format == 0x80000000)
        m_state |= 0x80;
    m_state |= 1;
    return 0;
}