// Leaf: ?GetMorphInfo@CEngineInternalPrimitiveMorphedStaticMesh@@QAEPAUSub_00baa370@@XZ
struct Sub_00baa370;
struct CEngineInternalPrimitiveMorphedStaticMesh { char pad[0xac]; Sub_00baa370* sub; Sub_00baa370* GetMorphInfo(); };
Sub_00baa370* CEngineInternalPrimitiveMorphedStaticMesh::GetMorphInfo() { return (Sub_00baa370*)&this->sub; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetMorphInfo@CEngineInternalPrimitiveMorphedStaticMesh@@UAEPAVCEngineInternalPrimitiveMorphInfo@@XZ); evidence: RTTI vtable slot + PDB name set.
