// Behaviour test for CShaderRenderManager::SetAmbientColour (0x00988680): colour stored with A = 1.0f, flags |= 4.
#include <cstdio>
struct CRGBFloatColour { float R, G, B, A; };
struct CShaderRenderManager {
    CRGBFloatColour AmbientColour;
    unsigned long UpdateFlags;
    void SetAmbientColour(const CRGBFloatColour& colour)
    {
        AmbientColour = colour;
        AmbientColour.A = 1.0f;
        UpdateFlags |= 4;
    }
};
int main()
{
    CRGBFloatColour c = { 0.25f, 0.5f, 0.75f, 0.0f };
    CShaderRenderManager m; m.UpdateFlags = 0x100;
    m.SetAmbientColour(c);
    if (m.AmbientColour.R != 0.25f || m.AmbientColour.G != 0.5f || m.AmbientColour.B != 0.75f || m.AmbientColour.A != 1.0f || c.A != 0.0f || m.UpdateFlags != 0x104) { std::printf("00988680_TEST FAIL\n"); return 1; }
    std::printf("00988680_TEST PASS\n");
    return 0;
}