// CEnginePrimitiveLine::CEnginePrimitiveLine(unsigned __int64, const C3DVector&, const C3DVector&, const CRGBColour&, float, unsigned long, EEnginePrimitiveBlendMode, EEngineInterpolationSet, EEnginePrimitiveBlendOp) -- primitive type 0x10: start, end, colour, width, bank index and blend state
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007324A0, length 134 bytes (+ int3 padding). No callees.
// Label correction: the catalog calls this CEnginePrimitiveParticleLine (propagated; that constructor is
// 0x00AFE220, type 0x15). This body is the retail CEnginePrimitiveLine constructor (type 0x10): 10 stack words
// (`ret 28h`) and a 0x38-byte object -- the Ego_r.pdb version (0x3c, 11 words) adds the trailing UseZBuffer
// bool and its parameter. Stores: Start +0xc, End +0x18, Colour +0x24, Width +0x28, BankIndex +0x2c,
// BlendMode +0x30, BlendOp +0x34 (PDB member names); the parameters feed them in that member order with the
// interpolation set (to the base) between BlendMode and BlendOp, as in the PDB signature.
// Shape: base inline constructor (interpolation set argument, type immediate, zero channel bytes), vectors
// copied memberwise, the colour through the inline Set(r, g, b, a) copy.
// Leaf: ??0CEnginePrimitiveLine

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

struct CEnginePrimitiveLine : CEnginePrimitive {
    C3DVector     Start;                                 // +0x0c
    C3DVector     End;                                   // +0x18
    CRGBColour    Colour;                                // +0x24
    float         Width;                                 // +0x28
    unsigned long BankIndex;                             // +0x2c
    long          BlendMode;                             // +0x30 EEnginePrimitiveBlendMode
    long          BlendOp;                               // +0x34 EEnginePrimitiveBlendOp   sizeof == 0x38 (retail)
    CEnginePrimitiveLine(unsigned __int64 handle, const C3DVector& start, const C3DVector& end,
                         const CRGBColour& colour, float width, unsigned long bankIndex, long blendMode,
                         long interpolationSet, long blendOp);
};

CEnginePrimitiveLine::CEnginePrimitiveLine(unsigned __int64 handle, const C3DVector& start, const C3DVector& end,
                                           const CRGBColour& colour, float width, unsigned long bankIndex,
                                           long blendMode, long interpolationSet, long blendOp)
    : CEnginePrimitive(0x10, interpolationSet)           // ENGINE_PRIMITIVE_LINE
    , Start(start)
    , End(end)
    , Colour(colour)
    , Width(width)
    , BankIndex(bankIndex)
    , BlendMode(blendMode)
    , BlendOp(blendOp)
{
}