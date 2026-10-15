#include <cstdio>
#include <cmath>
struct C3DVector { float X, Y, Z; };
struct CRGBColour { unsigned char B, G, R, A; };
struct CRGBFloatColour { float R, G, B, A; void Set(float r, float g, float b, float a) { R = r; G = g; B = b; A = a; } };
struct CLightDesc { C3DVector WorldPos; CRGBColour Colour; float Radius; float SpotRadius; unsigned int LightingChannelMask; };
struct CLight {
    unsigned int EnginePrimitiveFlags; C3DVector WorldPos; CRGBFloatColour Colour; float Radius; float SpotRadius; unsigned int ChannelMask;
    void Update(const CLightDesc& light, unsigned int flags)
    {
        EnginePrimitiveFlags = flags; WorldPos = light.WorldPos; Radius = light.Radius;
        Colour.Set(light.Colour.R * (1.0f / 255.0f), light.Colour.G * (1.0f / 255.0f), light.Colour.B * (1.0f / 255.0f), light.Colour.A * (1.0f / 255.0f));
        SpotRadius = light.SpotRadius; ChannelMask = light.LightingChannelMask;
    }
};
int main()
{
    CLightDesc d; d.WorldPos.X = 1.0f; d.WorldPos.Y = 2.0f; d.WorldPos.Z = 3.0f; d.Colour.B = 0; d.Colour.G = 51; d.Colour.R = 255; d.Colour.A = 102;
    d.Radius = 10.0f; d.SpotRadius = 4.0f; d.LightingChannelMask = 0x5;
    CLight l; l.Update(d, 9);
    if (l.EnginePrimitiveFlags != 9 || l.WorldPos.Y != 2.0f || std::fabs(l.Colour.R - 1.0f) > 1e-6f || std::fabs(l.Colour.G - 0.2f) > 1e-6f
        || l.Colour.B != 0.0f || std::fabs(l.Colour.A - 0.4f) > 1e-6f || l.Radius != 10.0f || l.SpotRadius != 4.0f || l.ChannelMask != 5) { std::printf("00b453d0_TEST FAIL\n"); return 1; }
    std::printf("00b453d0_TEST PASS\n");
    return 0;
}