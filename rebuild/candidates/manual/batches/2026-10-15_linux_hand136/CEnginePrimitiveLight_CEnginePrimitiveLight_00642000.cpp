// CEnginePrimitiveLight::CEnginePrimitiveLight(unsigned __int64, const C3DVector&, const CRGBColour&, float, float, EEngineInterpolationSet, unsigned long) -- primitive type 0x0f: world position, colour, radius, spot radius and lighting channel mask
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00642000, length 108 bytes. No callees.
// Label correction: the catalog calls this CEnginePrimitiveShadowedSpotLight (propagated); the body is the
// CEnginePrimitiveLight constructor: Ego_r.pdb ??0CEnginePrimitiveLight@@QAE@_KABVC3DVector@@ABVCRGBColour@@MMW4EEngineInterpolationSet@@K@Z
// has exactly these 8 stack words (`ret 20h`) and its layout (0x28) matches every store: WorldPos +0xc,
// Colour +0x18, Radius +0x1c, SpotRadius +0x20, LightingChannelMask +0x24 (the ShadowedSpotLight constructor
// takes a second vector and three floats, 10 words). The 64-bit primitive handle is not stored.
// Shape: base inline constructor (interpolation set argument, type immediate, zero channel bytes), the vector
// copied memberwise, the colour through the inline Set(r, g, b, a) copy (bytes loaded A, B, G, R with B spilled
// into the dead interpolation-set slot, stored R, G, B, A).
// Leaf: ??0CEnginePrimitiveLight

struct C3DVector {                                       // memberwise user copy constructor: one register per dword
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
struct C2DVector {
    float X, Y;
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};
struct CRGBColour {                                      // the union's channel view: D3DCOLOR byte order
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};

struct CEnginePrimitiveThemeChannelDesc {                // 4 bytes (PDB)
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};
struct CEnginePrimitive {                                // 12 bytes (PDB)
    long                             Type;               // +0x00 EEnginePrimitiveType
    long                             InterpolationSet;   // +0x04 EEngineInterpolationSet
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;   // +0x08
    CEnginePrimitive(long type, long interpolationSet = 0) : Type(type), InterpolationSet(interpolationSet) { }
};

struct CEnginePrimitiveLight : CEnginePrimitive {
    C3DVector     WorldPos;                              // +0x0c
    CRGBColour    Colour;                                // +0x18
    float         Radius;                                // +0x1c
    float         SpotRadius;                            // +0x20
    unsigned long LightingChannelMask;                   // +0x24   sizeof == 0x28
    CEnginePrimitiveLight(unsigned __int64 handle, const C3DVector& worldPos, const CRGBColour& colour, float radius,
                          float spotRadius, long interpolationSet, unsigned long lightingChannelMask);
};

CEnginePrimitiveLight::CEnginePrimitiveLight(unsigned __int64 handle, const C3DVector& worldPos,
                                             const CRGBColour& colour, float radius, float spotRadius,
                                             long interpolationSet, unsigned long lightingChannelMask)
    : CEnginePrimitive(0x0f, interpolationSet)           // ENGINE_PRIMITIVE_LIGHT
    , WorldPos(worldPos)
    , Colour(colour)
    , Radius(radius)
    , SpotRadius(spotRadius)
    , LightingChannelMask(lightingChannelMask)
{
}
