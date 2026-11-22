#include <stdio.h>

#ifndef FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#endif
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_BITMAP
#define FABLE_RETAIL_CSURFACE_SIZE_OUTOFLINE
#define FABLE_RETAIL_C2DEXTENTSI_ORIGINAL_CTOR_BODIES_PASS22
#include "rebuild_abi.h"
#include "engine/C2DExtentsI.h"
#include "engine/CSurface.h"

typedef unsigned long (CSurface::*SurfaceExtent)() const;
const SurfaceExtent& FABLE_CDECL VerifyWidthFunction();
const SurfaceExtent& FABLE_CDECL VerifyHeightFunction();

static long Width;
static long Height;
static unsigned DimensionReads;

CSurface::~CSurface() {}

C2DExtentsI CSurface::GetDimensions() const
{
    ++DimensionReads;
    return C2DExtentsI(Width, Height);
}

int main()
{
    CSurface surface;
    const SurfaceExtent width = VerifyWidthFunction();
    const SurfaceExtent height = VerifyHeightFunction();
    Width = 1;
    Height = 4096;
    if ((surface.*width)() != 1UL || (surface.*height)() != 4096UL)
        return 1;
    Width = 193;
    Height = 0;
    if ((surface.*height)() != 0UL || (surface.*width)() != 193UL || DimensionReads != 4)
        return 2;
    puts("009f3da0_OWNER_TEST PASS");
    return 0;
}
