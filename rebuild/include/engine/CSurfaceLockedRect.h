#ifndef FABLE_ENGINE_CSURFACELOCKEDRECT_H
#define FABLE_ENGINE_CSURFACELOCKEDRECT_H
#include "engine/C2DExtentsI.h"
// Original PDB data and user copy/default constructors, size10.
class CSurfaceLockedRect{public:C2DExtentsI Dimensions;long Pitch;unsigned char* PMem;CSurfaceLockedRect(){}CSurfaceLockedRect(const CSurfaceLockedRect& o):Dimensions(o.Dimensions),Pitch(o.Pitch),PMem(o.PMem){}};
#endif
