// CCombatRenderer::SetRenderScaleWithFallback() -- try the configured scale and each fallback until one applies; with no configuration, scale 4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3F710, length 68 bytes.
// Callees: CCombatRenderer::SetRenderScale(long) @ 0x00C3F110 (bool; manifest "CCombatRenderer_SetRenderScale" is a
// placeholder label), CCombatRenderer::GetFallbackRenderScale(long) @ 0x00C3F280 (long, 0 ends the chain; the
// manifest's "Render_QualityLevelAdjustment" is a placeholder). The configuration object is the pointer at
// +0x188 whose +4 is the first scale. No PDB layout exists for CCombatRenderer (local model).
// Shape: `if (!config) return SetRenderScale(4);` passes the callee's al straight through (`call ; pop edi ;
// ret`); the loop `for (scale = first; scale; scale = Fallback(scale)) if (Set(scale)) return true; return false;`
// with `mov al,1` / `xor al,al` tails.
struct CRenderScaleConfig {
    unsigned char _pad[4];
    long m_firstScale;                          // +0x04
};

struct CCombatRenderer {
    unsigned char _pad[0x188];
    CRenderScaleConfig* m_pScaleConfig;         // +0x188
    bool SetRenderScale(long scale);            // 0x00C3F110
    long GetFallbackRenderScale(long scale);    // 0x00C3F280
    bool SetRenderScaleWithFallback();
};

bool CCombatRenderer::SetRenderScaleWithFallback()
{
    CRenderScaleConfig* config = m_pScaleConfig;
    if (config != 0) {                           // the loop is the fall-through block, the null case sits last
        long scale = config->m_firstScale;
        while (scale != 0) {
            if (SetRenderScale(scale))
                return true;
            scale = GetFallbackRenderScale(scale);
        }
        return false;
    }
    return SetRenderScale(4);
}