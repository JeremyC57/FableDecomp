// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test cannot fail (main only prints PASS))
// CEnginePrimitiveManagerStaticMeshesBase::FrameStart() -- point the per-frame static-mesh shadow shader selectors at the
//   colour-buffer shader set (shadow technique 2) or the depth-buffer set (otherwise)
// Leaf: FrameStart
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B9D100, 0xb8 bytes.
// Layout: PDB (Ego_r) names; retail offsets equal the PDB (CVertexShader is 8 bytes): the seven selector pointers
//   VertexShadersStatic*Shadow +0x2f0..+0x304 and VertexShaderShadowBufferRender +0x308; the shader arrays they point at
//   +0x0d8..+0x2a8. The global shadow renderer (CEngineShadowRenderer*, 0x01436E60, as landed) holds RenderingTechnique
//   at +0x334 (PDB). Local model: no engine/ header exists for these classes.
class CVertexShader { public: unsigned long _data[2]; };       // 8 bytes (PDB)
enum EShadowRenderingTechnique { SHADOW_TECHNIQUE_2 = 2 };   // only value 2 (the colour-buffer arm) is evidenced here
class CEngineShadowRenderer { public:
    unsigned char _pad000[0x334];
    EShadowRenderingTechnique RenderingTechnique;       // +0x334
};
extern CEngineShadowRenderer* g_pShadowRenderer_01436e60;   // 0x01436E60
struct CEnginePrimitiveManagerStaticMeshesBase {
    unsigned char _pad000[0xd8];
    CVertexShader VertexShadersStaticDepthBufferShadow[4];          // +0x0d8
    CVertexShader VertexShadersStaticEnvDepthBufferShadow[4];       // +0x0f8
    CVertexShader VertexShadersStaticEnvBumpDepthBufferShadow[4];   // +0x118
    CVertexShader VertexShadersStaticDP3BumpDepthBufferShadow[4];   // +0x138
    CVertexShader VertexShadersStaticSpotDepthBufferShadow[4];      // +0x158
    CVertexShader VertexShadersStaticEnvSpotDepthBufferShadow[4];   // +0x178
    CVertexShader VertexShadersStaticEnvBumpSpotDepthBufferShadow[4]; // +0x198
    CVertexShader VertexShadersStaticDP3BumpSpotDepthBufferShadow[4]; // +0x1b8
    CVertexShader VertexShaders2DStatic, VertexShaders2DStaticEnv, VertexShaders2DStaticEnvBump, VertexShaders2DStaticDP3Bump; // +0x1d8
    CVertexShader VertexShaderDepthShadowBufferRender;              // +0x1f8
    CVertexShader VertexShaderColourShadowBufferRender;             // +0x200
    CVertexShader VertexShadersStaticColourBufferShadow[4];         // +0x208
    CVertexShader VertexShadersStaticEnvColourBufferShadow[4];      // +0x228
    CVertexShader VertexShadersStaticEnvBumpColourBufferShadow[4];  // +0x248
    CVertexShader VertexShadersStaticDP3BumpColourBufferShadow[4];  // +0x268
    CVertexShader VertexShadersStaticSpotColourBufferShadow[4];     // +0x288
    CVertexShader VertexShadersStaticEnvSpotColourBufferShadow[4];  // +0x2a8
    CVertexShader PixelShaderDiffuseFogStippleAlpha;                // +0x2c8 (CPixelShader, 8 bytes)
    CVertexShader VertexShaderOutline, VertexShaderDP3BumpOutline, VertexShaderOutlineSmoothFill, VertexShaderDP3BumpOutlineSmoothFill; // +0x2d0
    CVertexShader* VertexShadersStaticShadow;                       // +0x2f0
    CVertexShader* VertexShadersStaticEnvShadow;                    // +0x2f4
    CVertexShader* VertexShadersStaticEnvBumpShadow;                // +0x2f8
    CVertexShader* VertexShadersStaticDP3BumpShadow;                // +0x2fc
    CVertexShader* VertexShadersStaticSpotShadow;                   // +0x300
    CVertexShader* VertexShadersStaticEnvSpotShadow;                // +0x304
    CVertexShader* VertexShaderShadowBufferRender;                  // +0x308
    void FrameStart();
};
void CEnginePrimitiveManagerStaticMeshesBase::FrameStart()
{
    if (g_pShadowRenderer_01436e60->RenderingTechnique == 2) {
        VertexShaderShadowBufferRender = &VertexShaderColourShadowBufferRender;
        VertexShadersStaticShadow = VertexShadersStaticColourBufferShadow;
        VertexShadersStaticEnvShadow = VertexShadersStaticEnvColourBufferShadow;
        VertexShadersStaticEnvBumpShadow = VertexShadersStaticEnvBumpColourBufferShadow;
        VertexShadersStaticDP3BumpShadow = VertexShadersStaticDP3BumpColourBufferShadow;
        VertexShadersStaticSpotShadow = VertexShadersStaticSpotColourBufferShadow;
        VertexShadersStaticEnvSpotShadow = VertexShadersStaticEnvSpotColourBufferShadow;
    } else {
        VertexShaderShadowBufferRender = &VertexShaderDepthShadowBufferRender;
        VertexShadersStaticShadow = VertexShadersStaticDepthBufferShadow;
        VertexShadersStaticEnvShadow = VertexShadersStaticEnvDepthBufferShadow;
        VertexShadersStaticEnvBumpShadow = VertexShadersStaticEnvBumpDepthBufferShadow;
        VertexShadersStaticDP3BumpShadow = VertexShadersStaticDP3BumpDepthBufferShadow;
        VertexShadersStaticSpotShadow = VertexShadersStaticSpotDepthBufferShadow;
        VertexShadersStaticEnvSpotShadow = VertexShadersStaticEnvSpotDepthBufferShadow;
    }
}