// CEnginePrimitiveParticleLine::CEnginePrimitiveParticleLine(unsigned __int64, const C3DVector&, const C3DVector&, const CRGBColour&, float, EEngineInterpolationSet) -- primitive type 0x15: start, end, colour and width
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AFE220, length 115 bytes (+ int3 padding). No callees.
// Label correction: the catalog calls this CEnginePrimitiveShadowedSpotLight (propagated); the body is the
// CEnginePrimitiveParticleLine constructor: Ego_r.pdb ??0CEnginePrimitiveParticleLine@@QAE@_KABVC3DVector@@1ABVCRGBColour@@MW4EEngineInterpolationSet@@@Z
// has exactly these 7 stack words (`ret 1Ch`) and the PDB layout (0x2c) matches every store: Start +0xc,
// End +0x18, Colour +0x24, Width +0x28 (the ShadowedSpotLight is 0x38 bytes with three floats after the colour).
// The 64-bit primitive handle is not stored. Shape: base inline constructor (interpolation set argument, type
// immediate, zero channel bytes), vectors copied memberwise, the colour through the inline Set(r, g, b, a) copy.
// Leaf: ??0CEnginePrimitiveParticleLine

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

struct CEnginePrimitiveParticleLine : CEnginePrimitive {
    C3DVector  Start;                                    // +0x0c
    C3DVector  End;                                      // +0x18
    CRGBColour Colour;                                   // +0x24
    float      Width;                                    // +0x28   sizeof == 0x2c
    CEnginePrimitiveParticleLine(unsigned __int64 handle, const C3DVector& start, const C3DVector& end,
                                 const CRGBColour& colour, float width, long interpolationSet);
};

CEnginePrimitiveParticleLine::CEnginePrimitiveParticleLine(unsigned __int64 handle, const C3DVector& start,
                                                           const C3DVector& end, const CRGBColour& colour,
                                                           float width, long interpolationSet)
    : CEnginePrimitive(0x15, interpolationSet)           // ENGINE_PRIMITIVE_PARTICLE_LINE
    , Start(start)
    , End(end)
    , Colour(colour)
    , Width(width)
{
}
