#ifndef FABLE_ENGINE_C3DPOSITIONINTERPOLATOR_H
#define FABLE_ENGINE_C3DPOSITIONINTERPOLATOR_H
// Retail opt-in FABLE_RETAIL_C3DPOSITIONINTERPOLATOR (chunk 1020, row 0x00BCF4F0 CEngineInternalPrimitive2DAnimatedMesh::Initialise):
// Ego_r TPI C3DPositionInterpolator, 28 bytes: vptr +0 (FableWin ??1C3DPositionInterpolator@@UAE@XZ is virtual), StartPosition +4,
// EndPosition +0x10. Ctors ??0C3DPositionInterpolator@@QAE@XZ / @ABVC3DVector@@0@Z (Ego_r 0x00536060) declared only.
#ifdef FABLE_RETAIL_C3DPOSITIONINTERPOLATOR
#include "fable_compiler.h"
#include "engine/C3DVector.h"
class C3DPositionInterpolator {
public:
 C3DPositionInterpolator();
 C3DPositionInterpolator(const C3DVector& startPosition,const C3DVector& endPosition);
 virtual ~C3DPositionInterpolator();
 C3DVector StartPosition;
 C3DVector EndPosition;
};
FABLE_STATIC_ASSERT(sizeof(C3DPositionInterpolator)==28);
#endif
#endif
