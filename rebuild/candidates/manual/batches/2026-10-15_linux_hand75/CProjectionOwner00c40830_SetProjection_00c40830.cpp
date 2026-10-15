// CProjectionOwner_00c40830::SetProjection(const SProjectionBlock* projection) -- copies a 0x6c-byte projection block into +0x0c, raises the twenty low dirty bits of +0x78 and bit 24 of the global render-flags word (0x0143BC28)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C40830, length 45 bytes.
// Leaf: SetProjection
// The manifest label "CShaderRenderManager::SetProjectionTransform" is propagated (the copied block is 27 dwords,
// not a CMatrix4x4, and the PDB class has no such members); class and method names are descriptive.
// Shape: `this` parked in eax for the `rep movsd` (ecx = 1Bh), `or dword ptr [eax+78h],0FFFFFh`, then the
// global flag update split into load / or / store around the register pops.
struct SProjectionBlock {
    unsigned long m_words[27];                  // 0x6c bytes copied as one block
};

extern unsigned long g_renderFlags_0143bc28;    // 0x0143BC28 (bit 24: projection changed)

struct CProjectionOwner_00c40830 {
    unsigned char    _pad00[0x0c];
    SProjectionBlock m_projection;              // +0x0c
    unsigned long    m_dirtyFlags;              // +0x78

    void SetProjection(const SProjectionBlock* projection);
};

void CProjectionOwner_00c40830::SetProjection(const SProjectionBlock* projection)
{
    m_projection = *projection;
    m_dirtyFlags |= 0xfffff;
    g_renderFlags_0143bc28 |= 0x1000000;
}
