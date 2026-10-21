// CEngineWaterRenderer::SetupBackgroundShaderConstants() -- upload the background water shader state: user constant 3 = half the bump-matrix factor (0 when bump mapping is off), the four background bone matrices (other-background vars, shore lookup mask, mask-for-bounds, mask 2), then the inlined CShaderRenderManager::PrepareForRender(0x604)
// Leaf: SetupBackgroundShaderConstants
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B7F600, length 383 bytes, plain `ret`.
// Label correction: catalogued "CEngineWeatherRenderer::RenderSnowBox" (propagated; the real RenderSnowBox is the landed
//   0x00B54210 and takes a C3DVector). Every member it reads is CEngineWaterRenderer::Settings (CEngineWaterSettings) and it
//   sits in the water renderer's TU (0x00B7ED70 PeekSceneFilterFlags, 0x00B7F0F0 RenderBackground). It has no direct
//   callers or vtable slot in retail, so the PDB name is unknown: the name here is descriptive.
// Callees: CShaderRenderManager::SetUserVertexShaderConstant @ 0x00989B00, the BoneMatrices CArray<CPreTransposedBoneMatrix>
//   resize(n, const T&) @ 0x00B7EDF0 (48-byte elements; manifest label propagated), SetBoneTransform(unsigned long,
//   const CMatrix3x4*) @ 0x0098A7F0, and the PrepareForRender inline of the landed RenderSnowBox 0x00B54210: SetVSConstants
//   @ 0x0098B930, UpdatePixelShader @ 0x00988A20, UpdatePixelShaderConstants @ 0x00989BF0, UpdateBoneTransform @ 0x0098A8E0,
//   UpdateWorldSpaceFogTransform @ 0x00989640, UpdateAmbient @ 0x00989760, UpdateFogColour @ 0x009897C0,
//   UpdateLightGlobals @ 0x0098A760, ResetWorldTransform @ 0x00988290, UpdateCombinedProjectionTransform @ 0x00988A50,
//   UpdateCameraPos @ 0x00989700. Global: the shader render manager @ 0x01436E14; float pool 0x0122F59C = 0.5f.
// Layout: Ego_r.pdb CEngineWaterRenderer with 12-byte retail CArrays (13 arrays before Settings: PDB +0x2b4 -> retail
//   +0x280): Settings.EnableBumpMap +0x28d, BumpMatFactor +0x2dc, ShoreLookupMaskMatrix +0x414, ShoreLookupMaskMatrix2
//   +0x444, ShoreLookupMaskMatrixForBounds +0x474, OtherBackgroundShaderVars +0x504. engine/CEngineWaterRenderer.h has the
//   PDB offsets, so the class is modelled locally. CShaderRenderManager as in 0x00B54210 (BoneMatrices +0x3dc).
// Shape: `float factor = 0; if (...) factor = ...;` keeps the zero store before the branch and the frame slot; resize(4)
//   passes an uninitialised 48-byte default-constructed temporary (the 0x30 bytes of frame above the float).
struct C4DVector { float X, Y, Z, W; };
struct CMatrix3x4 { float M[12]; };
struct CPreTransposedBoneMatrix {
    float M[12];
    CPreTransposedBoneMatrix() { }
};

template <class T> struct CArray {
    T* First;
    T* Last;
    T* End;
    unsigned long Size() const { return Last - First; }
    const T* GetData() const { return Size() > 0 ? First : (const T*)this; }
    void resize(unsigned int n, const T& value);        // 0x00B7EDF0 for CPreTransposedBoneMatrix
};

struct CShaderConstantRange {
    void* _vptr;                                        // +0
    long Offset;                                        // +4
    unsigned long Count;                                // +8
};

struct CVertexShaderConstantLayout {
    void* _vptr;                                        // +0x00
    CShaderConstantRange Reserved;                      // +0x04
    unsigned char _pad10[0x130];                        // +0x10 the other ranges
    CArray<C4DVector> ReservedPresets;                  // +0x140
};

class CShaderRenderManager {
public:
    void SetUserVertexShaderConstant(unsigned long index, float x, float y, float z, float w);   // 0x00989B00
    void SetBoneTransform(unsigned long index, const CMatrix3x4* matrix);                        // 0x0098A7F0

