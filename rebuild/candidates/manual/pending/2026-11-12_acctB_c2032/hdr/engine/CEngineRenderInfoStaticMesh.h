#ifndef FABLE_ENGINE_CENGINERENDERINFOSTATICMESH_H
#define FABLE_ENGINE_CENGINERENDERINFOSTATICMESH_H
#include "rebuild_abi.h"
class CEngineLocalLightArray;class C3DMesh2;
// Complete types in both original PDBs; native RenderInfo occupies12bytes.
class CEngineRenderInfoMeshBase {
public:
 void SetMesh(const C3DMesh2*,unsigned long);
 CEngineLocalLightArray* Lights;
 const C3DMesh2* Mesh;
 unsigned long LOD;
};
class CEngineRenderInfoStaticMesh:public CEngineRenderInfoMeshBase {};
FABLE_STATIC_ASSERT(sizeof(CEngineRenderInfoStaticMesh)==12);
#endif
