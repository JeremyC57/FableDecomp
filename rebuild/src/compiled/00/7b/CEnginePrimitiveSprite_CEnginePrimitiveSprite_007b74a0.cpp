// CEnginePrimitiveSprite::CEnginePrimitiveSprite(const C3DVector& worldPos, unsigned long bankIndex, unsigned long frame, unsigned short flags, float angle, float sizeX, float sizeY, const C2DVector& offset, EEnginePrimitiveBlendMode blendMode, unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue, float fadeStart, EEngineInterpolationSet interpolationSet, EEnginePrimitiveBlendOp blendOp) -- primitive type 7 with an explicit screen offset
// Leaf: ??0CEnginePrimitiveSprite@@QAE@ABVC3DVector@@KKGMMMABVC2DVector@@W4EEnginePrimitiveBlendMode@@EEEEMW4EEngineInterpolationSet@@W4EEnginePrimitiveBlendOp@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B74A0, 158 bytes, thiscall, 16 stack words (`ret 40h`), no callees.
// Identity (proven): Ego_r.pdb declares three CEnginePrimitiveSprite constructors; only the C2DVector-offset overload
//   takes 16 words, and the stores fit its PDB layout (WorldPos +0xc, BankIndex +0x18, Frame +0x1c, SizeX +0x20, SizeY
//   +0x24, FadeStart +0x28, Angle +0x2c, Colour +0x30, Flags +0x34, BlendMode +0x38, BlendOp +0x3c, Offset +0x40).
//   The 15-word overload is the landed 0x006558CE. Its only caller is the GUI tree-pane draw
//   code at 0x007B7540 (five call sites).
// Shape: the landed 0x006558CE initialiser list with the offset copied from the reference; channel bytes go through
//   CRGBColour(r, g, b, a) (B +0x30, G +0x31, R +0x32, A +0x33). Speed TU, frame pointer omitted.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
class C2DVector { public:
    float X, Y;
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};
class CRGBColour { public:                                      // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

enum EEnginePrimitiveBlendMode { ENGINE_PRIMITIVE_BLEND_MODE_DEFAULT = 0 };
enum EEnginePrimitiveBlendOp { ENGINE_PRIMITIVE_BLEND_OP_DEFAULT = 0 };
enum EEngineInterpolationSet { ENGINE_INTERPOLATION_SET_DEFAULT = 0 };
enum EEnginePrimitiveType { ENGINE_PRIMITIVE_SPRITE = 7 };

class CEnginePrimitiveThemeChannelDesc { public:                // 4 bytes (PDB)
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};
class CEnginePrimitive { public:                                // 12 bytes (PDB)
    EEnginePrimitiveType             Type;               // +0x00
    EEngineInterpolationSet          InterpolationSet;   // +0x04
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;   // +0x08
    CEnginePrimitive(EEnginePrimitiveType type, EEngineInterpolationSet interpolationSet)
        : Type(type), InterpolationSet(interpolationSet) { }
};

class CEnginePrimitiveSprite : public CEnginePrimitive { public:
    C3DVector                 WorldPos;                  // +0x0c
    unsigned long             BankIndex;                 // +0x18
    unsigned long             Frame;                     // +0x1c
    float                     SizeX;                     // +0x20
    float                     SizeY;                     // +0x24
    float                     FadeStart;                 // +0x28
    float                     Angle;                     // +0x2c
    CRGBColour                Colour;                    // +0x30
    unsigned short            Flags;                     // +0x34
    EEnginePrimitiveBlendMode BlendMode;                 // +0x38
    EEnginePrimitiveBlendOp   BlendOp;                   // +0x3c
    C2DVector                 Offset;                    // +0x40   sizeof == 0x48
    CEnginePrimitiveSprite(const C3DVector& worldPos, unsigned long bankIndex, unsigned long frame, unsigned short flags,
                           float angle, float sizeX, float sizeY, const C2DVector& offset,
                           EEnginePrimitiveBlendMode blendMode, unsigned char alpha, unsigned char red,
                           unsigned char green, unsigned char blue, float fadeStart,
                           EEngineInterpolationSet interpolationSet, EEnginePrimitiveBlendOp blendOp);
};

CEnginePrimitiveSprite::CEnginePrimitiveSprite(const C3DVector& worldPos, unsigned long bankIndex, unsigned long frame,
                                               unsigned short flags, float angle, float sizeX, float sizeY,
                                               const C2DVector& offset, EEnginePrimitiveBlendMode blendMode,
                                               unsigned char alpha, unsigned char red, unsigned char green,
                                               unsigned char blue, float fadeStart,
                                               EEngineInterpolationSet interpolationSet, EEnginePrimitiveBlendOp blendOp)
    : CEnginePrimitive(ENGINE_PRIMITIVE_SPRITE, interpolationSet)
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
    , Offset(offset)
{
}