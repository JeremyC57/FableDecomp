#ifndef FABLE_ENGINE_CENGINERENDERINFOMESHBASE_H
#define FABLE_ENGINE_CENGINERENDERINFOMESHBASE_H
#include "fable_compiler.h"
class CEngineLocalLightArray;class C3DMesh2;
// Layout: Ego_r/FableWin original12-byte type; native SetupMesh2D00bcf430 accesses0/4/8.
class CEngineRenderInfoMeshBase {public:CEngineLocalLightArray* Lights;const C3DMesh2* Mesh;unsigned long LOD;};
FABLE_STATIC_ASSERT(sizeof(CEngineRenderInfoMeshBase)==12);
#endif
