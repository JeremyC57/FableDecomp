// CVtable_012b4f74::Setup(const SBufferDesc& desc) -- validate the descriptor kind (0..6, else -3), store it via SetDesc, and for kinds 0..2 with sound hardware present create the audio buffer at +0x30, closing the object again when that fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3C6B0, length 97 bytes (`ret 4`).
// Callees: SetDesc @ 0x00C3C3E0 (this chunk), the +0x30 member's Create @ 0x00C4DC40 (catalog: CAudioBuffer::Create),
// Close @ 0x00C3C450 (slot 1 of vtable 0x012B4F74, called non-virtually = qualified call), global 0x0143BC2C (+0x1d4 tested).
// Label: "CShaderRenderManager::SetupProjectionWithValidation" is propagated; same no-RTTI class as 0x00C3C3E0.
// Shape: the `desc.m_kind` test is unsigned (`jbe`/`ja`); the four conditions are one && chain in source order.
// Leaf: Setup
struct SBufferDesc {
    unsigned long m_format;
    unsigned long m_kind;       // +0x04 (0..6 valid)
    long          m_b;
    unsigned long m_size;
    long          m_c, m_d;
    void*         m_pBuffer;
};
struct CSoundCaps { unsigned char _pad[0x1d4]; void* m_pHardware; };
extern CSoundCaps* g_pSoundCaps;   // 0x0143BC2C
struct CAudioBuffer { long Create(const SBufferDesc* desc); };   // 0x00C4DC40

struct CVtable_012b4f74 {
    virtual void v00();
    virtual void Close();       // slot 1, 0x00C3C450
    unsigned char _pad[0xc];
    SBufferDesc   m_desc;       // +0x10
    unsigned long m_state;      // +0x2c
    CAudioBuffer  m_audio;      // +0x30
    long SetDesc(const SBufferDesc& desc);   // 0x00C3C3E0
    long Setup(const SBufferDesc& desc);
};

long CVtable_012b4f74::Setup(const SBufferDesc& desc)
{
    if (desc.m_kind > 6)
        return -3;
    long r = SetDesc(desc);
    if (r >= 0 && g_pSoundCaps && g_pSoundCaps->m_pHardware && desc.m_kind <= 2) {
        r = m_audio.Create(&m_desc);
        if (r < 0)
            CVtable_012b4f74::Close();
    }
    return r;
}