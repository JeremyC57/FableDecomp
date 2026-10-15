// CEngineLightingManager::SetupShaderSpotLight() -- loads the shadowed spot light's falloff (matrix from a copy of the shadow renderer's scene data, the radius-ratio scale and the light colour) into the shader render manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B47310, length 116 bytes (128 with padding).
// Callees: CShadowRendererSceneData::CShadowRendererSceneData(const CShadowRendererSceneData&) @ 0x00B38600 (out-of-line copy
// constructor, `rep movsd` blocks), CShaderRenderManager::SetShadowSpotLightFalloffParameters(const CMatrix3x4*, float, float,
// const CRGBFloatColour*) @ 0x009899B0 (on the shader render manager @ 0x01436E14). Globals: the shadow renderer @ 0x01436E60
// (SceneData at +0x0C), the 1.0f pool constant @ 0x0122DED8.
// Shape: a 0x240-byte scene-data local copied from the renderer (`lea ecx,[esp+0Ch]` after pushing the source), then
// ratio = SpotRadius / Radius and scale = 1 / (1 / ratio^2 - 1) -- a named float local homed in the frame word below the
// scene copy (fstp, then re-read as a dword for the push and through the FPU for `-scale`); the arguments are the copy's
// SpotLightFalloffMatrix (+0xC0), scale, -scale and the spot light's colour.
// Layout: the spotlight parameter block (PDB CEngineLightingManagerSpotlightParams, byte-packed) starts at +0x4698 in the TLC
// object (the landed UpdateShadowScene @ 0x00B444F0 passes it), so Radius / SpotRadius / Colour are +0x46B4 / +0x46B8 / +0x46C0.
struct CMatrix3x4 { float m[12]; };
struct CMatrix4x4 { float m[16]; };
struct C3DVector { float X, Y, Z; };
struct CRGBFloatColour { float R, G, B, A; };

struct CShadowRendererSceneData {                  // 0x240 bytes
    CMatrix4x4    WorldToShadowBufferMatrix;       // +0x000
    CMatrix4x4    WorldToShadowBufferMatrixHomogenousScreenSpace;   // +0x040
    CMatrix4x4    WorldToShadowBufferMatrixTextureSpace;            // +0x080
    CMatrix3x4    SpotLightFalloffMatrix;          // +0x0C0
    float         ShadowFadeStart;                 // +0x0F0
    float         ShadowFadeEnd;                   // +0x0F4
    C3DVector     ShadowLightVector;               // +0x0F8
    C3DVector     CameraPos;                       // +0x104
    unsigned char CasterClipVolume[0x98];          // +0x110 CShadowClipVolume
    unsigned char RecieverClipVolume[0x98];        // +0x1A8 CShadowClipVolume
    CShadowRendererSceneData(const CShadowRendererSceneData& other);   // 0x00B38600
};

struct CEngineShadowRenderer {
    unsigned char            _pad00[0xc];
    CShadowRendererSceneData SceneData;            // +0x0C
};
extern CEngineShadowRenderer* g_pShadowRenderer_01436e60;   // 0x01436E60

struct CShaderRenderManager {
    void SetShadowSpotLightFalloffParameters(const CMatrix3x4* falloff, float scale, float offset,
                                             const CRGBFloatColour* colour);   // 0x009899B0
};
extern CShaderRenderManager* g_pShaderRenderManager_01436e14;   // 0x01436E14

#pragma pack(push, 1)
struct CEngineLightingManagerSpotlightParams {     // 0x38 bytes, byte-packed (PDB)
    bool            Active;                        // +0x00
    C3DVector       Pos;                           // +0x01
    C3DVector       Vector;                        // +0x0D
    unsigned char   _pad19[3];
    float           Radius;                        // +0x1C
    float           SpotRadius;                    // +0x20
    float           ConeWidth;                     // +0x24
    CRGBFloatColour Colour;                        // +0x28
};
#pragma pack(pop)

struct CEngineLightingManager {
    unsigned char                         _pad0000[0x4698];
    CEngineLightingManagerSpotlightParams SpotLightParams;   // +0x4698
    void SetupShaderSpotLight();
};

void CEngineLightingManager::SetupShaderSpotLight()
{
    CShadowRendererSceneData scene(g_pShadowRenderer_01436e60->SceneData);
    float ratio = SpotLightParams.SpotRadius / SpotLightParams.Radius;
    float scale = 1.0f / (1.0f / (ratio * ratio) - 1.0f);
    g_pShaderRenderManager_01436e14->SetShadowSpotLightFalloffParameters(&scene.SpotLightFalloffMatrix, scale, -scale,
                                                                         &SpotLightParams.Colour);
}
