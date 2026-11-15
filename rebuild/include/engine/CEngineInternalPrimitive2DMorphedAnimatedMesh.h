#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVE2DMORPHEDANIMATEDMESH_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVE2DMORPHEDANIMATEDMESH_H
#ifdef FABLE_RETAIL_PASS22_ANIMATED_MESH_RENDER
// Layout: original PDB bases/fields and native00BD71B0 operands; no synthetic members.
#include "engine/CEngineInternalPrimitive2DAnimatedMesh.h"
#include "engine/CEngineInternalPrimitiveMorphInfo.h"
class CEngineInternalPrimitive2DMorphedAnimatedMesh:public CEngineInternalPrimitive2DAnimatedMesh {
public: CEngineInternalPrimitiveMorphInfo Morph;
};
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitive2DMorphedAnimatedMesh)==0x188);
#endif
#endif
