#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVEANIMATEDMESH_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVEANIMATEDMESH_H
// Retail opt-in (Codex pass18): all original fields preserved with native complete sizes.
#include "engine/CEngineInternalPrimitiveMeshBase.h"
class C3DMesh2;
class CEngineInternalPrimitiveAnimatedMesh:public CEngineInternalPrimitiveMeshBase {
public:
 virtual void CalcInterpolatedObjectTransform(CMatrix3x4&,float,bool)const;
 virtual void InitialiseSimulation(const C3DMeshInfo*,const C3DMesh2&);
 unsigned char Anim[20]; // Original C3DAnimationInterpolator
 unsigned char WorldPos[28]; // Original C3DPositionInterpolator
 unsigned char Orientation[52]; // Original C3DOrientationInterpolator
 unsigned char Cache[16]; // Original CCache
private:
 unsigned char RenderInfo[72]; // Original CEngineRenderInfoAnimatedMesh
};
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitiveAnimatedMesh)==300);
#endif
