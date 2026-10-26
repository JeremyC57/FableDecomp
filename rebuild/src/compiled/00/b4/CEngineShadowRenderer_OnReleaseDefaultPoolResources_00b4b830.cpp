// CEngineShadowRenderer::OnReleaseDefaultPoolResources() -- when the default-pool resources are held (+0x2af): uninitialise the render target at +0x2c4, release the two shader holders at +0x308 / +0x2f8 and the two buffers at +0x318 / +0x2e8, then clear the flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B4B830, length 77 bytes.
// Callees: CRenderTarget::Uninitialise @ 0x00A10E30, the holder release @ 0x009F2E20 twice, the buffer release @ 0x009F9F70 twice (each on the member by address).
// Shape: five `lea ecx ; call` pairs in that order, then an immediate byte clear.
class CRenderTarget { public:
    unsigned char _body[0x24];
    void Uninitialise();                           // 0x00A10E30
};

struct CShaderHolder_009f2e20 {
    unsigned char _body[0x10];
    void Release();                                // 0x009F2E20
};

struct CBufferHolder_009f9f70 {
    unsigned char _body[0xc];
    void Release();                                // 0x009F9F70
};

class CEngineShadowRenderer { public:
    unsigned char          _pad[0x2af];
    bool                   m_defaultPoolHeld;      // +0x2af
    unsigned char          _pad2b0[0x14];
    CRenderTarget          m_renderTarget;         // +0x2c4
    CBufferHolder_009f9f70 m_bufferA;              // +0x2e8
    unsigned char          _pad2f4[4];
    CShaderHolder_009f2e20 m_shaderA;              // +0x2f8
    CShaderHolder_009f2e20 m_shaderB;              // +0x308
    CBufferHolder_009f9f70 m_bufferB;              // +0x318
    void OnReleaseDefaultPoolResources();
};

void CEngineShadowRenderer::OnReleaseDefaultPoolResources()
{
    if (m_defaultPoolHeld) {
        m_renderTarget.Uninitialise();
        m_shaderB.Release();
        m_shaderA.Release();
        m_bufferB.Release();
        m_bufferA.Release();
        m_defaultPoolHeld = false;
    }
}