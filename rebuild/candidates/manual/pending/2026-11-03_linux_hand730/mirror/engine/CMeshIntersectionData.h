#ifndef FABLE_ENGINE_CMESHINTERSECTIONDATA_H
#define FABLE_ENGINE_CMESHINTERSECTIONDATA_H
#include "engine/C3DVector.h"
#include "rebuild_abi.h"
// Ego_r TPI0x3104; native0074a821 copies IntersectionPosition0/4/8 from output.
class CMeshIntersectionData { public:C3DVector IntersectionPosition,Normal;float DistanceSquared; };
FABLE_STATIC_ASSERT(sizeof(CMeshIntersectionData)==28);
#endif
