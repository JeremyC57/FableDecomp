// Leaf: ?GetMorphInfo@CEngineInternalPrimitiveMorphedAnimatedMesh@@QAEPAUS_00bd9a30@@XZ
struct S_00bd9a30;
struct CEngineInternalPrimitiveMorphedAnimatedMesh { char pad[0x128]; S_00bd9a30* sub; S_00bd9a30* GetMorphInfo(); };
S_00bd9a30* CEngineInternalPrimitiveMorphedAnimatedMesh::GetMorphInfo() { return (S_00bd9a30*)&this->sub; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetMorphInfo@CEngineInternalPrimitiveMorphedAnimatedMesh@@UAEPAVCEngineInternalPrimitiveMorphInfo@@XZ); evidence: RTTI vtable slot + PDB name set.
