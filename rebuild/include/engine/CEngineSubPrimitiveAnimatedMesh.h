#ifndef FABLE_ENGINE_CENGINESUBPRIMITIVEANIMATEDMESH_H
#define FABLE_ENGINE_CENGINESUBPRIMITIVEANIMATEDMESH_H
#ifdef FABLE_RETAIL_PASS22_ANIMATED_MESH_RENDER
// Layout: original PDB bases/fields and native00BD71B0 operands; no synthetic members.
#include "engine/CEngineSubPrimitiveRepeatedStaticMesh.h"
class CEngineInternalPrimitiveAnimatedMesh;
class CEngineSubPrimitiveAnimatedMesh:public CEngineSubPrimitiveMeshBase {
public:
 CEngineSubPrimitiveAnimatedMesh(CEngineInternalPrimitiveAnimatedMesh*,C3DPrimitive2*,ERenderMode,bool);
 virtual ~CEngineSubPrimitiveAnimatedMesh();
};
FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitiveAnimatedMesh)==36);
#endif
#endif
