// Leaf: ?GetMorphInfo@CEngineInternalPrimitive2DMorphedAnimatedMesh@@QAEPAUS_00bd9820@@XZ
struct S_00bd9820;
struct CEngineInternalPrimitive2DMorphedAnimatedMesh { char pad[0x150]; S_00bd9820* sub; S_00bd9820* GetMorphInfo(); };
S_00bd9820* CEngineInternalPrimitive2DMorphedAnimatedMesh::GetMorphInfo() { return (S_00bd9820*)&this->sub; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetMorphInfo@CEngineInternalPrimitive2DMorphedAnimatedMesh@@UAEPAVCEngineInternalPrimitiveMorphInfo@@XZ); evidence: RTTI vtable slot + PDB name set.
