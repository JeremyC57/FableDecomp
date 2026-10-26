// CEngineWaterRenderer::SetupForegroundShaderConstantsAndFog() -- set up the water foreground shader state: user constant 0 = swell (when enabled), 1 = soft-edge span, the four foreground bone matrices, pixel constant 5 = the clamped fresnel factor; then the inlined CShaderRenderManager::PrepareForRender(0x604)
// Leaf: SetupForegroundShaderConstantsAndFog
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B7EEC0, length 560 bytes (+ int3 padding), plain `ret`.
// Label correction: catalogued "CEngineWeatherRenderer::RenderSnowBox" (propagated; the real RenderSnowBox is the landed
//   0x00B54210, `ret 4` with a C3DVector). Every member it reads is CEngineWaterRenderer::Settings and it sits in the water
//   renderer's TU next to 0x00B7F0F0/0x00B7F600; it has no direct callers. The name is the PDB's SetupForegroundShaderConstantsAndFog (inferred from the
//   bone/constant set: swell + soft edge, no sea colour), not proven.
// Callees: SetUserVertexShaderConstant @ 0x00989B00, the BoneMatrices resize @ 0x00B7EDF0, SetBoneTransform @ 0x0098A7F0
//   and the PrepareForRender inline of the landed 0x00B7F600 (same TU). Float pool: 0.25f @ 0x0123078C, 0.0f @ 0x0122DEDC,
//   1.0f @ 0x0122DED8, 0.5f @ 0x0122F59C.
// Layout: as 0x00B7FA20 (retail Settings +0x280): EnableBumpMap +0x0d, EnableSwell +0x0e, EnableFresnel +0x13, FresnelFactor
//   +0x18, SwellFactor +0x1c, ShoreSwellFactor +0x20, BumpMatFactor +0x5c, SoftEdgeSpanFactor +0x68, RefOffset +0x88,
//   OtherShaderVars +0x224.
// Shape: the fresnel clamp is `if (EnableFresnel) f = Clamp(FresnelFactor * 0.25f, 0, 1); else f = 0;` (retail tail-merges the
//   else with the low clamp and puts the high clamp after the `ret`).
class C4DVector { public:
    float X, Y, Z, W;
    C4DVector() { }
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
};
class CRGBFloatColour { public: float R, G, B, A; };
inline C4DVector operator*(const C4DVector& a, const CRGBFloatColour& c) { return C4DVector(a.X * c.R, a.Y * c.G, a.Z * c.B, a.W * c.A); }
class CMatrix3x4 { public: float M[12]; };
struct CPreTransposedBoneMatrix {
    float M[12];
    CPreTransposedBoneMatrix() { }
};

template <class T> class CArray { public:
    T* First;
    T* Last;
    T* End;
    unsigned long Size() const { return Last - First; }
    const T* GetData() const { return Size() > 0 ? First : (const T*)this; }
    void resize(unsigned int n, const T& value);        // 0x00B7EDF0 for CPreTransposedBoneMatrix
};

class CShaderConstantRange { public:
    void* _vptr;                                        // +0
    long Offset;                                        // +4
    unsigned long Count;                                // +8
};

class CVertexShaderConstantLayout { public:
    void* _vptr;                                        // +0x00
    CShaderConstantRange Reserved;                      // +0x04
    unsigned char _pad10[0x130];                        // +0x10 the other ranges
    CArray<C4DVector> ReservedPresets;                  // +0x140
};

