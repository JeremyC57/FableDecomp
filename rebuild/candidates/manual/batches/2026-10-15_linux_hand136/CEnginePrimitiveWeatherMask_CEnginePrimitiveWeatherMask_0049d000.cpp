// CEnginePrimitiveWeatherMask::CEnginePrimitiveWeatherMask(const C3DBoundingSphere&, const C3DBoundingBox&, const CMatrix3x4&, float, unsigned long, EEngineInterpolationSet) -- primitive type 0x1f: bounding sphere and box, inverse transform, scale and flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049D000, length 127 bytes. No callees.
// Signature from Ego_r.pdb: ??0CEnginePrimitiveWeatherMask@@QAE@ABVC3DBoundingSphere@@ABVC3DBoundingBox@@ABVCMatrix3x4@@MKW4EEngineInterpolationSet@@@Z
// (6 stack words, `ret 18h`). Layout = PDB CEnginePrimitiveWeatherMask (0x6c): CEnginePrimitive base
// {Type +0, InterpolationSet +4, ThemeChannelDesc +8}, BSphere +0xc, BBox +0x1c, InvTransform +0x34,
// Scale +0x64, Flags +0x68.
// Shape: base inline constructor (interpolation set argument, type immediate, zero channel bytes); the sphere and
// the box are copied memberwise (C3DVector members with a user copy constructor: one register per dword),
// while the 0x30-byte matrix is a plain POD of floats whose implicit copy is a `rep movsd` block of 12 dwords.

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

struct C3DBoundingSphere { C3DVector Center; float Radius; };
struct C3DBoundingBox { C3DVector Min; C3DVector Max; };
struct CMatrix3x4 { float m[12]; };                      // XAxis, YAxis, ZAxis, Position: a POD block

struct CEnginePrimitiveWeatherMask : CEnginePrimitive {
    C3DBoundingSphere BSphere;                           // +0x0c
    C3DBoundingBox    BBox;                              // +0x1c
    CMatrix3x4        InvTransform;                      // +0x34
    float             Scale;                             // +0x64
    unsigned long     Flags;                             // +0x68   sizeof == 0x6c
    CEnginePrimitiveWeatherMask(const C3DBoundingSphere& bSphere, const C3DBoundingBox& bBox,
                                const CMatrix3x4& invTransform, float scale, unsigned long flags,
                                long interpolationSet);
};

CEnginePrimitiveWeatherMask::CEnginePrimitiveWeatherMask(const C3DBoundingSphere& bSphere, const C3DBoundingBox& bBox,
                                                         const CMatrix3x4& invTransform, float scale,
                                                         unsigned long flags, long interpolationSet)
    : CEnginePrimitive(0x1f, interpolationSet)           // ENGINE_PRIMITIVE_WEATHER_MASK
    , BSphere(bSphere)
    , BBox(bBox)
    , InvTransform(invTransform)
    , Scale(scale)
    , Flags(flags)
{
}
