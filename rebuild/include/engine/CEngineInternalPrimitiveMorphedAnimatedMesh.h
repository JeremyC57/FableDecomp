#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVEMORPHEDANIMATEDMESH_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVEMORPHEDANIMATEDMESH_H
#ifdef FABLE_RETAIL_PASS22_ANIMATED_MESH_RENDER
// Layout: original PDB bases/fields and native00BD71B0 operands; no synthetic members.
#include "engine/CEngineInternalPrimitiveAnimatedMesh.h"
#include "engine/CEngineInternalPrimitiveMorphInfo.h"
class CEngineInternalPrimitiveMorphedAnimatedMesh:public CEngineInternalPrimitiveAnimatedMesh {
public: CEngineInternalPrimitiveMorphInfo Morph;
};
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitiveMorphedAnimatedMesh)==0x160);
#endif
#endif
