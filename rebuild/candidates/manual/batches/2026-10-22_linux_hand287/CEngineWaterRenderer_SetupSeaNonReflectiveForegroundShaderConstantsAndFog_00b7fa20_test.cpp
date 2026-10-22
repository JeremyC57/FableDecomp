// Behaviour test for 0x00B7FA20: the shader constants, bone matrices and PrepareForRender flags it sets.
#include <cstdio>
#include <cmath>
struct C4DVector {
    float X, Y, Z, W;
    C4DVector() { }
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
};
struct CRGBFloatColour {
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
};
inline CRGBFloatColour operator*(const CRGBFloatColour& a, const CRGBFloatColour& b) { return CRGBFloatColour(a.R * b.R, a.G * b.G, a.B * b.B, a.A * b.A); }
inline C4DVector ToVector(const CRGBFloatColour& c) { return C4DVector(c.R, c.G, c.B, c.A); }
struct CMatrix3x4 { float M[12]; };
inline float Clamp(float value, float lo, float hi) { if (value < lo) return lo; if (value > hi) return hi; return value; }
struct CShaderRenderManager {
    C4DVector vs[8]; int vsSet[8];
    const CMatrix3x4* bones[4]; int resized;
    C4DVector ps[8]; unsigned long UpdateFlags; unsigned long lastPrepare;
    void Reset() { for (int i = 0; i < 8; ++i) { vsSet[i] = 0; ps[i] = C4DVector(-9, -9, -9, -9); } for (int i = 0; i < 4; ++i) bones[i] = 0; resized = 0; UpdateFlags = 0; lastPrepare = 0; }
    void SetUserVertexShaderConstant(unsigned long index, float x, float y, float z, float w) { vs[index] = C4DVector(x, y, z, w); vsSet[index]++; }
    void ResizeBones(unsigned int n) { resized = (int)n; }
    void SetBoneTransform(unsigned long index, const CMatrix3x4* m) { bones[index] = m; }
    void SetPixelShaderConstant(unsigned long index, const C4DVector& v) { ps[index] = v; UpdateFlags |= 2; }
    void PrepareForRender(unsigned long flags) { lastPrepare = flags; }
};
static CShaderRenderManager g_mgr;
static CShaderRenderManager* g_pShaderRenderManager = &g_mgr;
struct CEngineWaterSettings {
    bool EnableBumpMap, EnableSwell, EnableFresnel;
    float FresnelFactor, SwellFactor, ShoreSwellFactor;
    CRGBFloatColour Colour;
    float BumpMatFactor, SoftEdgeSpanFactor, ColourToReflectionFactor, RefOffset;
    CMatrix3x4 ShoreLookupMaskMatrix, ShoreLookupMaskMatrix2, ShoreLookupMaskMatrixForBounds, OtherShaderVars, OtherSeaShaderVars, OtherBackgroundShaderVars;
};
static int Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
static int VecIs(const C4DVector& v, float x, float y, float z, float w) { return Near(v.X, x) && Near(v.Y, y) && Near(v.Z, z) && Near(v.W, w); }
struct CEngineWaterRenderer {
    CEngineWaterSettings Settings;
    float WaterBumpFactor;
    float themeAlpha;
    void SetupSeaNonReflectiveForegroundShaderConstantsAndFog()
    {
    if (Settings.EnableSwell)
        g_pShaderRenderManager->SetUserVertexShaderConstant(0, Settings.SwellFactor, Settings.ShoreSwellFactor, Settings.RefOffset, 0.0f);
    else
        g_pShaderRenderManager->SetUserVertexShaderConstant(0, 0.0f, 0.0f, 0.0f, 0.0f);
    g_pShaderRenderManager->ResizeBones(4);
    g_pShaderRenderManager->SetBoneTransform(0, &Settings.OtherSeaShaderVars);
    g_pShaderRenderManager->SetBoneTransform(1, &Settings.ShoreLookupMaskMatrix);
    g_pShaderRenderManager->SetBoneTransform(2, &Settings.ShoreLookupMaskMatrixForBounds);
    g_pShaderRenderManager->SetBoneTransform(3, &Settings.ShoreLookupMaskMatrix2);
    float fresnel;
    if (Settings.EnableFresnel)
        fresnel = Clamp(Settings.FresnelFactor * 0.25f, 0.0f, 1.0f);
    else
        fresnel = 0.0f;
    g_pShaderRenderManager->SetPixelShaderConstant(5, C4DVector(fresnel, fresnel, fresnel, fresnel));
        float factor = WaterBumpFactor * Settings.BumpMatFactor;
        g_pShaderRenderManager->SetUserVertexShaderConstant(3, factor, 0.0f, factor * -0.5f, 1.0f);
        C4DVector colour;
        colour.X = Settings.Colour.R;
        colour.Y = Settings.Colour.G;
        colour.Z = Settings.Colour.B;
        colour.W = themeAlpha * Settings.ColourToReflectionFactor;
        g_pShaderRenderManager->SetPixelShaderConstant(7, colour);
        g_pShaderRenderManager->PrepareForRender(0x604);
    }
};

int main()
{
    CEngineWaterRenderer r;
    r.Settings.EnableBumpMap = true; r.Settings.EnableSwell = true; r.Settings.EnableFresnel = true;
    r.Settings.FresnelFactor = 2.0f; r.Settings.SwellFactor = 3.0f; r.Settings.ShoreSwellFactor = 4.0f; r.Settings.RefOffset = 5.0f;
    r.Settings.BumpMatFactor = 6.0f; r.Settings.SoftEdgeSpanFactor = 7.0f; r.Settings.ColourToReflectionFactor = 0.5f;
    r.Settings.Colour = CRGBFloatColour(0.1f, 0.2f, 0.3f, 0.4f);
    g_mgr.Reset();
    r.WaterBumpFactor = 2.0f; r.themeAlpha = 0.6f;
    r.SetupSeaNonReflectiveForegroundShaderConstantsAndFog();
    int ok = VecIs(g_mgr.vs[0], 3, 4, 5, 0) && VecIs(g_mgr.vs[3], 12, 0, -6, 1) && VecIs(g_mgr.ps[5], 0.5f, 0.5f, 0.5f, 0.5f)
        && VecIs(g_mgr.ps[7], 0.1f, 0.2f, 0.3f, 0.3f) && g_mgr.lastPrepare == 0x604;
    ok = ok && g_mgr.resized == 4 && g_mgr.bones[0] == &r.Settings.OtherSeaShaderVars && g_mgr.bones[1] == &r.Settings.ShoreLookupMaskMatrix
        && g_mgr.bones[2] == &r.Settings.ShoreLookupMaskMatrixForBounds && g_mgr.bones[3] == &r.Settings.ShoreLookupMaskMatrix2;
    if (!ok) { std::printf("00b7fa20_TEST FAIL\n"); return 1; }
    std::printf("00b7fa20_TEST PASS\n");
    return 0;
}
