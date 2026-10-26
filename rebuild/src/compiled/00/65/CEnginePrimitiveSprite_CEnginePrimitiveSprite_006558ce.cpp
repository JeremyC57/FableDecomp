// CEnginePrimitiveSprite::CEnginePrimitiveSprite(const C3DVector&, unsigned long, unsigned long, unsigned short, float, float, float, EEnginePrimitiveBlendMode, unsigned char, unsigned char, unsigned char, unsigned char, float, EEngineInterpolationSet, EEnginePrimitiveBlendOp) -- primitive type 7: world position, bank/frame, flags, angle, size, colour channels, fade start, blend state and a zero offset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006558CE, length 142 bytes. No callees.
// Signature from Ego_r.pdb: ??0CEnginePrimitiveSprite@@QAE@ABVC3DVector@@KKGMMMW4EEnginePrimitiveBlendMode@@EEEEMW4EEngineInterpolationSet@@W4EEnginePrimitiveBlendOp@@@Z
// (15 stack words, `ret 3Ch`). Layout = PDB CEnginePrimitiveSprite (0x48): CEnginePrimitive base {Type +0,
// InterpolationSet +4, ThemeChannelDesc +8}, WorldPos +0xc, BankIndex +0x18, Frame +0x1c, SizeX +0x20,
// SizeY +0x24, FadeStart +0x28, Angle +0x2c, Colour +0x30, Flags +0x34, BlendMode +0x38, BlendOp +0x3c, Offset +0x40.
// The three floats after the flags feed Angle, SizeX, SizeY; the four channel bytes are alpha, red, green, blue
// and go through the CRGBColour(r, g, b, a) constructor, which stores R (+0x32), G (+0x31), B (+0x30), A (+0x33)
// in the D3DCOLOR byte layout. Offset is a C2DVector(0, 0) (two fldz/fstp; VC hoists the first fldz to the top).
// Shape: a frame-pointer body with no locals, the byte zero register (`xor cl,cl`) and the two Offset zeroes
// stored through the FPU (`fldz ; fstp` is 5 bytes against a 7-byte immediate store): a size-optimised translation
// unit with frame-pointer omission off -- `#pragma optimize("y", off)` + `#pragma optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)

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

struct CEnginePrimitiveSpriteColour {                    // CRGBColour with its four-channel constructor
    unsigned char B, G, R, A;
    CEnginePrimitiveSpriteColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct C2DVectorValue {
    float X, Y;
    C2DVectorValue(float x, float y) : X(x), Y(y) { }
};

struct CEnginePrimitiveSprite : CEnginePrimitive {
    C3DVector                    WorldPos;               // +0x0c
    unsigned long                BankIndex;              // +0x18
    unsigned long                Frame;                  // +0x1c
    float                        SizeX;                  // +0x20
    float                        SizeY;                  // +0x24
    float                        FadeStart;              // +0x28
    float                        Angle;                  // +0x2c
    CEnginePrimitiveSpriteColour Colour;                 // +0x30
    unsigned short               Flags;                  // +0x34
    long                         BlendMode;              // +0x38 EEnginePrimitiveBlendMode
    long                         BlendOp;                // +0x3c EEnginePrimitiveBlendOp
    C2DVectorValue               Offset;                 // +0x40   sizeof == 0x48
    CEnginePrimitiveSprite(const C3DVector& worldPos, unsigned long bankIndex, unsigned long frame,
                           unsigned short flags, float angle, float sizeX, float sizeY, long blendMode,
                           unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue,
                           float fadeStart, long interpolationSet, long blendOp);
};

CEnginePrimitiveSprite::CEnginePrimitiveSprite(const C3DVector& worldPos, unsigned long bankIndex, unsigned long frame,
                                               unsigned short flags, float angle, float sizeX, float sizeY,
                                               long blendMode, unsigned char alpha, unsigned char red,
                                               unsigned char green, unsigned char blue, float fadeStart,
                                               long interpolationSet, long blendOp)
    : CEnginePrimitive(7, interpolationSet)              // ENGINE_PRIMITIVE_SPRITE
    , WorldPos(worldPos)
    , BankIndex(bankIndex)
    , Frame(frame)
    , SizeX(sizeX)
    , SizeY(sizeY)
    , FadeStart(fadeStart)
    , Angle(angle)
    , Colour(red, green, blue, alpha)
    , Flags(flags)
    , BlendMode(blendMode)
    , BlendOp(blendOp)
    , Offset(0.0f, 0.0f)
{
}