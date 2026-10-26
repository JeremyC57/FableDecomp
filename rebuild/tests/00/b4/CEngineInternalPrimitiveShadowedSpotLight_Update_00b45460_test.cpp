// Behaviour test for retail 0x00b45460: the candidate source below is embedded verbatim (from src/00b45460.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CEngineInternalPrimitiveShadowedSpotLight::Update(const CEnginePrimitive&, unsigned long) -- copy the public shadowed
//   spot light's position, direction, radii and byte colour (scaled by 1/255 into float RGBA) into the internal primitive
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B45460, 176 bytes (manifest signature: const CEnginePrimitive&, ulong).
// Layout: PDB (Ego_r) names. The argument is the public CEnginePrimitiveShadowedSpotLight (base CEnginePrimitive 0x0c;
//   WorldPos +0x0c, Direction +0x18, CRGBColour Colour +0x24 (bytes Blue, Green, Red, Alpha), Radius +0x28, SpotRadius
//   +0x2c, ConeRadius +0x30). The internal primitive keeps the ulong argument at +0x0c (descriptive name) and WorldPos
//   +0x48, Direction +0x54, CRGBFloatColour Colour +0x60, Radius +0x70, SpotRadius +0x74, ConeRadius +0x78 (retail offsets
//   equal the PDB). The 1/255 factor is the shared float constant at 0x01231724.
//   Local model: no engine/ header exists for these primitive classes.
class C3DVector { public: float X, Y, Z; };
class CRGBColour { public: unsigned char Blue, Green, Red, Alpha; };
class CRGBFloatColour { public:
    float R, G, B, A;
    void Set(float r, float g, float b, float a) { R = r; G = g; B = b; A = a; }
};
class CEnginePrimitive { public: long Type; long InterpolationSet; unsigned long ThemeChannelDesc; };
struct CEnginePrimitiveShadowedSpotLight : CEnginePrimitive {
    C3DVector WorldPos;                                 // +0x0c
    C3DVector Direction;                                // +0x18
    CRGBColour Colour;                                  // +0x24
    float Radius;                                       // +0x28
    float SpotRadius;                                   // +0x2c
    float ConeRadius;                                   // +0x30
    unsigned long LightingChannelMask;                  // +0x34
};
extern const float g_OneOver255;                        // 0x01231724 = 1/255
class CEngineInternalPrimitiveShadowedSpotLight { public:
    unsigned char _base00[0x0c];
    unsigned long UpdateParam;                          // +0x0c (descriptive)
    unsigned char _base10[0x38];
    C3DVector WorldPos;                                 // +0x48
    C3DVector Direction;                                // +0x54
    CRGBFloatColour Colour;                             // +0x60
    float Radius;                                       // +0x70
    float SpotRadius;                                   // +0x74
    float ConeRadius;                                   // +0x78
    void Update(const CEnginePrimitive& primitive, unsigned long param);
};
void CEngineInternalPrimitiveShadowedSpotLight::Update(const CEnginePrimitive& primitive, unsigned long param)
{
    const CEnginePrimitiveShadowedSpotLight& light = static_cast<const CEnginePrimitiveShadowedSpotLight&>(primitive);
    UpdateParam = param;
    WorldPos = light.WorldPos;
    Radius = light.Radius;
    Colour.Set(light.Colour.Red * g_OneOver255, light.Colour.Green * g_OneOver255, light.Colour.Blue * g_OneOver255, light.Colour.Alpha * g_OneOver255);
    SpotRadius = light.SpotRadius;
    Direction = light.Direction;
    ConeRadius = light.ConeRadius;
}

// ===== harness =====
#include <stdio.h>
#include <string.h>
#include <stddef.h>
extern const float g_OneOver255 = 1.0f / 255.0f;
int main() {
    CEnginePrimitiveShadowedSpotLight s; memset(&s, 0, sizeof s);
    s.WorldPos.X = 1; s.WorldPos.Y = 2; s.WorldPos.Z = 3; s.Direction.X = 0; s.Direction.Y = -1; s.Direction.Z = 0.5f;
    s.Colour.Blue = 255; s.Colour.Green = 51; s.Colour.Red = 0; s.Colour.Alpha = 102; s.Radius = 10; s.SpotRadius = 4; s.ConeRadius = 0.75f;
    CEngineInternalPrimitiveShadowedSpotLight d; memset(&d, 0xcd, sizeof d);
    d.Update(s, 0x1234);
    int ok = offsetof(CEnginePrimitiveShadowedSpotLight, Colour) == 0x24 && offsetof(CEngineInternalPrimitiveShadowedSpotLight, Colour) == 0x60
          && offsetof(CEngineInternalPrimitiveShadowedSpotLight, ConeRadius) == 0x78;
    ok = ok && d.UpdateParam == 0x1234 && d.WorldPos.X == 1 && d.WorldPos.Z == 3 && d.Direction.Y == -1 && d.Direction.Z == 0.5f;
    ok = ok && d.Radius == 10 && d.SpotRadius == 4 && d.ConeRadius == 0.75f;
    float eg = 51 * g_OneOver255, eb = 255 * g_OneOver255, ea = 102 * g_OneOver255;   /* /Od stores (rounds) these locals like the candidate stores */
    ok = ok && d.Colour.R == 0.0f && d.Colour.G == eg && d.Colour.B == eb && d.Colour.A == ea;
    ok = ok && d.Colour.B > 0.999f && d.Colour.B < 1.001f && d._base10[0] == 0xcd;
    printf(ok ? "00b45460_TEST PASS\n" : "00b45460_TEST FAIL\n"); return ok ? 0 : 1;
}