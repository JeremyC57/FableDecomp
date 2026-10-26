// CEnginePrimitiveParticleSprite::CEnginePrimitiveParticleSprite(unsigned __int64, unsigned long, const C3DVector&, const C2DVector&, float, const CRGBColour&, EEngineInterpolationSet) -- primitive type 0x16: frame, world position, size, angle and colour
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AFE2A0, length 116 bytes (+ int3 padding). No callees.
// Signature from Ego_r.pdb: ??0CEnginePrimitiveParticleSprite@@QAE@_KKABVC3DVector@@ABVC2DVector@@MABVCRGBColour@@W4EEngineInterpolationSet@@@Z
// (8 stack words, `ret 20h`); the leading 64-bit primitive handle is not stored.
// Layout = PDB CEnginePrimitiveParticleSprite (0x2c): CEnginePrimitive base {Type +0, InterpolationSet +4,
// ThemeChannelDesc +8}, Frame +0xc, WorldPos +0x10, Size +0x1c, Angle +0x24, Colour +0x28.
// Shape: base inline constructor (interpolation set argument, type immediate, three zero channel bytes), vectors
// copied memberwise through one register per dword, the colour through the inline Set(r, g, b, a) copy.

class C3DVector { public:                                       // memberwise user copy constructor: one register per dword
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
class C2DVector { public:
    float X, Y;
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};
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

struct CEnginePrimitiveParticleSprite : CEnginePrimitive {
    unsigned long Frame;                                 // +0x0c
    C3DVector     WorldPos;                              // +0x10
    C2DVector     Size;                                  // +0x1c
    float         Angle;                                 // +0x24
    CRGBColour    Colour;                                // +0x28   sizeof == 0x2c
    CEnginePrimitiveParticleSprite(unsigned __int64 handle, unsigned long frame, const C3DVector& worldPos,
                                   const C2DVector& size, float angle, const CRGBColour& colour,
                                   long interpolationSet);
};

CEnginePrimitiveParticleSprite::CEnginePrimitiveParticleSprite(unsigned __int64 handle, unsigned long frame,
                                                               const C3DVector& worldPos, const C2DVector& size,
                                                               float angle, const CRGBColour& colour,
                                                               long interpolationSet)
    : CEnginePrimitive(0x16, interpolationSet)           // ENGINE_PRIMITIVE_PARTICLE_SPRITE
    , Frame(frame)
    , WorldPos(worldPos)
    , Size(size)
    , Angle(angle)
    , Colour(colour)
{
}