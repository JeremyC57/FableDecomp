// CEngineInternalPrimitive2DSprite::Update(const CEnginePrimitive& primitive, unsigned long flags) -- copy a 2D sprite descriptor (clip box, layer, bank/frame, angles, blend mode, colour, interpolation set, texture, filter bit, UV offsets), derive WrapTexture and the render layer mask from the flags, start loading the bank graphic and grow the clip box by the frame / texture size when the box is a position
// Leaf: Update
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BAD8A0, length 671 bytes (`ret 8`, virtual `?Update@CEngineInternalPrimitive2DSprite@@UAEXABVCEnginePrimitive@@K@Z`).
// Callees: CGraphicInfoFrame::BeginLoading(unsigned long, unsigned long) @ 0x009FE620, CTexture::GetDimensions(unsigned long)
// @ 0x009F9DB0 (C2DExtentsI by hidden pointer). Globals: the engine @ 0x01436E98 (graphic bank at +0x954, its 0x2c-byte
// CGraphicInfo vector at +0x1e0), the UV epsilon float @ 0x0129BA3C (0.0001f, squared once and spilled).
// Layout: base = engine/CEngineInternalPrimitiveBase.h (EnginePrimitiveFlags +0x0c, InterpolationSet +0x2c, RenderLayerMask
// +0x30); the sprite members are those of engine/CEngineInternalPrimitive2DSprite.h at the same (pack 1, odd UV) offsets,
// re-declared on a derived shell because the generated header flattens the base into `_base_0x04` (as the landed
// CEngineInternalPrimitiveLine::Update 0x00BC7F70 does). The descriptor is the PDB CEnginePrimitive2DSprite (84 bytes);
// CGraphicInfo (0x2c: Header with FrameWidth +6 / FrameHeight +8, Frames +0x28) and CGraphicInfoFrame (16 bytes,
// GraphicInfo +0x0c) follow the PDB. The method is attached through the shell, so it is graded non-virtual.
// Shape: the colour goes through a named CRGBColour local (a direct member copy shifts the eax/edx alternation of every
// later copy by one); WrapTexture is `start.IsNonZero() || end.IsNonZero()` (`lenSq > eps*eps`, `test ah,41h ; je`); the
// frame pointer is `Get(i) ? &Get(i)->Frames[frame] : 0` over the __forceinline bounds-checked bank getter; the centre
// case halves the ushort sizes into `unsigned long` locals (unsigned fild fix-up, both conversions kept on the x87 stack);
// the texture case needs `unsigned long` locals for both dimensions (else VC filds the first straight from the result)
// and an inline GetTexture() accessor (a plain member read loads the second call's `this` after its pushes).
// NF46 (round 7): the 0x01436E98 stand-in CEngineGlobal_01436e98 / g_pEngine_01436e98 is NEngineInternal::GPResourceManager
//   (?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A) on engine/CEngineResourceManager.h; +0x954 is GetGraphicBank (_GETGRAPHICBANK,
//   as 0x00B2D9F0), and the local graphic-bank / record-vector / CGraphicInfoFrame models are the header CGraphicDataBank / CGraphicInfo /
//   CGraphicInfoFrame (GraphicList +0x1E0, Frames +0x28; FableWin.pdb inline PeekGraphic / PeekGraphicInfoFrame / PeekInfoFrame as landed
//   0x00BB0970, both getters forced inline; BeginLoading const from the _BEGINLOADING opt-in).
//   The frame size is FableWin.pdb CGraphicInfoFrame::GetWidth / GetHeight (inline, unsigned long; FABLE_RETAIL_CGRAPHICINFO_PASS22_SPRITE bodies:
//   retail converts unsigned, `test ; jge ; fadd [0x0122DCB4]`), was the frame's GraphicInfo->Header word read on a local model.
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETGRAPHICBANK
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICINFOFRAME_BEGINLOADING
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#define FABLE_RETAIL_CGRAPHICINFO_PASS22_SPRITE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "fable_compiler.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfo.h"
#include "engine/CEngineInternalPrimitiveBase.h"

