// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test cannot fail (main only prints PASS))
// CEnginePrimitiveManagerAnimatedMeshesBase::FrameStart() -- point the per-frame palette-skin shadow shader selectors at
//   the colour-buffer shader set (shadow technique 2) or the depth-buffer set (otherwise)
// Leaf: FrameStart
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BCEA60, 0xd0 bytes.
// Layout: PDB (Ego_r) names; retail offsets equal the PDB (CVertexShader is 8 bytes): the selector pointers
//   VertexShadersPalSkinShadowBufferRender +0x2e8 .. VertexShadersPalSkinDP3BumpSpotShadow +0x308; the shader arrays they
//   point at +0x098..+0x268. Global shadow renderer as in 0x00B9D100. Local model: no engine/ header exists.
struct CVertexShader { unsigned long _data[2]; };       // 8 bytes (PDB)
enum EShadowRenderingTechnique { SHADOW_TECHNIQUE_2 = 2 };   // only value 2 (the colour-buffer arm) is evidenced here
struct CEngineShadowRenderer {
    unsigned char _pad000[0x334];
    EShadowRenderingTechnique RenderingTechnique;       // +0x334
};
extern CEngineShadowRenderer* g_pShadowRenderer_01436e60;   // 0x01436E60
struct CEnginePrimitiveManagerAnimatedMeshesBase {
    unsigned char _pad000[0x98];
    CVertexShader VertexShadersPalSkinDepthBufferShadow[4];             // +0x098
    CVertexShader VertexShadersPalSkinEnvDepthBufferShadow[4];          // +0x0b8
    CVertexShader VertexShadersPalSkinEnvBumpDepthBufferShadow[4];      // +0x0d8
    CVertexShader VertexShadersPalSkinDP3BumpDepthBufferShadow[4];      // +0x0f8
    CVertexShader VertexShadersPalSkinSpotDepthBufferShadow[4];         // +0x118
    CVertexShader VertexShadersPalSkinEnvSpotDepthBufferShadow[4];      // +0x138
    CVertexShader VertexShadersPalSkinEnvBumpSpotDepthBufferShadow[4];  // +0x158
    CVertexShader VertexShadersPalSkinDP3BumpSpotDepthBufferShadow[4];  // +0x178
    CVertexShader VertexShadersPalSkinDepthShadowBufferRender;          // +0x198
    CVertexShader VertexShadersPalSkinColourShadowBufferRender;         // +0x1a0
    CVertexShader VertexShadersPalSkinColourBufferShadow[4];            // +0x1a8
    CVertexShader VertexShadersPalSkinEnvColourBufferShadow[4];         // +0x1c8
    CVertexShader VertexShadersPalSkinEnvBumpColourBufferShadow[4];     // +0x1e8
    CVertexShader VertexShadersPalSkinDP3BumpColourBufferShadow[4];     // +0x208
    CVertexShader VertexShadersPalSkinSpotColourBufferShadow[4];        // +0x228
    CVertexShader VertexShadersPalSkinEnvSpotColourBufferShadow[4];     // +0x248
    CVertexShader VertexShadersPalSkinEnvBumpSpotColourBufferShadow[4]; // +0x268
    CVertexShader VertexShadersPalSkinDP3BumpSpotColourBufferShadow[4]; // +0x288
    CVertexShader VertexShaderPalSkinOutline, VertexShaderPalSkinDP3BumpOutline, VertexShaderPalSkinOutlineSmoothFill, VertexShaderPalSkinDP3BumpOutlineSmoothFill; // +0x2a8
    CVertexShader VertexShaders2DPalSkin, VertexShaders2DPalSkinEnv, VertexShaders2DPalSkinEnvBump, VertexShaders2DPalSkinDP3Bump; // +0x2c8
    CVertexShader* VertexShadersPalSkinShadowBufferRender;              // +0x2e8
    CVertexShader* VertexShadersPalSkinShadow;                          // +0x2ec
    CVertexShader* VertexShadersPalSkinEnvShadow;                       // +0x2f0
    CVertexShader* VertexShadersPalSkinEnvBumpShadow;                   // +0x2f4
    CVertexShader* VertexShadersPalSkinDP3BumpShadow;                   // +0x2f8
    CVertexShader* VertexShadersPalSkinSpotShadow;                      // +0x2fc
    CVertexShader* VertexShadersPalSkinEnvSpotShadow;                   // +0x300
    CVertexShader* VertexShadersPalSkinEnvBumpSpotShadow;               // +0x304
    CVertexShader* VertexShadersPalSkinDP3BumpSpotShadow;               // +0x308
    void FrameStart();
};
void CEnginePrimitiveManagerAnimatedMeshesBase::FrameStart()
{
    // retail sets eight selectors in this order; VertexShadersPalSkinDP3BumpSpotShadow (+0x308) is not touched here
    if (g_pShadowRenderer_01436e60->RenderingTechnique == 2) {
        VertexShadersPalSkinShadowBufferRender = &VertexShadersPalSkinColourShadowBufferRender;
        VertexShadersPalSkinShadow = VertexShadersPalSkinColourBufferShadow;
        VertexShadersPalSkinSpotShadow = VertexShadersPalSkinSpotColourBufferShadow;
        VertexShadersPalSkinEnvShadow = VertexShadersPalSkinEnvColourBufferShadow;
        VertexShadersPalSkinEnvSpotShadow = VertexShadersPalSkinEnvSpotColourBufferShadow;
        VertexShadersPalSkinEnvBumpShadow = VertexShadersPalSkinEnvBumpColourBufferShadow;
        VertexShadersPalSkinEnvBumpSpotShadow = VertexShadersPalSkinEnvBumpSpotColourBufferShadow;
        VertexShadersPalSkinDP3BumpShadow = VertexShadersPalSkinDP3BumpColourBufferShadow;
    } else {
        VertexShadersPalSkinShadowBufferRender = &VertexShadersPalSkinDepthShadowBufferRender;
        VertexShadersPalSkinShadow = VertexShadersPalSkinDepthBufferShadow;
        VertexShadersPalSkinSpotShadow = VertexShadersPalSkinSpotDepthBufferShadow;
        VertexShadersPalSkinEnvShadow = VertexShadersPalSkinEnvDepthBufferShadow;
        VertexShadersPalSkinEnvSpotShadow = VertexShadersPalSkinEnvSpotDepthBufferShadow;
        VertexShadersPalSkinEnvBumpShadow = VertexShadersPalSkinEnvBumpDepthBufferShadow;
        VertexShadersPalSkinEnvBumpSpotShadow = VertexShadersPalSkinEnvBumpSpotDepthBufferShadow;
        VertexShadersPalSkinDP3BumpShadow = VertexShadersPalSkinDP3BumpDepthBufferShadow;
    }
}