#ifndef FABLE_ENGINE_CENGINERENDERINFOANIMATEDMESH_H
#define FABLE_ENGINE_CENGINERENDERINFOANIMATEDMESH_H
#include "engine/CEngineRenderInfoMeshBase.h"
#include "engine/CMatrix3x4.h"
class CMatrix4x4;
// Layout: both original PDBs size72; native SetupMesh2D RenderFrame+124 and BoneTransforms+120 from owner+e0.
class CEngineRenderInfoAnimatedMesh:public CEngineRenderInfoMeshBase {
public:CMatrix3x4 ObjectTransform;float Distance;CMatrix4x4* BoneTransforms;long RenderFrame;
};
FABLE_STATIC_ASSERT(sizeof(CEngineRenderInfoAnimatedMesh)==72);
#endif
