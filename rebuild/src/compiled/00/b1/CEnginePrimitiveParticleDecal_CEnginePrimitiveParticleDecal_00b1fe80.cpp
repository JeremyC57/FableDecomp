// CEnginePrimitiveParticleDecal::CEnginePrimitiveParticleDecal(long, const C3DVector&, const C2DVector&, const CRGBColour&, const CRightHandedSet&) -- primitive type 0x19: world position, size, colour, decal frame and orientation set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B1FE80, length 144 bytes. No callees.
// Signature from Ego_r.pdb: ??0CEnginePrimitiveParticleDecal@@QAE@JABVC3DVector@@ABVC2DVector@@ABVCRGBColour@@ABVCRightHandedSet@@@Z
// (5 stack words, `ret 14h`). Layout = PDB CEnginePrimitiveParticleDecal (0x40): the CEnginePrimitive base
// {Type +0, InterpolationSet +4, ThemeChannelDesc +8}, WorldPos +0xc, Size +0x18, Colour +0x20, DecalFrame +0x24,
// RHSet +0x28 (Up, Forward).
// Shape: the base's inline constructor stores the type immediate, then the interpolation set and the three
// channel bytes from one zero register (this constructor has no interpolation-set parameter, so the base
// default 0 is used). The vectors are plain PODs copied dword by dword; the colour's copy constructor goes
// through the inline Set(r, g, b, a) (landed shape 0x005BA48A): the four channel bytes are loaded A, B, G, R
// (arguments right to left; B spilled into the dead argument slot) and stored R, G, B, A, which with the
// D3DCOLOR byte order (B, G, R, A) gives the retail +0x22, +0x23, +0x20, +0x21 permutation.

class C3DVector { public:                                       // memberwise user copy constructor: one register per dword
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
class C2DVector { public:
    float X, Y;
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };

class CRGBColour { public:                                      // the union's channel view: D3DCOLOR byte order
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};

class CEnginePrimitiveThemeChannelDesc { public:                // 4 bytes (PDB)
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};
class CEnginePrimitive { public:                                // 12 bytes (PDB)
    long                             Type;               // +0x00 EEnginePrimitiveType
    long                             InterpolationSet;   // +0x04 EEngineInterpolationSet
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;   // +0x08
    CEnginePrimitive(long type, long interpolationSet = 0) : Type(type), InterpolationSet(interpolationSet) { }
};

struct CEnginePrimitiveParticleDecal : CEnginePrimitive {
    C3DVector       WorldPos;                            // +0x0c
    C2DVector       Size;                                // +0x18
    CRGBColour      Colour;                              // +0x20
    long            DecalFrame;                          // +0x24
    CRightHandedSet RHSet;                               // +0x28   sizeof == 0x40
    CEnginePrimitiveParticleDecal(long decalFrame, const C3DVector& worldPos, const C2DVector& size,
                                  const CRGBColour& colour, const CRightHandedSet& rhSet);
};

CEnginePrimitiveParticleDecal::CEnginePrimitiveParticleDecal(long decalFrame, const C3DVector& worldPos,
                                                             const C2DVector& size, const CRGBColour& colour,
                                                             const CRightHandedSet& rhSet)
    : CEnginePrimitive(0x19)                             // ENGINE_PRIMITIVE_PARTICLE_DECAL
    , WorldPos(worldPos)
    , Size(size)
    , Colour(colour)
    , DecalFrame(decalFrame)
    , RHSet(rhSet)
{
}