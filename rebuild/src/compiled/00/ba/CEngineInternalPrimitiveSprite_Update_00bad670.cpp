// CEngineInternalPrimitiveSprite::Update(const CEnginePrimitive& primitive, unsigned long flags) -- copy a sprite descriptor (position, bank index, frame, half sizes, angle, blend mode/op, colour with its alpha) and its flag word into the render bitfields, start loading the sprite's graphic frame, pick the offset factors from the alignment flags (or from the descriptor offset over the frame's pixel size), and apply the game-primitive fade defaults with the descriptor's fade start
// Leaf: Update
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BAD670, `ret 8`, virtual `?Update@CEngineInternalPrimitiveSprite@@UAEXABVCEnginePrimitive@@K@Z`.
// Callees: CGraphicInfoFrame::BeginLoading(unsigned long, unsigned long) @ 0x009FE620,
// CEngineInternalPrimitiveBase::SetFadeDistanceWithGamePrimitiveDefaults(float) @ 0x00B8FD40.
// Globals: the engine @ 0x01436E98 (graphic bank at +0x954, 0x2c-byte record vector at +0x1e0, each record's 16-byte frame
//   array at +0x28) -- the bank model of the landed CEngineInternalPrimitiveLine::Update 0x00BC7F70.
// Layout: base = engine/CEngineInternalPrimitiveBase.h (EnginePrimitiveFlags +0x0c, Alpha +0x27); the sprite fields follow the
//   PDB CEngineInternalPrimitiveSprite (WorldPos +0x48 .. bitfield byte +0x84) / CEnginePrimitiveSprite (WorldPos +0x0c .. Offset +0x40).
// Shape: the six flag-word bits are plain `bool`-valued bitfield assignments after `IsParticle = true` (VC7.1 merges them into
//   one byte RMW as retail); the frame getter is the landed bank model with a frame index; every offset branch shares the tail call.
// NF46 (round 7): the 0x01436E98 stand-in CEngineGlobal_01436e98 / g_pEngine_01436e98 is NEngineInternal::GPResourceManager
//   (?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A) on engine/CEngineResourceManager.h; +0x954 is GetGraphicBank (_GETGRAPHICBANK,
//   as 0x00B2D9F0), and the local graphic-bank / record-vector / CGraphicInfoFrame models are the header CGraphicDataBank / CGraphicInfo /
//   CGraphicInfoFrame (GraphicList +0x1E0, Frames +0x28; FableWin.pdb inline PeekGraphic / PeekGraphicInfoFrame / PeekInfoFrame as landed
//   0x00BB0970, both getters forced inline; BeginLoading const from the _BEGINLOADING opt-in).
//   The frame size is FableWin.pdb CGraphicInfoFrame::GetWidth / GetHeight (inline, unsigned long; FABLE_RETAIL_CGRAPHICINFO_PASS22_SPRITE bodies).
//   FITTED: retail converts it signed (0x00BAD85F `movzx ; mov [esp+18h] ; fild dword`), so the quotient divides by `(float)(long)GetWidth()`;
//   a plain unsigned long conversion emits the 2^32-correction form (the landed row read the unsigned short header word directly).
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

#include "engine/C3DVector.h"
class C2DVector { public: float X, Y; };
class CRGBColour { public: unsigned char B, G, R, A; };

class CEnginePrimitive {                           // PDB, 12 bytes
public:
    long          Type;
    long          InterpolationSet;
    unsigned long ThemeChannelDesc;
};

