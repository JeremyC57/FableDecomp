#ifndef FABLE_ENGINE_CENGINEPRIMITIVE2DSPRITE_H
#define FABLE_ENGINE_CENGINEPRIMITIVE2DSPRITE_H
// New shared header (chunk 779, row 0x00542A30 NUISystem::CIconText::Draw): Ego_r.pdb CEnginePrimitive2DSprite, sizeof 0x54
// (ghidra_out/struct_layouts_egor.tsv; the retail frame of Draw homes one at [esp+0B4h] up to the saved registers at 0x108):
// CEnginePrimitive base +0 (Type 0x22, InterpolationSet, ThemeChannelDesc), ClipBox (C2DBoxF) +0xC, Layer +0x1C, BankIndex +0x20,
// Frame +0x24, StartAngle +0x28, EndAngle +0x2C, BlendMode +0x30, Colour (CRGBColour) +0x34, ClipBoxIsPos +0x38, Flags +0x3C,
// Texture +0x40, UVOffsetsStart +0x44, UVOffsetsEnd +0x4C. The 16-parameter constructor is the landed 0x0041BEB0
// (FableWin.pdb ??0CEnginePrimitive2DSprite@@QAE@_KABVC2DBoxF@@JKKJJMMEEEEKABVC2DVector@@2@Z, `ret 44h`): its stores place the
// parameters as (unused u64, box, layer, bank index, frame, blend mode -> +0x30, interpolation set -> +0x4, start angle, end angle,
// a -> +0x37, r -> +0x36, g -> +0x35, b -> +0x34, flags -> +0x3C, uv start, uv end); ClipBoxIsPos and Texture are zeroed.
#include "rebuild_abi.h"
#include "engine/CEnginePrimitive.h"
#include "engine/C2DBoxF.h"
#include "engine/C2DVector.h"
#include "engine/CRGBColour.h"
class CTexture;
class CEnginePrimitive2DSprite : public CEnginePrimitive {
public:
    CEnginePrimitive2DSprite(unsigned __int64, const C2DBoxF&, long, unsigned long, unsigned long, long, long, float, float,
                             unsigned char, unsigned char, unsigned char, unsigned char, unsigned long,
                             const C2DVector&, const C2DVector&);                        // 0x0041BEB0
    C2DBoxF ClipBox;                                                                      // +0x0c
    long Layer;                                                                           // +0x1c
    unsigned long BankIndex;                                                              // +0x20
    unsigned long Frame;                                                                  // +0x24
    float StartAngle;                                                                     // +0x28
    float EndAngle;                                                                       // +0x2c
    long BlendMode;                                                                       // +0x30
    CRGBColour Colour;                                                                    // +0x34
    bool ClipBoxIsPos;                                                                    // +0x38
    unsigned long Flags;                                                                  // +0x3c
    CTexture* Texture;                                                                    // +0x40
    C2DVector UVOffsetsStart;                                                             // +0x44
    C2DVector UVOffsetsEnd;                                                               // +0x4c
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitive2DSprite) == 0x54);
#endif
