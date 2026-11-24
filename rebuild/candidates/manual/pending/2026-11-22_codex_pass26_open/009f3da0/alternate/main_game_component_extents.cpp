// Verification unit for the matching main_game_component.obj surface accessors.
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_BITMAP
#define FABLE_RETAIL_C2DEXTENTSI_ORIGINAL_CTOR_BODIES_PASS22
#include "rebuild_abi.h"
#include "engine/C2DExtentsI.h"
#include "engine/CSurface.h"

// Materialize the two original inline members in a compilation with GetDimensions declared.
typedef unsigned long (CSurface::*SurfaceExtent)() const;
const SurfaceExtent& FABLE_CDECL VerifyWidthFunction()
{
    static const SurfaceExtent function = &CSurface::GetWidth;
    return function;
}
const SurfaceExtent& FABLE_CDECL VerifyHeightFunction()
{
    static const SurfaceExtent function = &CSurface::GetHeight;
    return function;
}
