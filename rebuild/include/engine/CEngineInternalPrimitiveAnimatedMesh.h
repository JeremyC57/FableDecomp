#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVEANIMATEDMESH_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVEANIMATEDMESH_H
// Retail opt-in (Codex pass18): all original fields preserved with native complete sizes.
#include "engine/CEngineInternalPrimitiveMeshBase.h"
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_WORLDPOS
#ifndef FABLE_RETAIL_C3DPOSITIONINTERPOLATOR
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR
#endif
#include "engine/C3DPositionInterpolator.h"
#endif
class C3DMesh2;
class CEngineInternalPrimitiveAnimatedMesh:public CEngineInternalPrimitiveMeshBase {
public:
 virtual void CalcInterpolatedObjectTransform(CMatrix3x4&,float,bool)const;
 virtual void InitialiseSimulation(const C3DMeshInfo*,const C3DMesh2&);
 unsigned char Anim[20]; // Original C3DAnimationInterpolator
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_WORLDPOS
 // Opt-in (chunk 1020, row 0x00BCF4F0): the member with its Ego_r type (same 28 bytes at +0x84).
 C3DPositionInterpolator WorldPos;
#else
 unsigned char WorldPos[28]; // Original C3DPositionInterpolator
#endif
 unsigned char Orientation[52]; // Original C3DOrientationInterpolator
 unsigned char Cache[16]; // Original CCache
private:
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_SIZE128
 // Opt-in (chunk 1020, row 0x00BCF4F0): retail's object is 0x128 bytes, 4 fewer than Ego_r's 300 -- CEngineInternalPrimitive2DAnimatedMesh
 // keeps Initialised at +0x128 and Layer at +0x12C (Update 0x00BD2BC0 `mov [esi+128h],bl` / `mov [esi+12Ch],..`; Initialise 0x00BCF4F6
 // `mov eax,[esi+12Ch]`), and CEngineInternalPrimitiveMorphedAnimatedMesh::Update 0x00BD2B70 addresses its own first member at +0x128.
 // OPEN: which member is 4 bytes shorter in retail is not proven (Anim +0x70 / WorldPos +0x84 / Orientation +0xA0 are proven by the
 // landed 0x00BCF670 / 0x00BCF760 / 0x00BCF930); the shortfall is put in this opaque tail, which no landed row reads.
 unsigned char RenderInfo[68]; // Original CEngineRenderInfoAnimatedMesh (retail size unproven, see above)
};
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitiveAnimatedMesh)==0x128);
#else
 unsigned char RenderInfo[72]; // Original CEngineRenderInfoAnimatedMesh
};
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitiveAnimatedMesh)==300);
#endif
#endif
