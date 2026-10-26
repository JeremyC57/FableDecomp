// CEngineWaterRenderer::SetupBackgroundShaderConstantsAndFog() -- set up the background water shader state: the four background bone matrices, then (high-end water) user constant 3 from the bump factor or (low-end) pixel constants 5/6 from two static colours, the second modulated by the lighting theme colour; finally the inlined CShaderRenderManager::PrepareForRender(0x604)
// Leaf: SetupBackgroundShaderConstantsAndFog
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B7F0F0, length 704 bytes (+ int3 padding), plain `ret`.
// Label correction: catalogued "CEngineWaterRenderer::RenderBackground", but the PDB RenderBackground takes a float (`ret 4`);
//   this body has no argument and no direct caller. Named after the PDB's SetupBackgroundShaderConstantsAndFog (inferred from the
//   high-end/low-end background constants; the landed 0x00B7F600 is the ice-background twin), not proven.
// Callees: the BoneMatrices CArray<CPreTransposedBoneMatrix> resize @ 0x00B7EDF0, SetBoneTransform @ 0x0098A7F0,
//   SetUserVertexShaderConstant @ 0x00989B00 and the PrepareForRender inline of the landed SetupBackgroundShaderConstants
//   0x00B7F600 (same TU). Globals: the shader render manager @ 0x01436E14, the lighting manager @ 0x01436E9C (its first
//   ThemeChannels entry starts with the theme colour). Function statics @ 0x01437898 / 0x01437888 (guard @ 0x014378A8).
// Layout: Ego_r.pdb CEngineWaterRenderer with 12-byte retail CArrays (PDB +0x2a8 -> retail +0x274 HighendWaterEnabled,
//   Settings +0x280, BumpMatFactor +0x2dc, the bone matrices as in 0x00B7F600); WaterBumpFactor PDB +0x604 -> retail +0x5c0
//   (the two tesselators are smaller in retail). CShaderRenderManager: PSConstTable +0x3cc (retail, see 0x0098B390).
// Shape: the theme colour product is a named CRGBFloatColour local built BEFORE pixel constant 5 is set, and is handed to
//   constant 6 through a C4DVector(const CRGBFloatColour&) conversion temporary: retail keeps both (z/w go through the
//   product's frame slots, x/y straight to the C4DVector) -- a C4DVector product local, by-value parameters or a user copy
//   ctor all store the product once. Float pool: -0.5f @ 0x01238798.
class CRGBFloatColour;
class C4DVector { public:
    float X, Y, Z, W;
    C4DVector() { }
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
    inline C4DVector(const CRGBFloatColour& c);
};
class CRGBFloatColour { public:
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
};
inline CRGBFloatColour operator*(const CRGBFloatColour& a, const CRGBFloatColour& b) { return CRGBFloatColour(a.R * b.R, a.G * b.G, a.B * b.B, a.A * b.A); }
inline C4DVector::C4DVector(const CRGBFloatColour& c) : X(c.R), Y(c.G), Z(c.B), W(c.A) { }
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
    void UpdateCombinedProjectionTransform();           // 0x00988A50
    void UpdateCameraPos();                             // 0x00989700
public:
    void ResetWorldTransform();   // 0x00988290
};
extern CShaderRenderManager* g_pShaderRenderManager;    // 0x01436E14

class CEngineWaterSettings { public:                           // PDB CEngineWaterSettings (0x2e4)
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

struct CThemeChannel { CRGBFloatColour Colour; };

class CEngineLightingManager { public:
    unsigned char  _pad00[0x3c];
    CThemeChannel* ThemeChannels;                       // +0x3c (CArray first pointer)
};
extern CEngineLightingManager* g_pLightingManager;      // 0x01436E9C

class CEngineWaterRenderer { public:
    unsigned char        _pad000[0x274];                // vptr, buffers, shaders, render lists (retail 12-byte CArrays)
    bool                 HighendWaterEnabled;           // +0x274
    unsigned char        _pad275[0xb];
    CEngineWaterSettings Settings;                      // +0x280
    unsigned char        _pad564[0x5c0 - 0x564];
    float                WaterBumpFactor;               // +0x5c0
    void SetupBackgroundShaderConstantsAndFog();
};

void CEngineWaterRenderer::SetupBackgroundShaderConstantsAndFog()
{
    g_pShaderRenderManager->BoneMatrices.resize(4, CPreTransposedBoneMatrix());
    g_pShaderRenderManager->SetBoneTransform(0, &Settings.OtherBackgroundShaderVars);
    g_pShaderRenderManager->SetBoneTransform(1, &Settings.ShoreLookupMaskMatrix);
    g_pShaderRenderManager->SetBoneTransform(2, &Settings.ShoreLookupMaskMatrixForBounds);
    g_pShaderRenderManager->SetBoneTransform(3, &Settings.ShoreLookupMaskMatrix2);
    if (HighendWaterEnabled) {
        float factor = WaterBumpFactor * Settings.BumpMatFactor;
        g_pShaderRenderManager->SetUserVertexShaderConstant(3, factor, 0.0f, factor * -0.5f, 1.0f);
    } else {
        static C4DVector lowEndConstant(0.8f, 0.8f, 0.0f, 0.0f);
        static CRGBFloatColour lowEndColour(0.0f, 0.4f, 0.8f, 1.0f);
        CRGBFloatColour colour = lowEndColour * g_pLightingManager->ThemeChannels[0].Colour;
        g_pShaderRenderManager->SetPixelShaderConstant(5, lowEndConstant);
        g_pShaderRenderManager->SetPixelShaderConstant(6, C4DVector(colour));
    }
    g_pShaderRenderManager->PrepareForRender(0x604);
}