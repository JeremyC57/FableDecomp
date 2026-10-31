// CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64, const C2DVector&, EEngine2DLayer, unsigned long, unsigned long, EEnginePrimitiveBlendMode, EEngineInterpolationSet, float, float, unsigned char, unsigned char, unsigned char, unsigned char, unsigned long, const C2DVector&, const C2DVector&) -- 2D sprite primitive (type 0x22)
// Grade: DIFFER(153v153) -- scheduling only: retail `and dword ptr [esi+40h],0 ; mov eax,[ebp+40h]` (Texture = 0 before the Flags load), ours loads [ebp+40h] first; the `lea ecx,[esi+0Ch]` residue is closed (ThemeChannelDesc member with an inline ctor).
// Tried (2026-10-31, chunk 628): real-name base CEnginePrimitive + CEnginePrimitiveThemeChannelDesc member ctor (closed the lea),
//   trailing members as body assignments, ClipBoxIsPos/Texture in the init list with Flags in the body -- all leave the Texture/Flags order.
// Names: Ego_r.pdb CEnginePrimitive2DSprite (ClipBox, Layer, BankIndex, Frame, StartAngle, EndAngle, BlendMode, Colour, ClipBoxIsPos,
//   Flags, Texture, UVOffsetsStart, UVOffsetsEnd), CEnginePrimitive (Type, InterpolationSet, ThemeChannelDesc); the ctor mangles as
//   the PDB ??0CEnginePrimitive2DSprite@@QAE@_KABVC2DVector@@W4EEngine2DLayer@@KKW4EEnginePrimitiveBlendMode@@W4EEngineInterpolationSet@@MMEEEEK11@Z.
// Callee: C2DBoxF::C2DBoxF(const C2DVector&, const C2DVector&) 0x004371A0 (descriptive ctor signature; the class name is the PDB member type).
// Leaf: ??0CEnginePrimitive2DSprite
#pragma optimize("y", off)
#pragma optimize("s", on)

class C2DVector { public:
    float X, Y;
};
class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF(const C2DVector& tl, const C2DVector& br);   // 0x004371A0
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CTexture;
enum EEngine2DLayer { ENGINE_2D_LAYER_0 = 0 };
enum EEnginePrimitiveBlendMode { ENGINE_PRIMITIVE_BLEND_0 = 0 };
enum EEngineInterpolationSet { ENGINE_INTERPOLATION_SET_0 = 0 };
enum EEnginePrimitiveType { ENGINE_PRIMITIVE_2D_SPRITE = 0x22 };

class CEnginePrimitiveThemeChannelDesc { public:
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
};
class CEnginePrimitive { public:
    EEnginePrimitiveType             Type;
    EEngineInterpolationSet          InterpolationSet;
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;
    CEnginePrimitive(EEnginePrimitiveType type, EEngineInterpolationSet interpolationSet) : Type(type), InterpolationSet(interpolationSet) { }
};

class CEnginePrimitive2DSprite : public CEnginePrimitive { public:
    C2DBoxF                   ClipBox;
    EEngine2DLayer            Layer;
    unsigned long             BankIndex;
    unsigned long             Frame;
    float                     StartAngle;
    float                     EndAngle;
    EEnginePrimitiveBlendMode BlendMode;
    CRGBColour                Colour;
    bool                      ClipBoxIsPos;
    unsigned long             Flags;
    CTexture*                 Texture;
    C2DVector                 UVOffsetsStart;
    C2DVector                 UVOffsetsEnd;
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DVector& pos, EEngine2DLayer layer, unsigned long bankIndex,
                             unsigned long frame, EEnginePrimitiveBlendMode blendMode, EEngineInterpolationSet interpolationSet,
                             float startAngle, float endAngle, unsigned char alpha, unsigned char red, unsigned char green,
                             unsigned char blue, unsigned long flags, const C2DVector& uvOffsetsStart, const C2DVector& uvOffsetsEnd);
};

CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64 id, const C2DVector& pos, EEngine2DLayer layer,
                                                   unsigned long bankIndex, unsigned long frame,
                                                   EEnginePrimitiveBlendMode blendMode,
                                                   EEngineInterpolationSet interpolationSet, float startAngle, float endAngle,
                                                   unsigned char alpha, unsigned char red, unsigned char green,
                                                   unsigned char blue, unsigned long flags, const C2DVector& uvOffsetsStart,
                                                   const C2DVector& uvOffsetsEnd)
    : CEnginePrimitive(ENGINE_PRIMITIVE_2D_SPRITE, interpolationSet), ClipBox(pos, pos), Layer(layer), BankIndex(bankIndex),
      Frame(frame), StartAngle(startAngle), EndAngle(endAngle), BlendMode(blendMode), Colour(red, green, blue, alpha),
      ClipBoxIsPos(true), Flags(flags), Texture(0), UVOffsetsStart(uvOffsetsStart), UVOffsetsEnd(uvOffsetsEnd)
{
}