    __forceinline void PrepareForRender(unsigned long flags)
    {
        if (UpdateFlags & 4)
            SetVSConstants(CurrentVSConstantLayout->Reserved.Offset, CurrentVSConstantLayout->ReservedPresets.GetData(),
                           CurrentVSConstantLayout->ReservedPresets.Size());
        if (UpdateFlags & 1)
            UpdatePixelShader();
        if (UpdateFlags & 2)
            UpdatePixelShaderConstants();
        if ((flags & 0x400) && (UpdateFlags & 0x20))
            UpdateBoneTransform();
        if (flags & 0x40) {
            if (UpdateFlags & 0x1000)
                UpdateWorldSpaceTextureTransform();
        } else if (flags & 0x80) {
            if (UpdateFlags & 0x800)
                UpdateObjectSpaceTextureTransform();
        }
        if (flags & 0x200) {
            if (UpdateFlags & 0x2000)
                UpdateWorldSpaceFogTransform();
        } else if (flags & 0x100) {
            if (UpdateFlags & 0x4000)
                UpdateObjectSpaceFogTransform();
        }
        if (UpdateFlags & 4)
            UpdateAmbient();
        if (UpdateFlags & 0x20000)
            UpdateFogColour();
        if (UpdateFlags & 0x400)
            UpdateLightGlobals();
        if (flags & 8) {
            if (UpdateFlags & 0x100)
                UpdateObjectSpaceLighting(flags);
        } else if (flags & 0x10) {
            if (UpdateFlags & 0x200)
                UpdateWorldSpaceLighting();
        }
        if ((flags & 0x20) && (UpdateFlags & 0x8000))
            UpdateLocalisedSphereMapping();
        if (!(flags & 1) && !WorldMatrixIsIdentity)
            ResetWorldTransform();
        if (UpdateFlags & 0x80)
            UpdateCombinedProjectionTransform();
        if ((flags & 4) && (UpdateFlags & 8))
            UpdateCameraPos();
    }

    void* _vptr;                                        // +0x000
    CVertexShaderConstantLayout* CurrentVSConstantLayout;   // +0x004
    unsigned char _pad008[0x1e0];                       // +0x008
    bool WorldMatrixIsIdentity;                         // +0x1e8
    unsigned char _pad1e9[0x1ef];                       // +0x1e9
    unsigned long UpdateFlags;                          // +0x3d8
    CArray<CPreTransposedBoneMatrix> BoneMatrices;      // +0x3dc

private:
    void SetVSConstants(long reg, const void* data, unsigned long count);   // 0x0098B930
    void UpdatePixelShader();                           // 0x00988A20
    void UpdatePixelShaderConstants();                  // 0x00989BF0
    void UpdateBoneTransform();                         // 0x0098A8E0
    void UpdateWorldSpaceTextureTransform();            // 0x009894B0
    void UpdateObjectSpaceTextureTransform();           // 0x00989230
    void UpdateWorldSpaceFogTransform();                // 0x00989640
    void UpdateObjectSpaceFogTransform();               // 0x00989540
    void UpdateAmbient();                               // 0x00989760
    void UpdateFogColour();                             // 0x009897C0
    void UpdateLightGlobals();                          // 0x0098A760
    void UpdateObjectSpaceLighting(unsigned long flags);   // 0x00989CB0
    void UpdateWorldSpaceLighting();                    // 0x0098A540
    void UpdateLocalisedSphereMapping();                // 0x00988E80
    void ResetWorldTransform();                         // 0x00988290
    void UpdateCombinedProjectionTransform();           // 0x00988A50
    void UpdateCameraPos();                             // 0x00989700
};
extern CShaderRenderManager* g_pShaderRenderManager;    // 0x01436E14

struct CEngineWaterSettings {                           // PDB CEngineWaterSettings (0x2e4)
    unsigned char _pad00[0xd];
    bool          EnableBumpMap;                        // +0x0d
    unsigned char _pad0e[0x5c - 0x0e];
    float         BumpMatFactor;                        // +0x5c
    unsigned char _pad60[0x194 - 0x60];
    CMatrix3x4    ShoreLookupMaskMatrix;                // +0x194
    CMatrix3x4    ShoreLookupMaskMatrix2;               // +0x1c4
    CMatrix3x4    ShoreLookupMaskMatrixForBounds;       // +0x1f4
    CMatrix3x4    OtherShaderVars;                      // +0x224
    CMatrix3x4    OtherSeaShaderVars;                   // +0x254
    CMatrix3x4    OtherBackgroundShaderVars;            // +0x284
    CMatrix3x4    OtherSeaBackgroundShaderVars;         // +0x2b4
};

struct CEngineWaterRenderer {
    unsigned char        _pad000[0x280];                // vptr, buffers, shaders, render lists (retail 12-byte CArrays)
    CEngineWaterSettings Settings;                      // +0x280
    void SetupBackgroundShaderConstants();
};

void CEngineWaterRenderer::SetupBackgroundShaderConstants()
{
    float factor = 0.0f;
    if (Settings.EnableBumpMap)
        factor = Settings.BumpMatFactor * 0.5f;
    g_pShaderRenderManager->SetUserVertexShaderConstant(3, factor, factor, factor, factor);
    g_pShaderRenderManager->BoneMatrices.resize(4, CPreTransposedBoneMatrix());
    g_pShaderRenderManager->SetBoneTransform(0, &Settings.OtherBackgroundShaderVars);
    g_pShaderRenderManager->SetBoneTransform(1, &Settings.ShoreLookupMaskMatrix);
    g_pShaderRenderManager->SetBoneTransform(2, &Settings.ShoreLookupMaskMatrixForBounds);
    g_pShaderRenderManager->SetBoneTransform(3, &Settings.ShoreLookupMaskMatrix2);
    g_pShaderRenderManager->PrepareForRender(0x604);
}
