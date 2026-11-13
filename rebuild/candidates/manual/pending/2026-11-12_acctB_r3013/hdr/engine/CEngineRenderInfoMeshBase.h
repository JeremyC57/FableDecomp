#ifndef FABLE_ENGINE_CENGINERENDERINFOMESHBASE_H
#define FABLE_ENGINE_CENGINERENDERINFOMESHBASE_H
// New header (chunk 2028, row 0x00B9A4A0): CEngineRenderInfoMeshBase (Ego_r.pdb, 12 bytes: Lights +0x00, Mesh +0x04, LOD +0x08;
// fable_types.h) and CEngineRenderInfoRepeatedStaticMesh (Ego_r.pdb, 12 bytes: derived with no own member; the RenderInfo of
// CEngineInternalPrimitiveRepeatedStaticMesh +0xA4). The row stores Mesh +0xA8 and Lights +0xA4.
#include "rebuild_abi.h"
class CEngineLocalLightArray;
class C3DMesh2;
class CEngineRenderInfoMeshBase {
public:
    CEngineLocalLightArray* Lights;                                      // +0x00
    C3DMesh2*               Mesh;                                        // +0x04
    unsigned long           LOD;                                         // +0x08
};
class CEngineRenderInfoRepeatedStaticMesh : public CEngineRenderInfoMeshBase {
};
FABLE_STATIC_ASSERT(sizeof(CEngineRenderInfoRepeatedStaticMesh) == 0xc);
#endif
