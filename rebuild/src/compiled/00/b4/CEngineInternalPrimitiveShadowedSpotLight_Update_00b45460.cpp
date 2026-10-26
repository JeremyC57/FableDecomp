// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test runs a simplified algorithm (divides by 255, never uses g_ByteToFloat); signature differs from PDB; comment still says DIFFER)
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