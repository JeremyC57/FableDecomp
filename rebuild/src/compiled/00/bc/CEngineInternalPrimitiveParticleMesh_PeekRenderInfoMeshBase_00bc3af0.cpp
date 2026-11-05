// Leaf: ?PeekRenderInfoMeshBase@CEngineInternalPrimitiveParticleMesh_Methods@@QAEPAVCEngineLocalLightArray@@XZ
#include "engine/CEngineInternalPrimitiveParticleMesh.h"  // retyped onto the PDB layout; byte parity re-verified
class CEngineLocalLightArray;
struct CEngineInternalPrimitiveParticleMesh_Methods : CEngineInternalPrimitiveParticleMesh {
    CEngineLocalLightArray* PeekRenderInfoMeshBase();
};
CEngineLocalLightArray* CEngineInternalPrimitiveParticleMesh_Methods::PeekRenderInfoMeshBase() { return (CEngineLocalLightArray*)&this->RenderInfo_Lights; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?PeekRenderInfoMeshBase@CEngineInternalPrimitiveParticleMesh@@UBEPBVCEngineRenderInfoMeshBase@@XZ); evidence: RTTI vtable slot + PDB name set.