class CShaderRenderManager {
public:
    void SetUserVertexShaderConstant(unsigned long index, float x, float y, float z, float w);   // 0x00989B00
    void SetBoneTransform(unsigned long index, const CMatrix3x4* matrix);                        // 0x0098A7F0
    void SetPixelShaderConstant(unsigned long index, const C4DVector& value)
    {
        *(PSConstTable.First + index) = value;
        UpdateFlags |= 2;
    }

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
    unsigned char _pad1e9[0x1e3];                       // +0x1e9
    CArray<C4DVector> PSConstTable;                     // +0x3cc
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

inline float Clamp(float value, float lo, float hi)
{
    if (value < lo)
        return lo;
    if (value > hi)
        return hi;
    return value;
}

class CEngineWaterSettings { public:                           // PDB CEngineWaterSettings (0x2e4)
    unsigned char   _pad00[0xd];
    bool            EnableBumpMap;                      // +0x0d
    bool            EnableSwell;                        // +0x0e
    unsigned char   _pad0f[0x4];
    bool            EnableFresnel;                      // +0x13
    unsigned char   _pad14[0x4];
    float           FresnelFactor;                      // +0x18
    float           SwellFactor;                        // +0x1c
    float           ShoreSwellFactor;                   // +0x20
    float           SwellOscilationSpeed;               // +0x24
    CRGBFloatColour Colour;                             // +0x28
    unsigned char   _pad38[0x5c - 0x38];
    float           BumpMatFactor;                      // +0x5c
    float           SoftEdgeMultFactor;                 // +0x60
    float           SoftEdgeAddFactor;                  // +0x64
    float           SoftEdgeSpanFactor;                 // +0x68
    unsigned char   _pad6c[0x84 - 0x6c];
    float           ColourToReflectionFactor;           // +0x84
    float           RefOffset;                          // +0x88
    unsigned char   _pad8c[0x194 - 0x8c];
    CMatrix3x4      ShoreLookupMaskMatrix;              // +0x194
    CMatrix3x4      ShoreLookupMaskMatrix2;             // +0x1c4
    CMatrix3x4      ShoreLookupMaskMatrixForBounds;     // +0x1f4
    CMatrix3x4      OtherShaderVars;                    // +0x224
    CMatrix3x4      OtherSeaShaderVars;                 // +0x254
    CMatrix3x4      OtherBackgroundShaderVars;          // +0x284
    CMatrix3x4      OtherSeaBackgroundShaderVars;       // +0x2b4
};

class CEngineWaterRenderer { public:
    unsigned char        _pad000[0x280];                // vptr, buffers, shaders, render lists (retail 12-byte CArrays)
    CEngineWaterSettings Settings;                      // +0x280
    void SetupForegroundShaderConstantsAndFog();
};

void CEngineWaterRenderer::SetupForegroundShaderConstantsAndFog()
{
    if (Settings.EnableSwell)
        g_pShaderRenderManager->SetUserVertexShaderConstant(0, Settings.SwellFactor, Settings.ShoreSwellFactor, Settings.RefOffset, 0.0f);
    else
        g_pShaderRenderManager->SetUserVertexShaderConstant(0, 0.0f, 0.0f, 0.0f, 0.0f);
    g_pShaderRenderManager->SetUserVertexShaderConstant(1, Settings.SoftEdgeSpanFactor, 0.0f, 0.0f, 0.0f);
    g_pShaderRenderManager->BoneMatrices.resize(4, CPreTransposedBoneMatrix());
    g_pShaderRenderManager->SetBoneTransform(0, &Settings.OtherShaderVars);
    g_pShaderRenderManager->SetBoneTransform(1, &Settings.ShoreLookupMaskMatrix);
    g_pShaderRenderManager->SetBoneTransform(2, &Settings.ShoreLookupMaskMatrixForBounds);
    g_pShaderRenderManager->SetBoneTransform(3, &Settings.ShoreLookupMaskMatrix2);
    float fresnel;
    if (Settings.EnableFresnel)
        fresnel = Clamp(Settings.FresnelFactor * 0.25f, 0.0f, 1.0f);
    else
        fresnel = 0.0f;
    g_pShaderRenderManager->SetPixelShaderConstant(5, C4DVector(fresnel, fresnel, fresnel, fresnel));
    g_pShaderRenderManager->PrepareForRender(0x604);
}