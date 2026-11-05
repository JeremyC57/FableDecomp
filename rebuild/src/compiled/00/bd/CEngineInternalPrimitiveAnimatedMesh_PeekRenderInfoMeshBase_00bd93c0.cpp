// Leaf: ?PeekRenderInfoMeshBase@CEngineInternalPrimitiveAnimatedMesh@@QAEPAUS_00bd93c0@@XZ
struct S_00bd93c0;
struct CEngineInternalPrimitiveAnimatedMesh { char pad[0xe0]; S_00bd93c0* sub; S_00bd93c0* PeekRenderInfoMeshBase(); };
S_00bd93c0* CEngineInternalPrimitiveAnimatedMesh::PeekRenderInfoMeshBase() { return (S_00bd93c0*)&this->sub; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?PeekRenderInfoMeshBase@CEngineInternalPrimitiveAnimatedMesh@@UBEPBVCEngineRenderInfoMeshBase@@XZ); evidence: RTTI vtable slot + PDB name set.
