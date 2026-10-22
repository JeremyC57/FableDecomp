// Grade: DIFFER(665v665) -- operator*= x87 term order: VC7.1 orders commutative fmul/fadd terms by first-reference, source form per element has no effect (12 uniform + 3 mixed forms tried)
// Leaf: SetupShaderSpotLight
struct C3DVector { float X, Y, Z; };
struct CRGBFloatColour { float R, G, B, A; };
struct CMatrix4x4 { float m[16]; };
struct CMatrix3x4 {
    float m[12];
    __forceinline CMatrix3x4& operator*=(const CMatrix3x4& b)
    {
        CMatrix3x4 a(*this);
        m[0] = a.m[2] * b.m[6] + a.m[1] * b.m[3] + a.m[0] * b.m[0];
        m[1] = a.m[2] * b.m[7] + a.m[1] * b.m[4] + a.m[0] * b.m[1];
        m[2] = a.m[2] * b.m[8] + a.m[1] * b.m[5] + a.m[0] * b.m[2];
        m[3] = a.m[5] * b.m[6] + a.m[4] * b.m[3] + a.m[3] * b.m[0];
        m[4] = a.m[5] * b.m[7] + a.m[4] * b.m[4] + a.m[3] * b.m[1];
        m[5] = a.m[5] * b.m[8] + a.m[4] * b.m[5] + a.m[3] * b.m[2];
        m[6] = a.m[8] * b.m[6] + a.m[7] * b.m[3] + a.m[6] * b.m[0];
        m[7] = a.m[8] * b.m[7] + a.m[7] * b.m[4] + a.m[6] * b.m[1];
        m[8] = a.m[8] * b.m[8] + a.m[7] * b.m[5] + a.m[6] * b.m[2];
        m[9] = a.m[11] * b.m[6] + a.m[10] * b.m[3] + a.m[9] * b.m[0] + b.m[9];
        m[10] = a.m[11] * b.m[7] + a.m[10] * b.m[4] + a.m[9] * b.m[1] + b.m[10];
        m[11] = a.m[11] * b.m[8] + a.m[10] * b.m[5] + a.m[9] * b.m[2] + b.m[11];
        return *this;
    }
};

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
    void SetupShaderSpotLight(const CMatrix3x4& transform);
};
void CEngineLightingManager::SetupShaderSpotLight(const CMatrix3x4& transform)
{
    CShadowRendererSceneData scene(g_pShadowRenderer_01436e60->SceneData);
    float ratio = SpotLightParams.SpotRadius / SpotLightParams.Radius;
    float scale = 1.0f / (1.0f / (ratio * ratio) - 1.0f);
    CMatrix3x4 falloff = transform;
    falloff *= scene.SpotLightFalloffMatrix;
    g_pShaderRenderManager_01436e14->SetShadowSpotLightFalloffParameters(&falloff, scale, -scale, &SpotLightParams.Colour);
}
