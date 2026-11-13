#ifndef FABLE_ENGINE_CENGINERENDERINFOMESHBASE_H
#define FABLE_ENGINE_CENGINERENDERINFOMESHBASE_H
#include "fable_compiler.h"
class CEngineLocalLightArray;class C3DMesh2;
// Layout: Ego_r/FableWin original12-byte type; native SetupMesh2D00bcf430 accesses0/4/8.
class CEngineRenderInfoMeshBase {public:CEngineLocalLightArray* Lights;const C3DMesh2* Mesh;unsigned long LOD;};
FABLE_STATIC_ASSERT(sizeof(CEngineRenderInfoMeshBase)==12);
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEREPEATEDSTATICMESH
// Retail opt-in (chunk 2028, row 0x00B9A4A0): Ego_r.pdb CEngineRenderInfoRepeatedStaticMesh (12 bytes: derived with no own member; the
// RenderInfo of CEngineInternalPrimitiveRepeatedStaticMesh +0xA4).
class CEngineRenderInfoRepeatedStaticMesh : public CEngineRenderInfoMeshBase {
};
FABLE_STATIC_ASSERT(sizeof(CEngineRenderInfoRepeatedStaticMesh)==12);
#endif
#endif
