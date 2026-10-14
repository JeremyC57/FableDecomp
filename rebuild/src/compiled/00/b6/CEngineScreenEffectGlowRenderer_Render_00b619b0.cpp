// CEngineScreenEffectGlowRenderer::Render(const CEngineComponentRenderParams&) -- when enabled (+8): recompute the glow colour when dirty (+0xc), capture the back buffer, apply the glow, show the glow texture when one is held (+0x218), then finish
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B619B0, length 62 bytes.
// Callees: ComputeGlowColor @ 0x00B5F8C0 (on `this`, ecx passed straight through), CaptureBackBuffer @ 0x00B5FAB0, ApplyGlow @ 0x00B60F70, ShowGlowTexture @ 0x00B61690,
// the finish @ 0x00B5EC90 (the manifest label "CCombatAbilityData::TransferBinaryOut" is propagated); all on `this` (re-loaded from esi). Shape: the two flag
// bytes are tested through al; the argument is unused (`ret 4`).
struct CEngineScreenEffectGlowRenderer {
    unsigned char _pad[8];
    bool          m_enabled;                       // +0x08
    unsigned char _pad09[3];
    bool          m_colourDirty;                   // +0x0c
    unsigned char _pad0d[0x20b];
    void*         m_pGlowTexture;                  // +0x218
    void ComputeGlowColor();                       // 0x00B5F8C0
    void CaptureBackBuffer();                      // 0x00B5FAB0
    void ApplyGlow();                              // 0x00B60F70
    void ShowGlowTexture();                        // 0x00B61690
    void FinishRender();                           // 0x00B5EC90
    void Render(const void* params);
};

void CEngineScreenEffectGlowRenderer::Render(const void* params)
{
    if (m_enabled) {
        if (m_colourDirty)
            ComputeGlowColor();
        CaptureBackBuffer();
        ApplyGlow();
        if (m_pGlowTexture)
            ShowGlowTexture();
        FinishRender();
    }
}