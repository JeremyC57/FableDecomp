// CEngineWaterRenderer::SetupSeaMidgroundShaderConstantsAndFog() -- set up the sea midground shader state: user constant 0 = the sea bump-map span; high-end: pixel constant 7 = sea colour with the theme reflection alpha, user constant 3 from the bump factor and a camera-centred world transform; low-end: pixel constants 5/6 from two static colours (the second modulated by the theme colour); then the inlined CShaderRenderManager::PrepareForRender(0x205)
// Leaf: SetupSeaMidgroundShaderConstantsAndFog
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B7C510, length 864 bytes (+ int3 padding), plain `ret`.
// Label correction: catalogued "CEngineWaterRenderer::RenderSeaMidground", but the PDB method takes a float (`ret 4`); this body
//   has no argument and no direct caller. Named after the PDB's SetupSeaMidgroundShaderConstantsAndFog, not proven.
// Callees: SetUserVertexShaderConstant @ 0x00989B00, SetWorldTransform @ 0x009881F0, the environment provider chain of
//   0x00B7FA20, and the PrepareForRender inline of the landed 0x00B7F600 (same TU). Globals: shader render manager
//   @ 0x01436E14, engine state @ 0x01436E8C, lighting manager @ 0x01436E9C, camera @ 0x01436EA0 (WorldPosition +0x54).
//   Function statics @ 0x01437874 / 0x01437864 (guard @ 0x01437884). Float pool: -0.5f @ 0x01238798.
// Layout: as 0x00B7F0F0/0x00B7FA20 (retail Settings +0x280, HighendWaterEnabled +0x274, WaterBumpFactor +0x5c0);
//   SeaBumpmapSpan +0x3c of the settings.
// Shape: the low-end branch is 0x00B7F0F0's (a CRGBFloatColour product local handed on through a C4DVector conversion).
//   The world matrix is built from four C3DVector temporaries; retail stores the translation and off-diagonal zeros in
//   descending address order and the three diagonal ones last, which VC7.1 follows only in statement order, so the inline
//   matrix ctor writes its members in that order. C3DVector needs a user operator= for the camera-position copy to read
//   through the camera pointer (`mov ecx,[eax+54h]`) instead of forming the member address.
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
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector& operator=(const C3DVector& o) { X = o.X; Y = o.Y; Z = o.Z; return *this; }
};
class CMatrix3x4 { public:
    float M[9];                                         // rotation rows
    C3DVector Pos;                                      // translation
    CMatrix3x4() { }
    CMatrix3x4(const C3DVector& xAxis, const C3DVector& yAxis, const C3DVector& zAxis, const C3DVector& pos)
    {
        Pos.Z = pos.Z; Pos.Y = pos.Y; Pos.X = pos.X;
        M[7] = zAxis.Y; M[6] = zAxis.X;
        M[5] = yAxis.Z; M[3] = yAxis.X;
        M[2] = xAxis.Z; M[1] = xAxis.Y;
        M[8] = zAxis.Z; M[4] = yAxis.Y; M[0] = xAxis.X;
    }
};
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
    void SetWorldTransform(const CMatrix3x4& matrix);                                           // 0x009881F0
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
    float           BumpmapSpan;                        // +0x38
    float           SeaBumpmapSpan;                     // +0x3c
    unsigned char   _pad40[0x5c - 0x40];
    float           BumpMatFactor;                      // +0x5c
    unsigned char   _pad60[0x84 - 0x60];
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

struct SThemeSettings {
    unsigned char _pad000[0x138];
    float         SeaReflectionAlpha;                   // +0x138 (name descriptive)
};
struct CEnvironmentTheme {
    unsigned char   _pad[0x54];
    SThemeSettings* m_pSettings;                        // +0x54
};
class CEnvironment { public:
    virtual void v00();
    virtual CEnvironmentTheme* GetTheme();              // slot 1
};
struct CEnvironmentProvider {
    virtual void v00();
    virtual CEnvironment* GetCurrentEnvironment();      // slot 1
};
struct SEngineState {
    unsigned char         _pad[0x198];
    CEnvironmentProvider* m_pEnvironmentProvider;       // +0x198
};
extern SEngineState* g_pEngineState;                    // 0x01436E8C

struct CThemeChannel { CRGBFloatColour Colour; };

class CEngineLightingManager { public:
    unsigned char  _pad00[0x3c];
    CThemeChannel* ThemeChannels;                       // +0x3c (CArray first pointer)
};
extern CEngineLightingManager* g_pLightingManager;      // 0x01436E9C

class CEngineCamera { public:
    unsigned char _pad00[0x54];
    C3DVector     WorldPosition;                        // +0x54
};
extern CEngineCamera* g_pEngineCamera;                  // 0x01436EA0

class CEngineWaterRenderer { public:
    unsigned char        _pad000[0x274];                // vptr, buffers, shaders, render lists (retail 12-byte CArrays)
    bool                 HighendWaterEnabled;           // +0x274
    unsigned char        _pad275[0xb];
    CEngineWaterSettings Settings;                      // +0x280
    unsigned char        _pad564[0x5c0 - 0x564];
    float                WaterBumpFactor;               // +0x5c0
    void SetupSeaMidgroundShaderConstantsAndFog();
};

void CEngineWaterRenderer::SetupSeaMidgroundShaderConstantsAndFog()
{
    g_pShaderRenderManager->SetUserVertexShaderConstant(0, Settings.SeaBumpmapSpan, 0.0f, 0.0f, 0.0f);
    if (HighendWaterEnabled) {
        C4DVector colour;
        colour.X = Settings.Colour.R;
        colour.Y = Settings.Colour.G;
        colour.Z = Settings.Colour.B;
        colour.W = g_pEngineState->m_pEnvironmentProvider->GetCurrentEnvironment()->GetTheme()->m_pSettings->SeaReflectionAlpha
                 * Settings.ColourToReflectionFactor;
        g_pShaderRenderManager->SetPixelShaderConstant(7, colour);
        float factor = WaterBumpFactor * Settings.BumpMatFactor;
        g_pShaderRenderManager->SetUserVertexShaderConstant(3, factor, 0.0f, factor * -0.5f, 1.0f);
        CMatrix3x4 world(C3DVector(1.0f, 0.0f, 0.0f), C3DVector(0.0f, 1.0f, 0.0f), C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 0.0f, 0.0f));
        world.Pos = g_pEngineCamera->WorldPosition;
        g_pShaderRenderManager->SetWorldTransform(world);
    } else {
        static C4DVector lowEndConstant(0.8f, 0.8f, 0.0f, 0.0f);
        static CRGBFloatColour lowEndColour(0.0f, 0.4f, 0.8f, 1.0f);
        CRGBFloatColour colour = lowEndColour * g_pLightingManager->ThemeChannels[0].Colour;
        g_pShaderRenderManager->SetPixelShaderConstant(5, lowEndConstant);
        g_pShaderRenderManager->SetPixelShaderConstant(6, C4DVector(colour));
    }
    g_pShaderRenderManager->PrepareForRender(0x205);
}