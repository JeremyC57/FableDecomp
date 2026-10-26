// CEngineShadowRenderer::SetShadowMapSize(long width, long height) -- store the two size words (+0x338, +0x33c), release the default-pool resources and rebuild the textures
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B4FC10, length 39 bytes (+ int3 padding).
// Callees: CEngineShadowRenderer::OnReleaseDefaultPoolResources @ 0x00B4B830, CEngineShadowRenderer::InitialiseTextures @ 0x00B4F8E0.
// The chunk label "CInputProcessGameBase::AddToInterface" is propagated (the callees are the shadow renderer's own members).
// Shape: the first argument is loaded before `push esi`, the second after; the compiler stores the height word first although the
// source assigns width then height (the loads follow the source order, the stores are swapped), `ret 8`.
class CEngineShadowRenderer { public:
    unsigned char _pad[0x338];
    long m_shadowMapWidth;                          // +0x338
    long m_shadowMapHeight;                         // +0x33c
    void OnReleaseDefaultPoolResources();           // 0x00B4B830
    bool InitialiseTextures();                      // 0x00B4F8E0
    void SetShadowMapSize(long width, long height);
};

void CEngineShadowRenderer::SetShadowMapSize(long width, long height)
{
    m_shadowMapWidth = width;
    m_shadowMapHeight = height;
    OnReleaseDefaultPoolResources();
    InitialiseTextures();
}