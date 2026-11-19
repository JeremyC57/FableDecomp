// CEnginePrimitive2DSprite::CEnginePrimitive2DSprite(unsigned __int64, const C2DBoxF&, EEngine2DLayer, unsigned long, unsigned long, EEnginePrimitiveBlendMode, EEngineInterpolationSet, float, float, unsigned char x4, unsigned long, const C2DVector&, const C2DVector&) -- the box overload of the 2D sprite primitive: type 0x22, clip box, layer, bank / frame, angles, blend, colour, flags, no texture, UV offsets
// Leaf: ??0CEnginePrimitive2DSprite@@QAE@_KABVC2DBoxF@@W4EEngine2DLayer@@KKW4EEnginePrimitiveBlendMode@@W4EEngineInterpolationSet@@MMEEEEKABVC2DVector@@5@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041BEB0, 0xAB bytes, thiscall `ret 3Ch`.
// NF104 (q4944, 2026-11-19): moved off its local pointer-parameter model (??0CEnginePrimitive2DSprite@@QAE@_KPBVC2DBoxF@@KKKKKMMEEEEKPBVC2DVector@@2@Z,
//   local C2DBoxF / C2DVector / CEnginePrimitiveThemeChannelDesc / CEnginePrimitive2DSprite models) onto the shared engine/CEnginePrimitive2DSprite.h
//   box ctor body (FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS + _BOX_CTOR_BODY, the header text that grades MATCH against this address
//   on its own; the defines of the landed caller 0x0048E78E). Name: Ego_r.pdb 0x0042D110 ??0CEnginePrimitive2DSprite@@QAE@_KABVC2DBoxF@@W4EEngine2DLayer@@KKW4EEnginePrimitiveBlendMode@@W4EEngineInterpolationSet@@MMEEEEKABVC2DVector@@5@Z
//   (16 arguments incl. the 64-bit id; FableWin.pdb 0x01A662C6 in the CSubtitleRenderer TU); CMorphingSprite::Draw 0x00554CF5 and 7 other landed
//   rows bind it.
#include "fable_compiler.h"
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_BOX_CTOR_BODY
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#include <new>
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitive2DSprite.h"

namespace {
// Fidelity: scaffolding, not retail: a placement construction through the header's inline box ctor with inline expansion switched off for this
// one function, so this object emits the body out of line as a pick-any COMDAT (retail's 0x0041BEB0). Internal to this TU; never called.
#pragma inline_depth(0)
void ReferenceBoxCtor(void* where, unsigned __int64 id, const C2DBoxF& box, EEngine2DLayer layer, unsigned long bank, unsigned long frame,
                      EEnginePrimitiveBlendMode blend, EEngineInterpolationSet interpolation, float start_angle, float end_angle,
                      unsigned char alpha, unsigned char r, unsigned char g, unsigned char b, unsigned long flags,
                      const C2DVector& uv_start, const C2DVector& uv_end)
{
    new (where) CEnginePrimitive2DSprite(id, box, layer, bank, frame, blend, interpolation, start_angle, end_angle, alpha, r, g, b, flags,
                                         uv_start, uv_end);
}
#pragma inline_depth()
}
