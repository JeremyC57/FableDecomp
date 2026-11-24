#ifndef FABLE_ENGINE_CSURFACELOCKEDRECT_H
#define FABLE_ENGINE_CSURFACELOCKEDRECT_H
#include "engine/C2DExtentsI.h"
// Original PDB data and user copy/default constructors, size10.
#ifdef FABLE_RETAIL_CSURFACELOCKEDRECT_ORIGINAL_IMPLICIT_CTORS_PASS26
// Original default/copy ctor method attributes are both0x103; neither is source-declared.
class CSurfaceLockedRect{public:C2DExtentsI Dimensions;long Pitch;unsigned char* PMem;};
FABLE_STATIC_ASSERT(sizeof(CSurfaceLockedRect)==0x10);
#else
class CSurfaceLockedRect{public:C2DExtentsI Dimensions;long Pitch;unsigned char* PMem;CSurfaceLockedRect(){}CSurfaceLockedRect(const CSurfaceLockedRect& o):Dimensions(o.Dimensions),Pitch(o.Pitch),PMem(o.PMem){}};
#endif
#endif
