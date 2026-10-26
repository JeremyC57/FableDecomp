// Behaviour test for 0x00B7C510: the sea midground shader constants (high-end and low-end) and PrepareForRender flags.
#include <cstdio>
#include <cmath>
class C4DVector { public:
    float X, Y, Z, W;
    C4DVector() { }
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
};
class CRGBFloatColour { public:
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
};
inline CRGBFloatColour operator*(const CRGBFloatColour& a, const CRGBFloatColour& b) { return CRGBFloatColour(a.R * b.R, a.G * b.G, a.B * b.B, a.A * b.A); }
inline C4DVector ToVector(const CRGBFloatColour& c) { return C4DVector(c.R, c.G, c.B, c.A); }
class C3DVector { public: float X, Y, Z; };
class CMatrix3x4 { public: float M[9]; C3DVector Pos; };
inline float Clamp(float value, float lo, float hi) { if (value < lo) return lo; if (value > hi) return hi; return value; }
class CShaderRenderManager { public:
    C4DVector vs[8]; int vsSet[8];
    const CMatrix3x4* bones[4]; int resized;
    C4DVector ps[8]; unsigned long UpdateFlags; unsigned long lastPrepare;
    void Reset() { for (int i = 0; i < 8; ++i) { vsSet[i] = 0; ps[i] = C4DVector(-9, -9, -9, -9); } for (int i = 0; i < 4; ++i) bones[i] = 0; resized = 0; UpdateFlags = 0; lastPrepare = 0; worldSet = 0; }
    void SetUserVertexShaderConstant(unsigned long index, float x, float y, float z, float w) { vs[index] = C4DVector(x, y, z, w); vsSet[index]++; }
    void ResizeBones(unsigned int n) { resized = (int)n; }
    void SetBoneTransform(unsigned long index, const CMatrix3x4* m) { bones[index] = m; }
    void SetPixelShaderConstant(unsigned long index, const C4DVector& v) { ps[index] = v; UpdateFlags |= 2; }
    CMatrix3x4 world; int worldSet;
    void SetWorldTransform(const CMatrix3x4& m) { world = m; worldSet++; }
    void PrepareForRender(unsigned long flags) { lastPrepare = flags; }
};
static CShaderRenderManager g_mgr;
static CShaderRenderManager* g_pShaderRenderManager = &g_mgr;
class CEngineWaterSettings { public:
    bool EnableBumpMap, EnableSwell, EnableFresnel;
    float FresnelFactor, SwellFactor, ShoreSwellFactor;
    CRGBFloatColour Colour;
    float SeaBumpmapSpan, BumpMatFactor, SoftEdgeSpanFactor, ColourToReflectionFactor, RefOffset;
    CMatrix3x4 ShoreLookupMaskMatrix, ShoreLookupMaskMatrix2, ShoreLookupMaskMatrixForBounds, OtherShaderVars, OtherSeaShaderVars, OtherBackgroundShaderVars;
};
static int Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
static int VecIs(const C4DVector& v, float x, float y, float z, float w) { return Near(v.X, x) && Near(v.Y, y) && Near(v.Z, z) && Near(v.W, w); }
class CEngineWaterRenderer { public:
    bool HighendWaterEnabled;
    CEngineWaterSettings Settings;
    float WaterBumpFactor;
    CRGBFloatColour theme;
    float themeAlpha;
    C3DVector cameraPos;
    void SetupSeaMidgroundShaderConstantsAndFog()
    {
        g_pShaderRenderManager->SetUserVertexShaderConstant(0, Settings.SeaBumpmapSpan, 0.0f, 0.0f, 0.0f);
        if (HighendWaterEnabled) {
            C4DVector colour;
            colour.X = Settings.Colour.R;
            colour.Y = Settings.Colour.G;
            colour.Z = Settings.Colour.B;
            colour.W = themeAlpha * Settings.ColourToReflectionFactor;
            g_pShaderRenderManager->SetPixelShaderConstant(7, colour);
            float factor = WaterBumpFactor * Settings.BumpMatFactor;
            g_pShaderRenderManager->SetUserVertexShaderConstant(3, factor, 0.0f, factor * -0.5f, 1.0f);
            CMatrix3x4 world;
            for (int i = 0; i < 9; ++i) world.M[i] = (i % 4 == 0) ? 1.0f : 0.0f;
            world.Pos = cameraPos;
            g_pShaderRenderManager->SetWorldTransform(world);
        } else {
            static C4DVector lowEndConstant(0.8f, 0.8f, 0.0f, 0.0f);
            static CRGBFloatColour lowEndColour(0.0f, 0.4f, 0.8f, 1.0f);
            CRGBFloatColour colour = lowEndColour * theme;
            g_pShaderRenderManager->SetPixelShaderConstant(5, lowEndConstant);
            g_pShaderRenderManager->SetPixelShaderConstant(6, ToVector(colour));
        }
        g_pShaderRenderManager->PrepareForRender(0x205);
    }
};
int main()
{
    CEngineWaterRenderer r;
    r.Settings.SeaBumpmapSpan = 9.0f; r.Settings.BumpMatFactor = 6.0f; r.Settings.ColourToReflectionFactor = 0.5f;
    r.Settings.Colour = CRGBFloatColour(0.1f, 0.2f, 0.3f, 0.4f);
    r.WaterBumpFactor = 2.0f; r.themeAlpha = 0.6f; r.theme = CRGBFloatColour(0.5f, 0.5f, 0.5f, 0.5f);
    r.cameraPos.X = 10; r.cameraPos.Y = 20; r.cameraPos.Z = 30;
    r.HighendWaterEnabled = true; g_mgr.Reset();
    r.SetupSeaMidgroundShaderConstantsAndFog();
    int ok = VecIs(g_mgr.vs[0], 9, 0, 0, 0) && VecIs(g_mgr.ps[7], 0.1f, 0.2f, 0.3f, 0.3f) && VecIs(g_mgr.vs[3], 12, 0, -6, 1)
        && g_mgr.worldSet == 1 && g_mgr.world.M[0] == 1 && g_mgr.world.M[4] == 1 && g_mgr.world.M[8] == 1 && g_mgr.world.M[1] == 0
        && g_mgr.world.Pos.X == 10 && g_mgr.world.Pos.Z == 30 && g_mgr.lastPrepare == 0x205 && Near(g_mgr.ps[5].X, -9);
    r.HighendWaterEnabled = false; g_mgr.Reset();
    r.SetupSeaMidgroundShaderConstantsAndFog();
    ok = ok && g_mgr.worldSet == 0 && g_mgr.vsSet[3] == 0 && VecIs(g_mgr.ps[5], 0.8f, 0.8f, 0, 0) && VecIs(g_mgr.ps[6], 0, 0.2f, 0.4f, 0.5f);
    if (!ok) { std::printf("00b7c510_TEST FAIL\n"); return 1; }
    std::printf("00b7c510_TEST PASS\n");
    return 0;
}