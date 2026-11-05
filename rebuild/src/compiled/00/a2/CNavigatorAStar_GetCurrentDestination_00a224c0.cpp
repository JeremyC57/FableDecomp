// Leaf: ?GetCurrentDestination@CNavigatorAStar@@QAEPAXXZ
struct CNavigatorAStar { char pad[0x7c]; char sub; void* GetCurrentDestination(); };
void* CNavigatorAStar::GetCurrentDestination(){ return &this->sub; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetCurrentDestination@CNavigatorAStar@@UBEABVC3DVector@@XZ); evidence: RTTI vtable slot + PDB name set.
