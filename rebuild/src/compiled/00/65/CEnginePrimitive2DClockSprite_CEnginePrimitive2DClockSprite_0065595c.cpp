// 0065595C CEnginePrimitive2DClockSprite::CEnginePrimitive2DClockSprite
// Flags: /O1 /Oy- /Gy
// Fidelity: the private instantiation emits the shared constructor in its own object.
// Microsoft LINK discards the private call site when the constructor is linked.
// Leaf: ??0CEnginePrimitive2DClockSprite@@QAE@_KABVC2DVector@@W4EEngine2DLayer@@KKABVCRGBColour@@KK3W4EEnginePrimitiveBlendMode@@MM11KW4EEngineInterpolationSet@@@Z
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_C2DBOXF_CLOCK_CORNERS_OWNER_PASS30
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_INCLUDE_PASS23
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COPY_FORCEINLINE_PASS26
#include "engine/C2DVector.h"
#include "engine/CEnginePrimitive2DClockSprite.h"

namespace {
template<class Sprite>
#if defined(__clang__)
__attribute__((used, optnone))
#elif defined(__GNUC__)
__attribute__((used, optimize("O0")))
#endif
static void InstantiateClockConstructor(unsigned __int64 id,const C2DVector&pos,EEngine2DLayer layer,unsigned long bank,unsigned long frame,const CRGBColour&colour,
  unsigned long bank2,unsigned long frame2,const CRGBColour&colour2,EEnginePrimitiveBlendMode blend,float angle_start,float angle_end,
  const C2DVector&uv_start,const C2DVector&uv_end,unsigned long flags,EEngineInterpolationSet interpolation)
{
    Sprite clock(id, pos, layer, bank, frame, colour, bank2, frame2, colour2, blend, angle_start, angle_end, uv_start, uv_end, flags, interpolation);
}
template void InstantiateClockConstructor<CEnginePrimitive2DClockSprite>(unsigned __int64 id,const C2DVector&pos,EEngine2DLayer layer,unsigned long bank,unsigned long frame,const CRGBColour&colour,
  unsigned long bank2,unsigned long frame2,const CRGBColour&colour2,EEnginePrimitiveBlendMode blend,float angle_start,float angle_end,
  const C2DVector&uv_start,const C2DVector&uv_end,unsigned long flags,EEngineInterpolationSet interpolation);
}
