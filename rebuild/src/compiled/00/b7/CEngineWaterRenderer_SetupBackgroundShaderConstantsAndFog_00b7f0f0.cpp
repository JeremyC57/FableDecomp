// CEngineWaterRenderer::SetupBackgroundShaderConstantsAndFog() -- set up the background water shader state: the four background bone matrices, then (high-end water) user constant 3 from the bump factor or (low-end) pixel constants 5/6 from two static colours, the second modulated by the lighting theme colour; finally the inlined CShaderRenderManager::PrepareForRender(0x604)
// NF127 (2026-11-20): the row-local CShaderRenderManager declarations respelled in place to ?SetBoneTransform@CShaderRenderManager@@QAEXKABVCMatrix3x4@@@Z (0x0098A7F0; Ego_r.pdb 0x00AE5F80 + FableWin.pdb 0x03032D30: matrix by const reference).
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
#include "fable_compiler.h"
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

// Name fix (NF195, 2026-11-20, q4567): the local CArray<T> template model is replaced by the shared engine/CArray.h (CArray<T> is an
//   STLport vector<T>), so BoneMatrices.resize binds vector<CPreTransposedBoneMatrix>::resize 0x00B7EDF0 (declared only:
//   FABLE_RETAIL_STLP_VECTOR_RESIZE_VALUE_DECL_ONLY); Size() / GetData() / First are the vector's size(), CArray::GetAsCArray() and
//   CArray::operator[] / begin() / end().
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_VALUE_DECL_ONLY
#include "engine/CArray.h"

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
    void SetBoneTransform(unsigned long index, const CMatrix3x4& matrix);                        // 0x0098A7F0
    void SetPixelShaderConstant(unsigned long index, const C4DVector& value)
    {
        PSConstTable[index] = value;
        UpdateFlags |= 2;
    }

    FABLE_FORCEINLINE void PrepareForRender(unsigned long flags)
    {
        if (UpdateFlags & 4)
            SetVSConstants(CurrentVSConstantLayout->Reserved.Offset, CurrentVSConstantLayout->ReservedPresets.GetAsCArray(),
                           CurrentVSConstantLayout->ReservedPresets.size());
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
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }    // 0x01436E14

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
namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }   // 0x01436E9C ?GPLightingManager@NEngineInternal@@3PAVCEngineLightingManager@@A

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
    NEngineInternal::GPShaderRenderManager->BoneMatrices.resize(4, CPreTransposedBoneMatrix());
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(0, Settings.OtherBackgroundShaderVars);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(1, Settings.ShoreLookupMaskMatrix);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(2, Settings.ShoreLookupMaskMatrixForBounds);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(3, Settings.ShoreLookupMaskMatrix2);
    if (HighendWaterEnabled) {
        float factor = WaterBumpFactor * Settings.BumpMatFactor;
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, factor, 0.0f, factor * -0.5f, 1.0f);
    } else {
        static C4DVector lowEndConstant(0.8f, 0.8f, 0.0f, 0.0f);
        static CRGBFloatColour lowEndColour(0.0f, 0.4f, 0.8f, 1.0f);
        CRGBFloatColour colour = lowEndColour * NEngineInternal::GPLightingManager->ThemeChannels[0].Colour;
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, lowEndConstant);
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(6, C4DVector(colour));
    }
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0x604);
}