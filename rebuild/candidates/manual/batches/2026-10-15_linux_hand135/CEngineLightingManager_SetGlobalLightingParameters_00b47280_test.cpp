// Behaviour test for CEngineLightingManager::SetGlobalLightingParameters @ 0x00B47280.
#include <cstdio>
#include <cmath>
struct C3DVector {
    float X, Y, Z;
    C3DVector& Normalise() { float l = std::sqrt(X * X + Y * Y + Z * Z); float r = 1.0f / l; X *= r; Y *= r; Z *= r; return *this; }
};
struct CRGBFloatColour { float R, G, B, A; };
struct CThemeChannel { CRGBFloatColour DiffuseColour, AmbientColour, BacklightColour, ReflectionColour, FogColour; };
struct CEngineLightingManager {
    C3DVector DiffuseVector; CThemeChannel* ThemeChannels; unsigned int StateUpdateFlags;
    void SetGlobalLightingParameters(const C3DVector& d, const CRGBFloatColour& ambient, const CRGBFloatColour& diffuse, const CRGBFloatColour& backlight)
    {
        StateUpdateFlags |= 3;
        ThemeChannels[0].AmbientColour = ambient;
        ThemeChannels[0].DiffuseColour = diffuse;
        ThemeChannels[0].BacklightColour = backlight;
        DiffuseVector = d;
        DiffuseVector.Normalise();
    }
};
static int Eq(const CRGBFloatColour& a, const CRGBFloatColour& b) { return a.R == b.R && a.G == b.G && a.B == b.B && a.A == b.A; }
int main()
{
    CThemeChannel ch[2]; CEngineLightingManager m; m.ThemeChannels = ch; m.StateUpdateFlags = 0x10;
    CRGBFloatColour fog = { 9, 9, 9, 9 }; ch[0].FogColour = fog; ch[1].AmbientColour = fog;
    C3DVector d = { 3, 0, 4 }; CRGBFloatColour a = { 1, 2, 3, 4 }, df = { 5, 6, 7, 8 }, b = { 0.5f, 0.25f, 0, 1 };
    m.SetGlobalLightingParameters(d, a, df, b);
    int ok = m.StateUpdateFlags == 0x13 && Eq(ch[0].AmbientColour, a) && Eq(ch[0].DiffuseColour, df) && Eq(ch[0].BacklightColour, b)
          && Eq(ch[0].FogColour, fog) && Eq(ch[1].AmbientColour, fog)
          && std::fabs(m.DiffuseVector.X - 0.6f) < 1e-6f && m.DiffuseVector.Y == 0 && std::fabs(m.DiffuseVector.Z - 0.8f) < 1e-6f
          && d.X == 3;
    if (!ok) { std::printf("00b47280_TEST FAIL\n"); return 1; }
    std::printf("00b47280_TEST PASS\n");
    return 0;
}
