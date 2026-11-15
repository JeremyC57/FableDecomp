#ifndef FABLE_ENGINE_CENGINESUBPRIMITIVESTATICMESH_H
#define FABLE_ENGINE_CENGINESUBPRIMITIVESTATICMESH_H
#ifdef FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
// Both original PDB class/field/method records; native parent00BA1140 uses these original owners.
#include "engine/CEngineSubPrimitiveRepeatedStaticMesh.h"
class CEngineInternalPrimitiveStaticMesh;
class CEngineSubPrimitiveStaticMesh:public CEngineSubPrimitiveMeshBase {
public:
 CEngineSubPrimitiveStaticMesh(CEngineInternalPrimitiveStaticMesh*,C3DPrimitive2*,ERenderMode,bool);
 virtual ~CEngineSubPrimitiveStaticMesh();
};
FABLE_STATIC_ASSERT(sizeof(CEngineSubPrimitiveStaticMesh)==36);
#endif
#endif
