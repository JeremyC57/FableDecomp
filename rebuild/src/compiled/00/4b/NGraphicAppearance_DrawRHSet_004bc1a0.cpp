// Leaf: ?DrawRHSet@NGraphicAppearance@@YIXABVCEnginePrimitivePrepareInfo@@ABVC3DVector@@ABVCRightHandedSet@@MJ@Z
// Flags:
#define FABLE_RETAIL_CENGINEPRIMITIVELINE_INLINE_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CRIGHTHANDEDSET
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitiveEnums.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CEnginePrimitiveLine.h"
#include "engine/CRightHandedSet.h"

namespace NGraphicAppearance
{
void FABLE_FASTCALL DrawRHSet(const CEnginePrimitivePrepareInfo& info, const C3DVector& coord,
                             const CRightHandedSet& rhset, float width, long alpha)
{
    info.Engine.AddPrimitive(CEnginePrimitiveLine(
        0, coord, coord + rhset.GetUp(), CRGBColour(100, 100, 255, (unsigned char)alpha),
        width, ENGINE_PRIMITIVE_BLEND_SOLID, 0, true, ENGINE_INTERPOLATION_SET_DEFAULT, ENGINE_PRIMITIVE_BLEND_OP_ADD));
    info.Engine.AddPrimitive(CEnginePrimitiveLine(
        0, coord, coord + rhset.GetRight(), CRGBColour(255, 100, 100, (unsigned char)alpha),
        width, ENGINE_PRIMITIVE_BLEND_SOLID, 0, true, ENGINE_INTERPOLATION_SET_DEFAULT, ENGINE_PRIMITIVE_BLEND_OP_ADD));
    info.Engine.AddPrimitive(CEnginePrimitiveLine(
        0, coord, coord + rhset.GetForward(), CRGBColour(255, 255, 255, (unsigned char)alpha),
        width, ENGINE_PRIMITIVE_BLEND_SOLID, 0, true, ENGINE_INTERPOLATION_SET_DEFAULT, ENGINE_PRIMITIVE_BLEND_OP_ADD));
}
}