class CEnginePrimitiveSprite : public CEnginePrimitive {   // PDB, 72 bytes
public:
    C3DVector      WorldPos;                       // +0x0c
    unsigned long  BankIndex;                      // +0x18
    unsigned long  Frame;                          // +0x1c
    float          SizeX;                          // +0x20
    float          SizeY;                          // +0x24
    float          FadeStart;                      // +0x28
    float          Angle;                          // +0x2c
    CRGBColour     Colour;                         // +0x30
    unsigned short Flags;                          // +0x34
    long           BlendMode;                      // +0x38
    long           BlendOp;                        // +0x3c
    C2DVector      Offset;                         // +0x40
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

enum {
    SPRITE_3D              = 0x001,
    SPRITE_OFFSET_LEFT     = 0x002,
    SPRITE_OFFSET_RIGHT    = 0x004,
    SPRITE_OFFSET_TOP      = 0x008,
    SPRITE_OFFSET_BOTTOM   = 0x010,
    SPRITE_ROTATE_CENTRE   = 0x020,
    SPRITE_ROTATE_Z_AXIS   = 0x040,
    SPRITE_LIT             = 0x080,
    SPRITE_MODULATE_2X     = 0x100,
    SPRITE_NO_ZBUFFER      = 0x200
};

class CEngineInternalPrimitiveSprite : public CEngineInternalPrimitiveBase { public:
    C3DVector     WorldPos;                        // +0x48
    unsigned long BankIndex;                       // +0x54
    void*         Texture;                         // +0x58
    unsigned long Frame;                           // +0x5c
    float         SizeX;                           // +0x60
    float         SizeY;                           // +0x64
    float         OffsetFactorX;                   // +0x68
    float         OffsetFactorY;                   // +0x6c
    float         BSphereRadius;                   // +0x70
    float         Angle;                           // +0x74
    long          BlendMode;                       // +0x78
    long          BlendOp;                         // +0x7c
    CRGBColour    Colour;                          // +0x80
    unsigned char Modulate2X : 1;                  // +0x84
    unsigned char Lit : 1;
    unsigned char Sprite3D : 1;
    unsigned char RotateAroundCentre : 1;
    unsigned char RotateAroundZAxis : 1;
    unsigned char IsParticle : 1;
    unsigned char ZBuffer : 1;
    void SetFadeDistanceWithGamePrimitiveDefaults(float distance);   // 0x00B8FD40
    void Update(const CEnginePrimitive& primitive, unsigned long flags);
};

void CEngineInternalPrimitiveSprite::Update(const CEnginePrimitive& primitive, unsigned long flags)
{
    const CEnginePrimitiveSprite& sprite = static_cast<const CEnginePrimitiveSprite&>(primitive);
    EnginePrimitiveFlags = flags;
    WorldPos = sprite.WorldPos;
    BankIndex = sprite.BankIndex;
    Frame = sprite.Frame;
    SizeX = sprite.SizeX * 0.5f;
    SizeY = sprite.SizeY * 0.5f;
    Angle = sprite.Angle;
    BlendMode = sprite.BlendMode;
    BlendOp = sprite.BlendOp;
    Colour = sprite.Colour;
    Alpha = Colour.A;
    IsParticle = true;
    unsigned short spriteFlags = sprite.Flags;
    Modulate2X = (spriteFlags & SPRITE_MODULATE_2X) != 0;
    Lit = (spriteFlags & SPRITE_LIT) != 0;
    Sprite3D = (spriteFlags & SPRITE_3D) != 0;
    RotateAroundCentre = (spriteFlags & SPRITE_ROTATE_CENTRE) != 0;
    RotateAroundZAxis = (spriteFlags & SPRITE_ROTATE_Z_AXIS) != 0;
    ZBuffer = (spriteFlags & SPRITE_NO_ZBUFFER) == 0;
    const CGraphicInfoFrame* frame = NEngineInternal::GPResourceManager->GetGraphicBank((EGraphicBankNo)0)->PeekGraphicInfoFrame(BankIndex, Frame);
    frame->BeginLoading(0, 0);
    if (spriteFlags & SPRITE_OFFSET_LEFT) {
        OffsetFactorX = 1.0f;
        OffsetFactorY = 0.0f;
    } else if (spriteFlags & SPRITE_OFFSET_RIGHT) {
        OffsetFactorX = -1.0f;
        OffsetFactorY = 0.0f;
    } else if (spriteFlags & SPRITE_OFFSET_TOP) {
        OffsetFactorX = 0.0f;
        OffsetFactorY = -1.0f;
    } else if (spriteFlags & SPRITE_OFFSET_BOTTOM) {
        OffsetFactorX = 0.0f;
        OffsetFactorY = 1.0f;
    } else {
        OffsetFactorX = sprite.Offset.X / (float)(long)frame->GetWidth() * 2.0f;
        OffsetFactorY = sprite.Offset.Y / (float)(long)frame->GetHeight() * 2.0f;
    }
    SetFadeDistanceWithGamePrimitiveDefaults(sprite.FadeStart);
}