extern const float DELTA_NOTIONAL_ZERO;           // 0x0129BA3C (0.0001f)
class C2DVector { public:
    float X, Y;
    float GetLengthSquared() const { return X * X + Y * Y; }
    bool IsNonZero() const { return GetLengthSquared() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO; }
};
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
class CRGBColour { public: unsigned char B, G, R, A; };
#include "engine/C2DExtentsI.h"
class CEnginePrimitive {
public:
    long          Type;
    long          InterpolationSet;
    unsigned long ThemeChannelDesc;
};
class CEnginePrimitive2DSprite : public CEnginePrimitive {   // PDB, 84 bytes
public:
    C2DBoxF       ClipBox;          // +0x0c
    long          Layer;            // +0x1c
    unsigned long BankIndex;        // +0x20
    unsigned long Frame;            // +0x24
    float         StartAngle;       // +0x28
    float         EndAngle;         // +0x2c
    long          BlendMode;        // +0x30
    CRGBColour    Colour;           // +0x34
    bool          ClipBoxIsPos;     // +0x38
    unsigned long Flags;            // +0x3c
    CTexture*     Texture;          // +0x40
    C2DVector     UVOffsetsStart;   // +0x44
    C2DVector     UVOffsetsEnd;     // +0x4c
};
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98
FABLE_FORCEINLINE const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}
FABLE_FORCEINLINE const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long graphic, unsigned long frame) const
{
    return PeekGraphic(graphic) ? PeekGraphic(graphic)->PeekInfoFrame(frame) : 0;
}

#pragma pack(push, 1)
class CEngineInternalPrimitive2DSprite : public CEngineInternalPrimitiveBase { public:
    C2DBoxF       ClipBox;          // +0x48
    long          Layer;            // +0x58
    unsigned long BankIndex;        // +0x5c
    unsigned long Frame;            // +0x60
    float         StartAngle;       // +0x64
    float         EndAngle;         // +0x68
    CRGBColour    Colour;           // +0x6c
    CTexture*     Texture;          // +0x70
    bool          Filter;           // +0x74
    C2DVector     UVOffsetsStart;   // +0x75
    C2DVector     UVOffsetsEnd;     // +0x7d
    bool          WrapTexture;      // +0x85
    unsigned char _pad_0x86[2];
    long          BlendMode;        // +0x88
    CTexture* GetTexture() const { return Texture; }
    void Update(const CEnginePrimitive& primitive, unsigned long flags);
};
#pragma pack(pop)

void CEngineInternalPrimitive2DSprite::Update(const CEnginePrimitive& primitive, unsigned long flags)
{
    EnginePrimitiveFlags = flags;
    const CEnginePrimitive2DSprite& sprite = static_cast<const CEnginePrimitive2DSprite&>(primitive);
    ClipBox = sprite.ClipBox;
    Layer = sprite.Layer;
    BankIndex = sprite.BankIndex;
    Frame = sprite.Frame;
    StartAngle = sprite.StartAngle;
    EndAngle = sprite.EndAngle;
    BlendMode = sprite.BlendMode;
    CRGBColour colour = sprite.Colour;
    Colour = colour;
    *reinterpret_cast<long*>(InterpolationSet) = primitive.InterpolationSet;
    Texture = sprite.Texture;
    Filter = (sprite.Flags >> 1) & 1;
    UVOffsetsStart = sprite.UVOffsetsStart;
    UVOffsetsEnd = sprite.UVOffsetsEnd;
    WrapTexture = UVOffsetsStart.IsNonZero() || UVOffsetsEnd.IsNonZero();
    if (flags & 0x80)
        EnginePrimitiveFlags |= 0x80;
    RenderLayerMask = (flags & 0x400) ? 0x00200000 : 0x80000000;
    if (BankIndex) {
        const CGraphicInfoFrame* frame = NEngineInternal::GPResourceManager->GetGraphicBank((EGraphicBankNo)0)->PeekGraphicInfoFrame(BankIndex, Frame);
        frame->BeginLoading(0, 0);
        if (sprite.ClipBoxIsPos) {
            if (sprite.Flags & 1) {
                unsigned long halfWidth = frame->GetWidth() >> 1;
                unsigned long halfHeight = frame->GetHeight() >> 1;
                ClipBox.TLX -= halfWidth;
                ClipBox.TLY -= halfHeight;
                ClipBox.BRX += halfWidth;
                ClipBox.BRY += halfHeight;
            } else {
                ClipBox.BRX += frame->GetWidth();
                ClipBox.BRY += frame->GetHeight();
            }
        }
    } else if (sprite.Texture && sprite.ClipBoxIsPos) {
        unsigned long width = GetTexture()->GetDimensions(0).X;
        ClipBox.BRX += width;
        unsigned long height = GetTexture()->GetDimensions(0).Y;
        ClipBox.BRY += height;
    }
}