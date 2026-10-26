// CEngineInternalPrimitiveLight::Update(const CEnginePrimitive& primitive, unsigned long flags) -- copies a light descriptor into the internal light: primitive flags, position, radius, the byte colour converted to a float colour (each channel / 255), spot radius and lighting channel mask
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B453D0, length 142 bytes (`ret 8`). Constant 1/255 (0.0039215689f) @ 0x01231724.
// Layout: descriptor = PDB CEnginePrimitiveLight (40 bytes: WorldPos +0x0C, CRGBColour +0x18 stored B,G,R,A, Radius +0x1C, SpotRadius
// +0x20, LightingChannelMask +0x24); target = PDB CEngineInternalPrimitiveLight (WorldPos +0x48, CRGBFloatColour +0x54, Radius +0x64,
// SpotRadius +0x68, ChannelMask +0x6C; engine/CEngineInternalPrimitiveLight.h flattens the vectors, so modelled locally).
// Shape: the colour goes through an inline four-argument setter -- the arguments evaluate right to left (A, B, G, R: `movzx` of +0x1B
// first), each `fild` homed in the dead argument slot, then four `fstp` R..A straight into the member (a constructed temporary
// assigned to the member adds a frame copy); the radius copy is written before the colour (it is scheduled there).
class C3DVector { public: float X, Y, Z; };

class CRGBColour { public:                                // 4 bytes, stored B, G, R, A
    unsigned char B, G, R, A;
};

class CRGBFloatColour { public:                           // PDB, 16 bytes
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
    void Set(float r, float g, float b, float a) { R = r; G = g; B = b; A = a; }
};

class CEnginePrimitive {                           // PDB, 12 bytes
public:
    long          Type;
    long          InterpolationSet;
    unsigned long ThemeChannelDesc;
};

class CEnginePrimitiveLight : public CEnginePrimitive {   // PDB, 40 bytes
public:
    C3DVector     WorldPos;                        // +0x0c
    CRGBColour    Colour;                          // +0x18
    float         Radius;                          // +0x1c
    float         SpotRadius;                      // +0x20
    unsigned long LightingChannelMask;             // +0x24
};

class CEngineInternalPrimitiveLight {              // PDB, 112 bytes (engine/CEngineInternalPrimitiveLight.h)
public:
    virtual void v000();
    unsigned char   _pad04[8];
    unsigned long   EnginePrimitiveFlags;          // +0x0c (CEngineInternalPrimitiveBase)
    unsigned char   _pad10[0x38];
    C3DVector       WorldPos;                      // +0x48
    CRGBFloatColour Colour;                        // +0x54
    float           Radius;                        // +0x64
    float           SpotRadius;                    // +0x68
    unsigned long   ChannelMask;                   // +0x6c
    virtual void Update(const CEnginePrimitive& primitive, unsigned long flags);
};

void CEngineInternalPrimitiveLight::Update(const CEnginePrimitive& primitive, unsigned long flags)
{
    const CEnginePrimitiveLight& light = static_cast<const CEnginePrimitiveLight&>(primitive);
    EnginePrimitiveFlags = flags;
    WorldPos = light.WorldPos;
    Radius = light.Radius;
    Colour.Set(light.Colour.R * (1.0f / 255.0f), light.Colour.G * (1.0f / 255.0f),
               light.Colour.B * (1.0f / 255.0f), light.Colour.A * (1.0f / 255.0f));
    SpotRadius = light.SpotRadius;
    ChannelMask = light.LightingChannelMask;
}