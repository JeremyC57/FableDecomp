// CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64 id, const C2DVector& position, EEngine2DLayer layer, unsigned long bank_index, unsigned long frame, EEnginePrimitiveBlendMode blend_mode, EEngineInterpolationSet interpolation_set, float start_angle, float end_angle, unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue, unsigned long flags, const C2DVector& uv_offsets_start, const C2DVector& uv_offsets_end) -- fills a 2D sprite primitive (type 0x22) positioned at one point
// Leaf: ??0CEnginePrimitive2DSprite@@QAE@_KABVC2DVector@@W4EEngine2DLayer@@KKW4EEnginePrimitiveBlendMode@@W4EEngineInterpolationSet@@MMEEEEK11@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00493AC2, 153 bytes, `ret 44h`; Ego_r.pdb / FableWin.pdb
//   ??0CEnginePrimitive2DSprite@@QAE@_KABVC2DVector@@W4EEngine2DLayer@@KKW4EEnginePrimitiveBlendMode@@W4EEngineInterpolationSet@@MMEEEEK11@Z
//   (the position overload; the id is never read).
// Layout: Ego_r.pdb CEnginePrimitive2DSprite (0x54): CEnginePrimitive base +0 (engine/CEnginePrimitive.h), ClipBox +0xC,
//   Layer +0x1C, BankIndex +0x20, Frame +0x24, StartAngle +0x28, EndAngle +0x2C, BlendMode +0x30, Colour +0x34,
//   ClipBoxIsPos +0x38, Flags +0x3C, Texture +0x40, UVOffsetsStart +0x44, UVOffsetsEnd +0x4C.
// Callees: C2DBoxF::C2DBoxF(const C2DVector&, const C2DVector&) 0x004371A0 (PDB spelling), with the position for both corners.
// Shape: size TU with frame pointers (`push dword ptr [ebp+N]`, unaligned entry: `optimize("s")` + `("y", off)`, as the
//   landed clock-sprite ctor 0x0065595C). Every member from ClipBoxIsPos on is in the init list (declaration order): VC7.1
//   then stores Texture's zero before loading flags and sinks the ClipBoxIsPos byte below the first UV pointer load, as
//   retail; body assignments load flags above the `and` (the old draft's residue).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_C2DBOXF
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitive.h"
enum EEngine2DLayer;
enum EEnginePrimitiveBlendMode;
class CTexture;

class CEnginePrimitive2DSprite : public CEnginePrimitive {
public:
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DVector& position, EEngine2DLayer layer, unsigned long bank_index,
                             unsigned long frame, EEnginePrimitiveBlendMode blend_mode,
                             EEngineInterpolationSet interpolation_set, float start_angle, float end_angle,
                             unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue,
                             unsigned long flags, const C2DVector& uv_offsets_start, const C2DVector& uv_offsets_end);
    C2DBoxF                   ClipBox;             // +0x0C
    EEngine2DLayer            Layer;               // +0x1C
    unsigned long             BankIndex;           // +0x20
    unsigned long             Frame;               // +0x24
    float                     StartAngle;          // +0x28
    float                     EndAngle;            // +0x2C
    EEnginePrimitiveBlendMode BlendMode;           // +0x30
    CRGBColour                Colour;              // +0x34
    bool                      ClipBoxIsPos;        // +0x38
    unsigned long             Flags;               // +0x3C
    CTexture*                 Texture;             // +0x40
    C2DVector                 UVOffsetsStart;      // +0x44
    C2DVector                 UVOffsetsEnd;        // +0x4C
};

CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64 id, const C2DVector& position, EEngine2DLayer layer,
                                                   unsigned long bank_index, unsigned long frame,
                                                   EEnginePrimitiveBlendMode blend_mode,
                                                   EEngineInterpolationSet interpolation_set, float start_angle,
                                                   float end_angle, unsigned char alpha, unsigned char red,
                                                   unsigned char green, unsigned char blue, unsigned long flags,
                                                   const C2DVector& uv_offsets_start, const C2DVector& uv_offsets_end)
    : CEnginePrimitive((EEnginePrimitiveType)0x22, interpolation_set), ClipBox(position, position), Layer(layer),
      BankIndex(bank_index), Frame(frame), StartAngle(start_angle), EndAngle(end_angle), BlendMode(blend_mode),
      Colour(red, green, blue, alpha), ClipBoxIsPos(true), Flags(flags), Texture(0), UVOffsetsStart(uv_offsets_start),
      UVOffsetsEnd(uv_offsets_end)
{
}