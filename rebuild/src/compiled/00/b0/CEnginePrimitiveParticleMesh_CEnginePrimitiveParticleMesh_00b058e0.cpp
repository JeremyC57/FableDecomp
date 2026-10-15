// CEnginePrimitiveParticleMesh::CEnginePrimitiveParticleMesh(unsigned __int64, const C3DVector&, const CRightHandedSet&, const C3DVector&, const CRGBColour&, EEngineInterpolationSet) -- primitive type 0x17: world position, size, orientation set and colour
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B058E0, length 147 bytes (+ int3 padding). No callees.
// Signature from Ego_r.pdb: ??0CEnginePrimitiveParticleMesh@@QAE@_KABVC3DVector@@ABVCRightHandedSet@@1ABVCRGBColour@@W4EEngineInterpolationSet@@@Z
// (7 stack words, `ret 1Ch`); the leading 64-bit primitive handle is not stored by this constructor.
// Layout = PDB CEnginePrimitiveParticleMesh (0x40): CEnginePrimitive base {Type +0, InterpolationSet +4,
// ThemeChannelDesc +8}, WorldPos +0xc, Size +0x18, RightHandedSet +0x24, Colour +0x3c.
// Shape: the base's inline constructor stores the interpolation-set argument, the type immediate and the three
// zero channel bytes; vectors copy memberwise (user copy constructors, one register per dword); the colour's
// copy constructor goes through the inline Set(r, g, b, a) (landed shape 0x005BA48A): bytes loaded A, B, G, R
// (B spilled into the dead argument slot), stored R, G, B, A.

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

struct CRightHandedSet { C3DVector Up; C3DVector Forward; };

struct CEnginePrimitiveParticleMesh : CEnginePrimitive {
    C3DVector       WorldPos;                            // +0x0c
    C3DVector       Size;                                // +0x18
    CRightHandedSet RightHandedSet;                      // +0x24
    CRGBColour      Colour;                              // +0x3c   sizeof == 0x40
    CEnginePrimitiveParticleMesh(unsigned __int64 handle, const C3DVector& worldPos, const CRightHandedSet& rhSet,
                                 const C3DVector& size, const CRGBColour& colour, long interpolationSet);
};

CEnginePrimitiveParticleMesh::CEnginePrimitiveParticleMesh(unsigned __int64 handle, const C3DVector& worldPos,
                                                           const CRightHandedSet& rhSet, const C3DVector& size,
                                                           const CRGBColour& colour, long interpolationSet)
    : CEnginePrimitive(0x17, interpolationSet)           // ENGINE_PRIMITIVE_PARTICLE_MESH
    , WorldPos(worldPos)
    , Size(size)
    , RightHandedSet(rhSet)
    , Colour(colour)
{
}